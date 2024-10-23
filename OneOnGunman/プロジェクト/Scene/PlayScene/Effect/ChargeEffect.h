/*/
 *  �t�@�C����		�F	ChargeEffect.h
 *  �T�v			�F	�v���C���[�̃Q�[�W���ő�ɂȂ����ۂ̃G�t�F�N�g
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2024/01/29
/*/
#pragma once

class ChargeEffect
{
public:
	//	�萔�o�b�t�@(���M��)
	struct ConstBuffer
	{
		float timer;
		DirectX::SimpleMath::Vector3 none;
	};

public:
	ChargeEffect();
	~ChargeEffect() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render();

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
	//	�X�e���V���}�X�N�p
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	//	�^�C�}�[
	float m_timer;
};
