/*/
 *  �t�@�C����		�F	HomingBullet.cpp
 *  �T�v			�F	�v���C���[��ǔ�����e
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/21
/*/
#include "pch.h"
#include "HomingBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"

//	�e�̐�������
#define ALIVE_TIME	(3.f)

HomingBullet::HomingBullet(SimpleMath::Vector3 position, Player* target, float speed) :
	EnemyBullet(),
	m_target(target),
	m_speed(speed),
	m_timer(0.f)
{
	SetCollider(new Sphere(0.4f));

	SetPosition(position);

	m_direction = ToPlayerVector();
}

void HomingBullet::Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	m_model = GeometricPrimitive::CreateSphere(context, 0.8f);

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

bool HomingBullet::Update(float elapsedTime)
{
	//	�^�C�}�[�̍X�V
	m_timer += elapsedTime;

	//	�^�[�Q�b�g�ւ̕����x�N�g�����擾
	SimpleMath::Vector3 toPlayerVec = ToPlayerVector();
	//	�v���C���[�ւ̕������i�s�����ɑ΂��ċ��e�͈͂Ȃ�ǔ�����
	if (m_direction.Dot(toPlayerVec) >= 0.997f)
	{
		m_direction += toPlayerVec - m_direction * 0.1f * elapsedTime;
		m_direction.Normalize();
	}

	//	�ʒu�̍X�V
	SimpleMath::Vector3 delta = GetPosition() + (m_direction * m_speed * elapsedTime);
	SetPosition(delta);

	//	�������Ԃ��߂��������
	if (m_timer > ALIVE_TIME) return false;

	//	���̃I�u�W�F�N�g�ɓ������������
	if (GetCollider()->GetIsHit()) return false;

	return true;
}

void HomingBullet::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::Yellow, nullptr, false, [&] {
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

void HomingBullet::Finalize()
{
}

SimpleMath::Vector3 HomingBullet::ToPlayerVector()
{
	SimpleMath::Vector3 ret = m_target->GetCenter() - GetPosition();
	ret.Normalize();
	
	return ret;
}
