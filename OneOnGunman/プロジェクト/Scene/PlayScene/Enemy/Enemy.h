/*/
 *  ファイル名		：	Enemy.h
 *  概要			：	敵のクラスのインターフェース
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/18
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

	//	リトライ時のフラグ初期化
	virtual void RetryGame() = 0;

	virtual void SetPosition(DirectX::SimpleMath::Vector3 pos);
	virtual void SetOffset(DirectX::SimpleMath::Vector3 offset) { m_offset = offset; }
	virtual void SetCollider(Sphere* col) { m_collider.reset(col); }

	virtual DirectX::SimpleMath::Vector3 GetPosition() const final { return m_position; }
	virtual Collider* GetCollider() const final { return m_collider.get(); }
	virtual Observer* GetObserver() const final { return m_observer.get(); }

private:
	//	位置
	DirectX::SimpleMath::Vector3 m_position;
	//	位置とモデルの見た目による当たり判定の差異
	DirectX::SimpleMath::Vector3 m_offset;
	//	当たり判定
	std::unique_ptr<Sphere> m_collider;
	//	撃破イベント検知用オブザーバー
	std::unique_ptr<Observer> m_observer;

};