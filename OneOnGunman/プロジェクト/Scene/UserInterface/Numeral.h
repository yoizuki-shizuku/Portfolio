/*/
 *  �t�@�C����		�F	Numeral.h
 *  �T�v			�F	�ꖇ�̉摜���畔���I�ɐ؂���A������\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/17
/*/
#pragma once

class Numeral
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		float windowWidth;		//	��ʂ̉���
		float windowHeight;		//	��ʂ̏c��
		float alpha;			//	�����x
		int numeral;			//	�\�����鐔��
	};

public:
	Numeral();
	~Numeral() = default;

	//	�摜�f�[�^��ǂݍ���
	void LoadTexture(const wchar_t* path);

	void Render();

public:
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	void SetRadian(const float& radian) { m_radian = radian; }
	void SetSize(const DirectX::SimpleMath::Vector2 size) { m_size = size; }
	void SetAlpha(const float& alpha) { m_alpha = alpha; }
	void SetNumber(const int& number) { m_number = number; }

private:
	//	�V�F�[�_�[�̍쐬
	void CreateShader();

private:
	//	�V�F�[�_�[�ɏ���n�����߂̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	�ʒu
	DirectX::SimpleMath::Vector2 m_position;
	//	�p�x
	float m_radian;
	//	�T�C�Y(�s�N�Z���P��)
	DirectX::SimpleMath::Vector2 m_size;
	//	�����x
	float m_alpha;
	//	�\�����鐔��
	int m_number;

};