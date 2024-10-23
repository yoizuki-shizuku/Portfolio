/*/
 *  �t�@�C����		�F	TorpedoBullet.cpp
 *  �T�v			�F	�����̂悤�ɏ��X�ɉ������Ă����e
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/20
/*/
#include "pch.h"
#include "TorpedoBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

//	�e�̐�������
#define ALIVE_TIME	(3.f)
//	�ő呬�x�ɂȂ�܂łɂ����鎞��
#define MAX_TIME	(0.8f)

TorpedoBullet::TorpedoBullet(SimpleMath::Vector3 position, SimpleMath::Vector3 maxVelocity) :
	EnemyBullet(),
	m_maxVelocity(maxVelocity),
	m_nowVelocity(SimpleMath::Vector3::Zero),
	m_timer(0.f)
{
	SetCollider(new Sphere(0.3f));

	SetPosition(position);
}

void TorpedoBullet::Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	m_model = GeometricPrimitive::CreateSphere(context, 0.6f);

	m_inputLayout = input;
	m_vertexShader = vertex;
	m_pixelShader = pixel;

	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̒萔�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

bool TorpedoBullet::Update(float elapsedTime)
{
	//	�^�C�}�[�̍X�V
	m_timer += elapsedTime;

	//	���x�����X�ɏグ��
	float ratio = Useful::Ratio01(m_timer, MAX_TIME);
	float t = Easing::InQuint(ratio);
	m_nowVelocity = SimpleMath::Vector3::Lerp(SimpleMath::Vector3::Zero, m_maxVelocity, t);

	//	�ʒu�̍X�V
	SimpleMath::Vector3 delta = GetPosition() + (m_nowVelocity * elapsedTime);
	SetPosition(delta);

	//	�������Ԃ��߂��������
	if (m_timer > ALIVE_TIME) return false;

	//	���̃I�u�W�F�N�g�ɓ������������
	if (GetCollider()->GetIsHit()) return false;

	return true;
}

void TorpedoBullet::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	�^�C�}�[��0�ȏ�ŕ`�悷��
	if (m_timer < 0.f) return;

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::Orange, nullptr, false, [&] {
		ConstBuffer cbuff;
		cbuff.position = SimpleMath::Vector4(GetPosition().x, GetPosition().y, GetPosition().z, 1.f);
		cbuff.color = SimpleMath::Vector4(GetColor().x, GetColor().y, GetColor().z, 1.f);
		//	�󂯓n���p�o�b�t�@�̓��e�X�V
		context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//	�V�F�[�_�[�Ƀo�b�t�@��n��
		ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
		context->VSSetConstantBuffers(1, 1, cb);
		context->PSSetConstantBuffers(1, 1, cb);

		//	�������`��w��
		context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFF);
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

void TorpedoBullet::Finalize()
{
}
