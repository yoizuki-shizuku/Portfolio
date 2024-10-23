/*/
 *  �t�@�C����		�F	InfinitePlane.h
 *  �T�v			�F	�w�i�p�̕���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/24
/*/
#include "pch.h"
#include "InfinitePlane.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "ReadData.h"

InfinitePlane::InfinitePlane() :
	m_timer (0.f)
{
	//	�v���~�e�B�u�o�b�`�̍쐬
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	CreateShader();
}


void InfinitePlane::Update()
{
	m_timer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
}

void InfinitePlane::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	�V�F�[�_�[�ɓn���o�b�t�@���쐬����
	ConstBuffer cbuff{};
	cbuff.view = view.Transpose();
	cbuff.proj = proj.Transpose();
	cbuff.time = SimpleMath::Vector4(m_timer, 0.f, 0.f, 0.f);

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateScale(10.f, 1.f, 3.f);
	cbuff.world = world.Transpose();

	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	�������`��w��
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);

	//	�J�����O�͍�����
	context->RSSetState(states->CullNone());

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�V�F�[�_�[���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�|���S����`��
	m_batch->Begin();

	for (int i = 0; i < 2; i++)
	{
		VertexPositionTexture vertex[4] =
		{
			VertexPositionTexture(SimpleMath::Vector3( 1.f, 0.f,  1.f), SimpleMath::Vector2(0.f, 0.f)),	//	����
			VertexPositionTexture(SimpleMath::Vector3(-1.f, 0.f,  1.f), SimpleMath::Vector2(1.f, 0.f)),	//	�E��
			VertexPositionTexture(SimpleMath::Vector3(-1.f, 0.f, -1.f), SimpleMath::Vector2(1.f, 1.f)),	//	�E��
			VertexPositionTexture(SimpleMath::Vector3( 1.f, 0.f, -1.f), SimpleMath::Vector2(0.f, 1.f)),	//	����
		};

		for (int j = 0; j < 4; j++)
		{
			vertex[j].position.y += 2.f * i;
		}

		m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	}

	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3( 2.f,  3.f, 3.6f), SimpleMath::Vector2(0.f, 0.f)),	//	����
		VertexPositionTexture(SimpleMath::Vector3(-2.f,  3.f, 3.6f), SimpleMath::Vector2(1.f, 0.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3(-2.f, -1.f, 3.6f), SimpleMath::Vector2(1.f, 1.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3( 2.f, -1.f, 3.6f), SimpleMath::Vector2(0.f, 1.f)),	//	����
	};

	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	m_batch->End();

	//	�V�F�[�_�[�̓o�^������
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void InfinitePlane::Finalize()
{

}

void InfinitePlane::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/InfinitePlane/InfinitePlaneVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/InfinitePlane/InfinitePlanePS.cso");

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
