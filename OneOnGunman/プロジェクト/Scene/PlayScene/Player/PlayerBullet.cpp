/*/
 *  �t�@�C����		�F	PlayerBullet.cpp
 *  �T�v			�F	�v���C���[�̒e
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#include "pch.h"
#include "PlayerBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "ReadData.h"

//	�e�̃X�s�[�h
#define SPEED	(30.f)
//	�e�̗L�����ԁi�b�j
#define VALID_TIME	(5.f)

PlayerBullet::PlayerBullet() :
	m_isValid(false),
	m_validTimer(0.f)
{
}

void PlayerBullet::Initialize()
{
	m_collider = std::make_unique<Sphere>(0.25f);

	m_model = std::make_unique<BulletAppearance>();
	m_model->Initialize();

	m_landingEffect = std::make_unique<LandingEffect>();
	m_landingEffect->Initialize();
}

void PlayerBullet::Update(float elapsedTime)
{
	m_landingEffect->Update(elapsedTime);

	//	���Ԃ��v��
	m_validTimer += elapsedTime;

	//	�w��̎��Ԃ𒴂��Ă�����e������
	if (m_validTimer > VALID_TIME) m_isValid = false;

	//	�����蔻��̗L���t���O���X�V
	m_collider->SetIsValid(m_isValid);

	//	���̃I�u�W�F�N�g�ɐڐG������e������
	if (m_collider->GetIsHit())
	{
		//	�G�t�F�N�g�̍Đ�
		if (m_isValid) m_landingEffect->StartEffect(m_position, SimpleMath::Vector3(0.f, 0.f, 1.f));
	}

	//	�e���L���ȏ�ԂłȂ���΁A�������Ȃ�
	if (!m_isValid) return;

	//	�e���ړ�������
	m_position += m_velocity * SPEED * elapsedTime;

	//	���f�����ړ�
	m_model->Update(elapsedTime, m_position);

	//	�����蔻���Ǐ]
	m_collider->SetPosition(m_position);
}

void PlayerBullet::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	m_landingEffect->Render(view, proj);

	//	�L���ȏ�ԂłȂ���Ώ������Ȃ�
	if (!m_isValid) return;

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(m_position);

	m_model->Render(view, proj);
}

void PlayerBullet::Finalize()
{
	m_model->Finalize();
	m_landingEffect->Finalize();
}

void PlayerBullet::RetryGame()
{
	m_landingEffect->StopEffect();
}

void PlayerBullet::CreateBullet(DirectX::SimpleMath::Vector3 createPos, DirectX::SimpleMath::Vector3 moveAngle)
{
	m_position = createPos;
	m_velocity = moveAngle;
	m_firstVel = moveAngle;
	m_isValid = true;
	m_validTimer = 0.f;
	m_model->CreateBullet(moveAngle);
}



BulletAppearance::BulletAppearance() :
	m_timer(0.f),
	m_radian(0.f)
{
}

void BulletAppearance::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̒萔�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_luminous = std::make_unique<BulletEffect>(0.4f, L"Resources/CSO/BulletEffect/BulletEffectVS.cso", L"Resources/CSO/BulletEffect/LuminousPS.cso");
	m_thunder = std::make_unique<BulletEffect>(0.4f, L"Resources/CSO/BulletEffect/BulletEffectVS.cso", L"Resources/CSO/BulletEffect/ThuderPS.cso");
	m_aura = std::make_unique<BulletEffect>(0.5f, L"Resources/CSO/BulletEffect/AuraVS.cso", L"Resources/CSO/BulletEffect/AuraPS.cso");
}

void BulletAppearance::Update(float elapsedTime, DirectX::SimpleMath::Vector3 position)
{
	m_timer += elapsedTime;
	m_position = position;
}

void BulletAppearance::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateRotationY(m_radian);
	world *= rotate;
	world *= SimpleMath::Matrix::CreateTranslation(m_position);

	ConstBuffer cbuff;
	cbuff.position = SimpleMath::Vector3(m_position.x, m_position.y, m_position.z);
	cbuff.time = m_timer;
	cbuff.rotate = rotate.Transpose();
	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�e�G�t�F�N�g��`��
	m_luminous->Render(world, view, proj, m_CBuffer.Get());
	m_thunder->Render(world, view, proj, m_CBuffer.Get());
	m_aura->Render(world, view, proj, m_CBuffer.Get());
}

void BulletAppearance::Finalize()
{
}

void BulletAppearance::CreateBullet(SimpleMath::Vector3 velocity)
{
	m_timer = 0.f;
	m_radian = atan2f(velocity.x, velocity.z);
}



BulletAppearance::BulletEffect::BulletEffect(float d, const wchar_t* vertex, const wchar_t* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	m_model = GeometricPrimitive::CreateSphere(context, d);

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, psBlob;
	vsBlob = DX::ReadData(vertex);
	psBlob = DX::ReadData(pixel);

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(
		DirectX::VertexPositionNormalTexture::InputElements,
		DirectX::VertexPositionNormalTexture::InputElementCount,
		vsBlob.data(),
		vsBlob.size(),
		m_inputLayout.GetAddressOf()
	);

}

void BulletAppearance::BulletEffect::Render(const SimpleMath::Matrix& world,
											const SimpleMath::Matrix& view,
											const SimpleMath::Matrix& proj,
											ID3D11Buffer* cbuff)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::White, nullptr, false, [&] {

		//	�V�F�[�_�[�Ƀo�b�t�@��n��
		ID3D11Buffer* cb[1] = { cbuff };
		context->VSSetConstantBuffers(1, 1, cb);
		context->PSSetConstantBuffers(1, 1, cb);

		//	�������`��w��
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);
		//	�J�����O�͍�����
		context->RSSetState(states->CullCounterClockwise());
		//	�[�x�o�b�t�@�̐ݒ�
		context->OMSetDepthStencilState(states->DepthDefault(), 0);

		context->IASetInputLayout(m_inputLayout.Get());
		context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});

	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void BulletAppearance::BulletEffect::Finalize()
{
}
