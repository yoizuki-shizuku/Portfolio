/*/
 *  ファイル名		：	Collector.h
 *  概要			：	ステージ2の敵
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/23
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
	//	ビヘイビアツリーを作成する
	void CreateBehaviour();

	//	弾を打てる状態か
	NodeJudge ShotBulletCondition();
	//	魚雷弾を撃つかどうか
	NodeJudge TorpedoBulletCondition();
	//	魚雷弾を撃つ
	NodeStatus ShotTorpedoBullet();
	//	追尾弾を撃つかどうか
	NodeJudge HomingBulletCondition();
	//	追尾弾を撃つ
	NodeStatus ShotHomingBullet();
	//	自機狙いの通常弾連射
	NodeStatus RapidFireNormalBullet();
	//	バリアを張るべきかどうか
	NodeJudge BarrierCondition();
	//	バリアを張る
	NodeStatus PutUpBarrier();

private:
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
	//	プレイヤークラスのポインタ
	Player* m_player;
	//	弾管理用クラスのポインタ
	EnemyBulletManager* m_enemyBulletManger;
	//	モデルの回転
	DirectX::SimpleMath::Quaternion m_quaternion;
	//	攻撃を制御するタイマー
	float m_shotTimer;
	//	攻撃可能になる時間
	float m_canShotTime;

	//	魚雷弾の発射回数
	int m_torpedoCount;
	//	通常弾の発射回数
	int m_normalCount;

	//	ビヘイビアツリー
	std::unique_ptr<TreeManager> m_tree;

	//	影
	std::unique_ptr<Shadow> m_shadow;

};