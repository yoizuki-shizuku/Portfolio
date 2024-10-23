/*/
 *  �t�@�C����		�F	Shadow.cpp
 *  �T�v			�F	�|���S�����g���ȈՓI�ȉe��\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/06
/*/
#include "pch.h"
#include "Shadow.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "WICTextureLoader.h"

//	�e��\�����鍂��
#define SHADOW_VIEW_HEIGHT	(0.1f)

Shadow::Shadow()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	�쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetTextureEnabled(true);

	//	�摜�f�[�^�ǂݍ���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Texture/Play/Particle.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(device, m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	m_size = SimpleMath::Vector2::One;
}

void Shadow::Update(DirectX::SimpleMath::Vector3 position)
{
	m_position = position;
	m_position.y = SHADOW_VIEW_HEIGHT;
}

void Shadow::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_effect->SetView(view);
	m_effect->SetProjection(proj);
	m_effect->SetWorld(SimpleMath::Matrix::CreateTranslation(m_position));
	m_effect->SetTexture(m_texture.Get());
	m_effect->SetColorAndAlpha(SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f));
	m_effect->Apply(context);

	//	�������`��w��
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFF);
	//	�J�����O�͍�����
	context->RSSetState(states->CullCounterClockwise());
	//	�[�x�o�b�t�@�̐ݒ�
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�f�t�H���g�̒��_�ʒu
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-m_size.x, 0.f, -m_size.y), SimpleMath::Vector2(0.f, 0.f)),	//	����
		VertexPositionTexture(SimpleMath::Vector3( m_size.x, 0.f, -m_size.y), SimpleMath::Vector2(1.f, 0.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3( m_size.x, 0.f,  m_size.y), SimpleMath::Vector2(1.f, 1.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3(-m_size.x, 0.f,  m_size.y), SimpleMath::Vector2(0.f, 1.f)),	//	����
	};

	//	�`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

void Shadow::Finalize()
{
}
