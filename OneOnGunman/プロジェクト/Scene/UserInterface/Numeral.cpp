/*/
 *  �t�@�C����		�F	Numeral.cpp
 *  �T�v			�F	�ꖇ�̉摜���畔���I�ɐ؂���A������\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/17
/*/
#include "pch.h"
#include "Numeral.h"

using namespace DirectX;

#include "WICTextureLoader.h"
#include "ReadData.h"
#include "Library/UserResources.h"

Numeral::Numeral() :
	m_radian(0.f),
	m_alpha(1.f),
	m_number(0)
{
	//	�ϐ�������
	m_position = SimpleMath::Vector2::Zero;
	m_size = SimpleMath::Vector2(100.f, 100.f);

	//	�v���~�e�B�u�o�b�`�̍쐬
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	�V�F�[�_�[�̍쐬
	CreateShader();
}


void Numeral::LoadTexture(const wchar_t* path)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�摜�f�[�^�ǂݍ���
	DirectX::CreateWICTextureFromFile(device, path, nullptr, m_texture.ReleaseAndGetAddressOf());
}

void Numeral::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	���_���
	//	Position.xy : �ʒu
	//	Position.z  : �p�x
	//	Tex.xy		: �T�C�Y
	VertexPositionTexture vertex =
	{
		SimpleMath::Vector3(m_position.x, m_position.y, m_radian),
		SimpleMath::Vector2(m_size)
	};

	RECT windowSize = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();

	//	�V�F�[�_�[�ɓn���o�b�t�@���쐬����
	ConstBuffer cbuff{};
	cbuff.windowWidth = static_cast<float>(windowSize.right);
	cbuff.windowHeight = static_cast<float>(windowSize.bottom);
	cbuff.alpha = m_alpha;
	cbuff.numeral = m_number;

	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	�������`��w��
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);

	//	�J�����O�͍�����
	context->RSSetState(states->CullNone());

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�V�F�[�_�[���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�s�N�Z���V�F�[�_�[�Ƀe�N�X�`����o�^����
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	�|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex, 1);
	m_batch->End();

	//	�V�F�[�_�[�̓o�^������
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void Numeral::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/Numeral/NumeralVS.cso");
	gsBlob = DX::ReadData(L"Resources/CSO/Numeral/NumeralGS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/Numeral/NumeralPS.cso");

	//	�e�V�F�[�_�[�̍쐬
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreateGeometryShader(gsBlob.data(), gsBlob.size(), nullptr, m_geometryShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
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

