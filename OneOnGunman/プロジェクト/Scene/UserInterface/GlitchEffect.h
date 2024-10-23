/*/
 *  �t�@�C����		�F	GlitchEffect.h
 *  �T�v			�F	�O���b�`�G�t�F�N�g��K�p�����摜��\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/11
/*/
#pragma once

#include "UserInterface.h"

class GlitchEffect
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		//	��ʂ̑傫��
		DirectX::SimpleMath::Vector4	windowSize;
	};

public:
	GlitchEffect();
	~GlitchEffect() = default;

	//	�摜�f�[�^��ǂݍ���
	void LoadTexture(const wchar_t* path);

	void Update();
	void Render();

public:
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	void SetScale(const DirectX::SimpleMath::Vector2 scale) { m_scale = scale; }
	void SetAnchor(const Anchor anchor) { m_anchor = anchor; }

private:
	//	�V�F�[�_�[�̍쐬
	void CreateShader();

private:
	//	�V�F�[�_�[�ɏ���n�����߂̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
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
	//	�傫��
	DirectX::SimpleMath::Vector2 m_scale;
	//	�A���J�[�|�C���g
	Anchor m_anchor;
	//	��ʂ̑傫��
	int m_windowWidth, m_windowHeight;
	//	�e�N�X�`���̌��̑傫��
	int m_textureWidth, m_textureHeight;

	//	���Ԍv���p
	float m_timer;

};