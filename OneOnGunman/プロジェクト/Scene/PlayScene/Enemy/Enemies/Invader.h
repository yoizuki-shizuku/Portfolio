/*/
 *  �t�@�C����		�F	Invader.h
 *  �T�v			�F	�X�e�[�W3�̓G
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/23
/*/
#pragma once

#include "../Enemy.h"
#include "../../Effect/Shadow.h"
#include "Scene/BehaviourTree/TreeManager.h"

class Invader final : public Enemy
{
public:
	Invader();
	~Invader() = default;

	void Initialize(Player* player, EnemyBulletManager* enemyBulletManager) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	void RetryGame() override;

private:
	//	�r�w�C�r�A�c���[���쐬����
	void CreateBehaviour();

	//	�ړ��\�͈͓����ǂ���
	NodeJudge CheckMoveRange();
	//	�ړ�
	NodeStatus NormalMove();
	//	�͈͓��֕␳����
	NodeStatus CorrectRange();
	//	����łĂ��Ԃ�
	NodeJudge ShotBulletCondition();
	//	�E�F�[�u�e�������ǂ���
	NodeJudge WaveBulletCondition();
	//	�E�F�[�u�e������
	NodeStatus ShotWaveBullet();


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
	//	�e�m�[�h�֎󂯓n���p��elapsedTime
	float m_elapsedTime;
	//	�ړ����������炩�ɂ��邽�߂̊p�x
	DirectX::SimpleMath::Quaternion m_angle;
	//	�ړ�����
	DirectX::SimpleMath::Vector3 m_velocity;

	//	�r�w�C�r�A�c���[
	std::unique_ptr<TreeManager> m_tree;

	//	�e
	std::unique_ptr<Shadow> m_shadow;

};