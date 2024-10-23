/*/
 *  �t�@�C����		�F	Collector.h
 *  �T�v			�F	�X�e�[�W2�̓G
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/23
/*/
#pragma once

#include "../Enemy.h"
#include "../../Effect/Shadow.h"
#include "Scene/BehaviourTree/TreeManager.h"

class Collector final : public Enemy
{
public:
	Collector();
	~Collector() = default;

	void Initialize(Player* player, EnemyBulletManager* enemyBulletManager) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	void RetryGame() override;

private:
	//	�r�w�C�r�A�c���[���쐬����
	void CreateBehaviour();

	//	�e��łĂ��Ԃ�
	NodeJudge ShotBulletCondition();
	//	�����e�������ǂ���
	NodeJudge TorpedoBulletCondition();
	//	�����e������
	NodeStatus ShotTorpedoBullet();
	//	�ǔ��e�������ǂ���
	NodeJudge HomingBulletCondition();
	//	�ǔ��e������
	NodeStatus ShotHomingBullet();
	//	���@�_���̒ʏ�e�A��
	NodeStatus RapidFireNormalBullet();
	//	�o���A�𒣂�ׂ����ǂ���
	NodeJudge BarrierCondition();
	//	�o���A�𒣂�
	NodeStatus PutUpBarrier();

private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
	//	�v���C���[�N���X�̃|�C���^
	Player* m_player;
	//	�e�Ǘ��p�N���X�̃|�C���^
	EnemyBulletManager* m_enemyBulletManger;
	//	���f���̉�]
	DirectX::SimpleMath::Quaternion m_quaternion;
	//	�U���𐧌䂷��^�C�}�[
	float m_shotTimer;
	//	�U���\�ɂȂ鎞��
	float m_canShotTime;

	//	�����e�̔��ˉ�
	int m_torpedoCount;
	//	�ʏ�e�̔��ˉ�
	int m_normalCount;

	//	�r�w�C�r�A�c���[
	std::unique_ptr<TreeManager> m_tree;

	//	�e
	std::unique_ptr<Shadow> m_shadow;

};