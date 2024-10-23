/*/
 *  �t�@�C����		�F	EnemyBulletManager.h
 *  �T�v			�F	�G�̒e���ꊇ�Ǘ�����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/25
/*/
#pragma once

#include "EnemyBullet.h"
#include <vector>
#include "../Effect/LandingEffect.h"

class EnemyBulletManager
{
public:
	EnemyBulletManager();
	~EnemyBulletManager() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	//	�e��ǉ�����
	void AddBullet(std::unique_ptr<EnemyBullet> bullet);
	//	�����蔻����Ƃ�
	bool CheckHitBullet(Collider* col, Collider* dodge);

	//	���g���C���̃t���O������
	void RetryGame();

private:
	//	�e���i�[����z��
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets;
	//	�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	���e�G�t�F�N�g
	std::unique_ptr<LandingEffect> m_landingEffect;

};