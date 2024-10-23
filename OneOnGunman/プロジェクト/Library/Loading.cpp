/*/
 *  �t�@�C����		�F	Loading.cpp
 *  �T�v			�F	���[�h��ʂ�\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/27
/*/
#include "pch.h"
#include "Loading.h"

using namespace DirectX;

#include "UserResources.h"

Loading::Loading()
{
}

void Loading::Initialize()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	//	�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetVertexColorEnabled(true);

	//	�C���v�b�g���C�A�E�g�̍쐬
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_texture = std::make_unique<GlitchEffect>();
	m_texture->LoadTexture(L"Resources/Texture/NowLoading.png");
	m_texture->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_texture->SetPosition(SimpleMath::Vector2(width, height));
}

void Loading::Update()
{
	m_texture->Update();
}

void Loading::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	���_���
	VertexPositionColor vertex[4] =
	{
		VertexPositionColor(SimpleMath::Vector3(-1.f, 1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	����
		VertexPositionColor(SimpleMath::Vector3( 1.f, 1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	�E��
		VertexPositionColor(SimpleMath::Vector3( 1.f,-1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	�E��
		VertexPositionColor(SimpleMath::Vector3(-1.f,-1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	����
	};

	//�������`��w��
	auto states = UserResources::GetInstance()->GetCommonStates();
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �J�����O�͍�����
	context->RSSetState(states->CullNone());

	//	�[�x�o�b�t�@��ݒ�
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	//	�x�[�V�b�N�G�t�F�N�g��ݒ�
	m_effect->Apply(context);
	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�|���S����`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	m_texture->Render();
}

void Loading::Finalize()
{
}
