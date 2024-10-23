/*/
 *  �t�@�C����		�F	InfinitePlane.h
 *  �T�v			�F	�w�i�p�̕���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/24
/*/
#pragma once

class InfinitePlane
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix proj;
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Vector4 time;
	};

public:
	InfinitePlane();
	~InfinitePlane() = default;

	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	void CreateShader();

private:
	//	�V�F�[�_�[�ɏ���n�����߂̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;


	//	�A�j���[�V�����p�̃^�C�}�[
	float m_timer;

};
