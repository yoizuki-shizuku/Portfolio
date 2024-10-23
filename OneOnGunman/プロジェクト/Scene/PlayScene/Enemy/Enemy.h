/*/
 *  �t�@�C����		�F	Enemy.h
 *  �T�v			�F	�G�̃N���X�̃C���^�[�t�F�[�X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/18
/*/
#pragma once

#include "../Player/Player.h"
#include "EnemyBulletManager.h"
#include "Library/Collider.h"
#include "Scene/Observer/Observer.h"

class Enemy
{
public:
	Enemy();
	virtual ~Enemy() = default;

	virtual void Initialize(Player* player, EnemyBulletManager* enemyBulletManager) = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void Finalize() = 0;

	//	���g���C���̃t���O������
	virtual void RetryGame() = 0;

	virtual void SetPosition(DirectX::SimpleMath::Vector3 pos);
	virtual void SetOffset(DirectX::SimpleMath::Vector3 offset) { m_offset = offset; }
	virtual void SetCollider(Sphere* col) { m_collider.reset(col); }

	virtual DirectX::SimpleMath::Vector3 GetPosition() const final { return m_position; }
	virtual Collider* GetCollider() const final { return m_collider.get(); }
	virtual Observer* GetObserver() const final { return m_observer.get(); }

private:
	//	�ʒu
	DirectX::SimpleMath::Vector3 m_position;
	//	�ʒu�ƃ��f���̌����ڂɂ�铖���蔻��̍���
	DirectX::SimpleMath::Vector3 m_offset;
	//	�����蔻��
	std::unique_ptr<Sphere> m_collider;
	//	���j�C�x���g���m�p�I�u�U�[�o�[
	std::unique_ptr<Observer> m_observer;

};