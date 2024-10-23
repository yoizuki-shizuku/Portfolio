/*/
 *  �t�@�C����		�F	LandingEffect.h
 *  �T�v			�F	�e�̒��e���̃G�t�F�N�g
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/12
/*/
#include "pch.h"
#include "LandingEffect.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Easing.h"
#include "Library/Useful.h"
#include "WICTextureLoader.h"
#include <random>

//	�A�j���[�V�����̎���
#define ANIMATION_TIME		(2.f)

LandingEffect::LandingEffect() :
	m_timer(0.f),
	m_alpha(0.f),
	m_isAnimation(false)
{
}

void LandingEffect::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	�����Ə�����
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	m_effect = std::make_unique<AlphaTestEffect>(device);

	//	�摜�f�[�^�ǂݍ���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Texture/Play/Particle.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(device, m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

void LandingEffect::Update(float elapsedTime)
{
	//	�A�j���[�V�������Ă��Ȃ���Ώ������Ȃ�
	if (!m_isAnimation) return;

	//	�^�C�}�[�̍X�V
	m_timer += elapsedTime;

	//	�p�[�e�B�N���̍X�V
	for (auto& it : m_particles)
	{
		//	�ݒ肳�ꂽ�����ֈړ�������
		it.position += it.velocity * elapsedTime * Easing::Lerp(6.f, 0.f, Useful::Ratio01(m_timer, ANIMATION_TIME),Easing::OutQuint);
	}

	//	�����x�̕ύX
	m_alpha = Easing::Lerp(1.f, 0.f, Useful::Ratio01(m_timer, ANIMATION_TIME));

	//	�A�j���[�V�����̎��Ԃ𒴂�����~�߂�
	if (m_timer >= ANIMATION_TIME)
	{
		m_isAnimation = false;
		m_timer = 0.f;
	}
}

void LandingEffect::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	�A�j���[�V�������Ă��Ȃ���Ώ������Ȃ�
	if (!m_isAnimation) return;

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	�`��ŋ��ʂ��Ă��镔�����ɐݒ�
	m_effect->SetView(view);
	m_effect->SetProjection(proj);
	m_effect->SetTexture(m_texture.Get());

	//	�J�����̈ʒu�����߂�
	SimpleMath::Vector3 eye = ClacCameraPos(view);

	SimpleMath::Vector4 color = SimpleMath::Vector4(m_color.x, m_color.y, m_color.z, m_alpha);
	m_effect->SetColorAndAlpha(color);
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateBillboard(m_startPos, eye, SimpleMath::Vector3::Up);
	m_effect->SetWorld(world);
	m_effect->Apply(context);

	//	�������`��w��
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFF);

	//	�J�����O�͍�����
	context->RSSetState(states->CullClockwise());

	//	�[�x�o�b�t�@�̐ݒ�
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	//	�e�p�[�e�B�N���̕`��
	for (const auto& it : m_particles)
	{
			//	�f�t�H���g�̒��_�ʒu
		VertexPositionTexture vertex[4] =
		{
			VertexPositionTexture(SimpleMath::Vector3(-0.15f,  0.15f, 0.f), SimpleMath::Vector2(0.f, 0.f)),	//	����
			VertexPositionTexture(SimpleMath::Vector3( 0.15f,  0.15f, 0.f), SimpleMath::Vector2(1.f, 0.f)),	//	�E��
			VertexPositionTexture(SimpleMath::Vector3( 0.15f, -0.15f, 0.f), SimpleMath::Vector2(1.f, 1.f)),	//	�E��
			VertexPositionTexture(SimpleMath::Vector3(-0.15f, -0.15f, 0.f), SimpleMath::Vector2(0.f, 1.f)),	//	����
		};

		for (int i = 0; i < 4; i++)
		{
			vertex[i].position.x += it.position.x;
			vertex[i].position.y += it.position.y;
			vertex[i].position.z += it.position.z;
		}

		//	�`��
		m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	}

	m_batch->End();
}

void LandingEffect::Finalize()
{
	m_texture.Reset();
}

void LandingEffect::StartEffect(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 color)
{
	//	�e�ϐ��ɃA�j���[�V�������J�n���邽�߂̃t���O��ݒ�
	m_alpha = 1.f;
	m_color = color;
	m_isAnimation = true;
	m_startPos = startPos;

	for (auto& it : m_particles)
	{
		it.position = SimpleMath::Vector3::Zero;

		//	�����x�N�g���������_���ɐݒ�
		SimpleMath::Vector3 velocity = SimpleMath::Vector3::Zero;

		//	�����𐶐�
		std::random_device seed;
		std::mt19937	   rand{ seed() };
		std::uniform_real_distribution<> distY(-1.0, 1.0);

		//	�ݒ�
		velocity.x = static_cast<float>(distY(rand));
		velocity.y = static_cast<float>(distY(rand));
		velocity.z = static_cast<float>(distY(rand));

		//	���K��
		velocity.Normalize();

		//	���x�������_����
		std::uniform_real_distribution<> speed(-2.0, 2.0);
		it.velocity = velocity * static_cast<float>(speed(rand));
	}
}

void LandingEffect::StopEffect()
{
	m_isAnimation = false;
	m_timer = 0.f;
	m_alpha = 0.f;
}

SimpleMath::Vector3 LandingEffect::ClacCameraPos(const SimpleMath::Matrix& view)
{
	SimpleMath::Vector3 m1(view._11, view._12, view._13);
	SimpleMath::Vector3 m2(view._21, view._22, view._23);
	SimpleMath::Vector3 m3(view._31, view._32, view._33);
	SimpleMath::Vector3 m4(view._41, view._42, view._43);

	float tx = -m1.Dot(m4);
	float ty = -m2.Dot(m4);
	float tz = -m3.Dot(m4);

	return DirectX::SimpleMath::Vector3(tx, ty, tz);
}
