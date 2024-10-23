/*/
 *  �t�@�C����		�F	ChargeEffect.cpp
 *  �T�v			�F	�v���C���[�̃Q�[�W���ő�ɂȂ����ۂ̃G�t�F�N�g
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2024/01/29
/*/
#include "pch.h"
#include "ChargeEffect.h"

using namespace DirectX;

#include "ReadData.h"
#include "Library/UserResources.h"

ChargeEffect::ChargeEffect() :
	m_timer(0.f)
{
}

void ChargeEffect::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/ChargeEffect/ChargeEffectVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/ChargeEffect/ChargeEffectPS.cso");

	//	�e�V�F�[�_�[�̍쐬
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		vsBlob.data(), vsBlob.size(),
		m_inputLayout.GetAddressOf());

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	�[�x�X�e���V���̍쐬 
	D3D11_DEPTH_STENCIL_DESC desc;

	desc.DepthEnable = FALSE;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// �[�x�o�b�t�@���X�V���Ȃ�
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����

	desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// �\��
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;		// �P�ȏ�Ȃ�
	//desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// �P�ȏ�Ȃ�
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// OK�@�������Ȃ�
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG�@�������Ȃ�

	desc.BackFace = desc.FrontFace;	// ���ʂ�����

	DX::ThrowIfFailed(
		device->CreateDepthStencilState(&desc, m_depthStencilState.ReleaseAndGetAddressOf())
	);
}

void ChargeEffect::Update(float elapsedTime)
{
	m_timer += elapsedTime;
}

void ChargeEffect::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-1.f, 1.f,0.f) ,SimpleMath::Vector2(0.f, 1.f)),	//	����
		VertexPositionTexture(SimpleMath::Vector3(1.f, 1.f,0.f) ,SimpleMath::Vector2(1.f, 1.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3(1.f,-1.f,0.f) ,SimpleMath::Vector2(1.f, 0.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3(-1.f,-1.f,0.f) ,SimpleMath::Vector2(0.f, 0.f)),	//	����
	};

	//	�V�F�[�_�[�ɓn���o�b�t�@���쐬����
	ConstBuffer cbuff;
	cbuff.timer = m_timer;

	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�������`��w��
	auto states = UserResources::GetInstance()->GetCommonStates();
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �J�����O�͂��Ȃ�
	context->RSSetState(states->CullNone());

	//	�[�x�o�b�t�@�̐ݒ�
	context->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�V�F�[�_�[���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�|���S����`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	//	�V�F�[�_�[�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
