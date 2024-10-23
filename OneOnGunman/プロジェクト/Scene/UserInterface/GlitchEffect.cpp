/*/
 *  �t�@�C����		�F	GlitchEffect.cpp
 *  �T�v			�F	�O���b�`�G�t�F�N�g��K�p�����摜��\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/11
/*/
#include "pch.h"
#include "GlitchEffect.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "WICTextureLoader.h"
#include "ReadData.h"
#include <random>

//	�O���b�`�G�t�F�N�g���쓮�����鎞��
#define GLITCH_OPERATION	(0.2f)

GlitchEffect::GlitchEffect() :
	m_textureWidth(0),
	m_textureHeight(0),
	m_timer(0.f)
{
	//	�ϐ�������
	m_position = SimpleMath::Vector2::Zero;
	m_scale = SimpleMath::Vector2::One;
	m_anchor = Anchor::TOP_LEFT;
	RECT windowSize = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_windowWidth = windowSize.right;
	m_windowHeight = windowSize.bottom;

	m_timer = 0.f;

	//	�v���~�e�B�u�o�b�`�̍쐬
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);

	//	�V�F�[�_�[�̍쐬
	CreateShader();
}

void GlitchEffect::LoadTexture(const wchar_t* path)
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

void GlitchEffect::Update()
{
	//	�^�C�}�[�̍X�V
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	//	����傫���Ȃ�΃^�C�}�[���Đݒ肷��
	if (m_timer > GLITCH_OPERATION)
	{
		//	�Đݒ肷��l�͗����Ō���
		std::random_device seed;
		std::mt19937	   rand{ seed() };
		std::uniform_real_distribution<> dist(0.01, 1.2);

		m_timer -= static_cast<float>(dist(rand));
	}
}

void GlitchEffect::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();
	
	//	�O���b�`�G�t�F�N�g�p�̗����𐶐�
	std::random_device seed;
	std::mt19937	   rand{ seed() };
	//	���炷�ʒu(y����)
	std::uniform_real_distribution<> distY(-0.1, 1.0);
	float offsetPos = static_cast<float>(distY(rand));
	//	���炷��(x����)
	std::uniform_real_distribution<> distX(-0.1, 0.1);
	float offsetWidth = static_cast<float>(distX(rand)) * (m_timer >= 0.f);
	//	�F�����炵�̈ʒu�����炷��
	std::uniform_real_distribution<> distColor(-0.001, 0.001);
	float OffsetColor = static_cast<float>(distColor(rand));

	//	���_���
	//	Position.xy : �ʒu
	//	Position.z  : ���炷�ʒu
	//	Color.xy    : �g�k
	//	Color.z	    : �A���J�[�|�C���g
	//	Color.a		: ���炷��
	//	Tex.xy		: �摜�̃T�C�Y
	VertexPositionColorTexture vertex =
	{
		SimpleMath::Vector3(m_position.x, m_position.y, offsetPos),
		SimpleMath::Vector4(m_scale.x, m_scale.y, static_cast<float>(m_anchor), offsetWidth),
		SimpleMath::Vector2(static_cast<float>(m_textureWidth),static_cast<float>(m_textureHeight))
	};

	//	�V�F�[�_�[�ɓn���o�b�t�@���쐬����
	//	windowSize.xy : ��ʃT�C�Y
	//	windowSize.z : �F�����炵�G�t�F�N�g�̃I�t�Z�b�g��
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), OffsetColor, 1.f);

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

void GlitchEffect::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/GlitchEffect/GlitchEffectVS.cso");
	gsBlob = DX::ReadData(L"Resources/CSO/GlitchEffect/GlitchEffectGS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/GlitchEffect/GlitchEffectPS.cso");

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
