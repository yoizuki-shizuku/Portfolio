/*/
 *  �t�@�C����		�F	UserInterface.cpp
 *  �T�v			�F	UI��\�����邽�߂̃N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/02
/*/
#include "pch.h"
#include "UserInterface.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "WICTextureLoader.h"
#include "ReadData.h"

UserInterface::UserInterface() :
	m_radian(0.f),
	m_alpha(1.f),
	m_textureWidth(0),
	m_textureHeight(0),
	m_ratio(1.f)
{
	//	�ϐ�������
	m_position = SimpleMath::Vector2::Zero;
	m_scale = SimpleMath::Vector2::One;
	m_anchor = Anchor::TOP_LEFT;
	RECT windowSize = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_windowWidth = windowSize.right;
	m_windowHeight = windowSize.bottom;
	m_direction = Direction::Right;
	m_color = SimpleMath::Vector3(1.f);

	//	�v���~�e�B�u�o�b�`�̍쐬
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);

	//	�V�F�[�_�[�̍쐬
	CreateShader();
}

void UserInterface::LoadTexture(const wchar_t* path)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�摜�̑傫����ǂݍ��ނ��߂̃��\�[�X
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	//	�摜�f�[�^�ǂݍ���
	DirectX::CreateWICTextureFromFile(device, path, resource.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());

	//	�ǂݍ��񂾉摜�f�[�^����c�Ɖ��̃T�C�Y�̃f�[�^��ǂݏo��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(resource.As(&tex));
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
}

void UserInterface::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	���_���
	//	Position.xy : �ʒu
	//	Position.z  : �p�x
	//	Color.xy    : �g�k
	//	Color.z	    : �A���J�[�|�C���g
	//	Color.w     : �����x
	//	Tex.xy		: �摜�̃T�C�Y
	VertexPositionColorTexture vertex =
	{
		SimpleMath::Vector3(m_position.x, m_position.y, m_radian),
		SimpleMath::Vector4(m_scale.x, m_scale.y, static_cast<float>(m_anchor), m_alpha),
		SimpleMath::Vector2(static_cast<float>(m_textureWidth),static_cast<float>(m_textureHeight))
	};

	//	�V�F�[�_�[�ɓn���o�b�t�@���쐬����
	ConstBuffer cbuff{};
	cbuff.windowWidth = static_cast<float>(m_windowWidth);
	cbuff.windowHeight = static_cast<float>(m_windowHeight);
	cbuff.ratio = m_ratio;
	cbuff.direction = static_cast<int>(m_direction);
	cbuff.color = SimpleMath::Vector4(m_color.x, m_color.y, m_color.z, 1.f);

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
	context->RSSetState(states->CullClockwise());

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

void UserInterface::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/UserInterface/UserInterfaceVS.cso");
	gsBlob = DX::ReadData(L"Resources/CSO/UserInterface/UserInterfaceGS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/UserInterface/UserInterfacePS.cso");

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
		DirectX::VertexPositionColorTexture::InputElements,
		DirectX::VertexPositionColorTexture::InputElementCount,
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
