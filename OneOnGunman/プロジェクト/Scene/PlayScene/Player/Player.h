/*/
 *  �t�@�C����		�F	Player.h
 *  �T�v			�F	�v���C���[�𐧌䂷��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/09
/*/
#pragma once

#include "State/State.h"
#include "PlayerBullet.h"
#include "Library/Collider.h"
#include "../Effect/Shadow.h"
#include "Scene/Observer/Observer.h"
#include "../Effect/ChargeEffect.h"

class Player
{
public:
	Player();
	~Player() = default;

	void Initialize(PlayerBullet* playerBullet, float* timeSpeed);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	//	�e������
	void ShotBullet();
	//	�V���b�g�̃N�[���^�C�����I����Ă��邩
	bool CanShot();
	//	�V���b�g�̃N�[���^�C���ݒ�
	void ResetShotCoolTime() { m_shotCoolTime = 0.f; }
	//	�V���b�g�̃N�[���^�C���Z�k
	void ShorteningShotCoolTime(const float shortening) { m_shotCoolTime += shortening; }
	//	�V���b�g�̃N�[���^�C������
	float GetShotRatio() const;
	//	����̃N�[���^�C�����I����Ă��邩
	bool CanDodge();
	//	�����̃N�[���^�C���ݒ�
	void ResetDodgeCoolTime() { m_dodgeCoolTime = 0.f; }
	//	����̃N�[���^�C������
	float GetDodgeRatio() const;
	//	���f���̒��S�ʒu
	DirectX::SimpleMath::Vector3 GetCenter();

	//	���g���C���̃t���O������
	void RetryGame();

public:
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetFront()    const { return m_front; }
	DirectX::SimpleMath::Quaternion GetAngle() const { return m_angle; }
	Collider* GetCollider()                    const { return m_collider.get(); }
	Collider* GetJustDodge()                   const { return m_justDodge.get(); }
	bool GetIsAlive()                          const { return m_isAlive; }
	int GetShotNum()                           const { return m_shotNum; }
	Observer* GetFailureObserver()                   { return m_failureObs.get(); }

	void GetViewProj(DirectX::SimpleMath::Matrix* view, DirectX::SimpleMath::Matrix* proj) const;

	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }
	void SetVelocity(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }
	void SetFront(DirectX::SimpleMath::Vector3 front)		{ m_front = front; }
	void SetAngle(DirectX::SimpleMath::Quaternion angle)	{ m_angle = angle; }
	void SetColliderValid(const bool& flag)					{ m_collider->SetIsValid(flag); }
	void SetIsAlive(const bool& flag)	                    { m_isAlive = flag; }

	//	�ҋ@��Ԃ��擾����
	State* GetIdle() { return m_idle.get(); }
	//	�ړ���Ԃ��擾����
	State* GetRun() { return m_run.get(); }
	//	�V���b�g��Ԃ��擾����
	State* GetShot() { return m_shot.get(); }
	//	�����Ԃ��擾����
	State* GetDodge() { return m_dodge.get(); }
	//	�����Ԃ��擾����
	State* GetDie() { return m_die.get(); }

	//	�X�e�[�g��ύX����
	void ChangeState(State* changeState);

private:
	//	�v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_position;
	//	�v���C���[�̈ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_velocity;
	//	�J�������猩�����ʃx�N�g��(�ړ�����)
	DirectX::SimpleMath::Vector3 m_front;
	//	�v���C���[�̊p�x
	DirectX::SimpleMath::Quaternion m_angle;
	//	�����蔻��
	std::unique_ptr<Capsule> m_collider;
	//	�W���X�g����̔���
	std::unique_ptr<Sphere> m_justDodge;
	//	�v���C���[�̒e
	PlayerBullet* m_playerBullet;
	//	�V���b�g�̃N�[���^�C���v��
	float m_shotCoolTime;
	//	����̃N�[���^�C���v��
	float m_dodgeCoolTime;
	//	�������Ă��邩
	bool m_isAlive;
	//	�e����������
	int m_shotNum;

	//	�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//	�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	//	���f��
	std::unique_ptr<DirectX::Model> m_model;
	//	���݂̃X�e�[�g
	State* m_currentState;
	//	�ҋ@���
	std::unique_ptr<State> m_idle;
	//	�ړ����
	std::unique_ptr<State> m_run;
	//	�V���b�g���
	std::unique_ptr<State> m_shot;
	//	������
	std::unique_ptr<State> m_dodge;
	//	������
	std::unique_ptr<State> m_die;

	//	�e
	std::unique_ptr<Shadow> m_shadow;
	//	���ꂽ�ʒm
	std::unique_ptr<Observer> m_failureObs;
	//	�`���[�W�����̃G�t�F�N�g
	std::unique_ptr<ChargeEffect> m_chargeEffect;

};