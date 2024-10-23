/*/
 *  �t�@�C����		�F	LandingEffect.h
 *  �T�v			�F	�e�̒��e���̃G�t�F�N�g
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/12
/*/
#pragma once

#include <array>

struct Particle
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 velocity;
};

class LandingEffect
{
public:
	LandingEffect();
	~LandingEffect() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	//	�G�t�F�N�g�A�j���[�V�����̊J�n
	void StartEffect(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 color);

	//	�G�t�F�N�g�A�j���[�V�����̒�~
	void StopEffect();

private:
	//	view�s�񂩂�J�����̈ʒu�����߂�
	DirectX::SimpleMath::Vector3 ClacCameraPos(const DirectX::SimpleMath::Matrix& view);

	//	�z��̃T�C�Y
	static const int SIZE = 80;

private:
	//	�|���S����\������v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_effect;
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	�p�[�e�B�N��
	std::array<Particle, SIZE> m_particles;
	//	�F
	DirectX::SimpleMath::Vector3 m_color;
	//	�J�n�ʒu
	DirectX::SimpleMath::Vector3 m_startPos;

	//	�^�C�}�[
	float m_timer;
	//	�p�[�e�B�N���̓����x
	float m_alpha;
	//	�A�j���[�V�����̎��s��
	bool m_isAnimation;

};