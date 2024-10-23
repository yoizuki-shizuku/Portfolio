/*/
 *  �t�@�C����		�F	Hologram.cpp
 *  �T�v			�F	�X�e�[�W�I�����̓G�̌����ڂ��z���O�����ŕ\��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/16
/*/
#include "pch.h"
#include "Hologram.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "ReadData.h"

Hologram::Hologram() :
	m_scale(0.f),
	m_timer(0.f)
{

}

void Hologram::Initialize(const wchar_t* model, const wchar_t* tex)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO/");
	m_model = Model::CreateFromCMO(device, model, *fx);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, tex, nullptr, m_texture.ReleaseAndGetAddressOf())
	);

	CreateShader();
}

void Hologram::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_timer += elapsedTime;
}

void Hologram::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateScale(m_scale);
	world *= SimpleMath::Matrix::CreateRotationY(m_timer * 0.5f);
	world *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(1.f, -0.2f, -1.5f));
	m_model->Draw(context, *states, world, view, proj, false, [&]() {

		ConstBuffer cbuff;
		cbuff.Time = SimpleMath::Vector4(m_timer, 0.f, 0.f, 0.f);
		//	�󂯓n���p�o�b�t�@�̓��e�X�V
		context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//	�V�F�[�_�[�Ƀo�b�t�@��n��
		ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
		context->VSSetConstantBuffers(1, 1, cb);
		context->PSSetConstantBuffers(1, 1, cb);

		//	�������`��w��
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);
		//	�J�����O�͍�����
		context->RSSetState(states->CullCounterClockwise());
		//	�[�x�o�b�t�@�̐ݒ�
		context->OMSetDepthStencilState(states->DepthDefault(), 0);

		//	�摜�p�T���v���[�̓o�^
		ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
		context->PSSetSamplers(0, 1, sampler);

		//	�s�N�Z���V�F�[�_�[�Ƀe�N�X�`����o�^����
		context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

		context->IASetInputLayout(m_inputLayout.Get());
		context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		});

	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void Hologram::Finalize()
{

}

void Hologram::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, psBlob;
	vsBlob = DX::ReadData(L"Resources/CSO/Hologram/HologramVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/Hologram/HologramPS.cso");

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
		DirectX::VertexPositionNormalTangentColorTexture::InputElements,
		DirectX::VertexPositionNormalTangentColorTexture::InputElementCount,
		vsBlob.data(),
		vsBlob.size(),
		m_inputLayout.GetAddressOf()
	);

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}
