/*/
 *  �t�@�C����		�F	Guardian.h
 *  �T�v			�F	�X�e�[�W1�̓G
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#pragma once

#include "../Enemy.h"
#include "../../Effect/Shadow.h"
#include "Scene/BehaviourTree/TreeManager.h"

class Guardian final : public Enemy
{
public:
	Guardian();
	~Guardian() = default;

	void Initialize(Player* player, EnemyBulletManager* enemyBulletManager) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	void RetryGame() override;

private:
	//	�r�w�C�r�A�c���[���쐬����
	void CreateBehaviour();

	//	�ړ�������߂�ׂ���Ԃ�
	NodeJudge DestinationCondition();
	//	�ړ����ݒ肷��
	NodeStatus Destination();
	//	�ړ��p�^�C�}�[��ݒ肷��
	NodeStatus SettingMoveTimer();
	//	�ړ�
	NodeStatus Move();
	//	����łĂ��Ԃ�
	NodeJudge ShotBulletCondition();
	//	�ʏ�e�������ǂ���
	NodeJudge NormalBulletCondition();
	//	�ʏ�e������
	NodeJudge ShotNormalBullet();
	//	3Way�e�������ǂ���
	NodeJudge ThreeWayCondition();
	//	3Way�e������
	NodeStatus ShotThreeWay();
	//	�����_�������ɒe������
	NodeStatus ShotRandomBullet();
	//	�U���̃N�[���^�C���ݒ�
	NodeStatus SettingShotTimer();


private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
	//	�v���C���[�N���X�̃|�C���^
	Player* m_player;
	//	�e�Ǘ��p�N���X�̃|�C���^
	EnemyBulletManager* m_enemyBulletManger;
	//	���f���̉�]
	DirectX::SimpleMath::Quaternion m_quaternion;
	//	���̍s����
	DirectX::SimpleMath::Vector3 m_nextPos;

	//	�Q�[������1f�Ɍo�߂��鎞��
	float m_elapsedTime;
	
	//	�ړ��𐧌䂷��^�C�}�[
	float m_moveTimer;
	//	�U���𐧌䂷��^�C�}�[
	float m_shotTimer;
	//	�ړ��\�ɂȂ鎞��
	float m_canMoveTime;
	//	�U���\�ɂȂ鎞��
	float m_canShotTime;
	//	3�i�K�ڂ̈ړ��񐔃J�E���g
	int m_moveCount;

	//	�r�w�C�r�A�c���[
	std::unique_ptr<TreeManager> m_tree;

	//	�e
	std::unique_ptr<Shadow> m_shadow;

};