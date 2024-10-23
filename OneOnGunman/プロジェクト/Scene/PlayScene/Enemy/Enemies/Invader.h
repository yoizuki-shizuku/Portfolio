/*/
 *  ファイル名		：	Invader.h
 *  概要			：	ステージ3の敵
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/23
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
	//	ビヘイビアツリーを作成する
	void CreateBehaviour();

	//	移動可能範囲内かどうか
	NodeJudge CheckMoveRange();
	//	移動
	NodeStatus NormalMove();
	//	範囲内へ補正する
	NodeStatus CorrectRange();
	//	球を打てる状態か
	NodeJudge ShotBulletCondition();
	//	ウェーブ弾を撃つかどうか
	NodeJudge WaveBulletCondition();
	//	ウェーブ弾を撃つ
	NodeStatus ShotWaveBullet();


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
	//	各ノードへ受け渡し用のelapsedTime
	float m_elapsedTime;
	//	移動方向を滑らかにするための角度
	DirectX::SimpleMath::Quaternion m_angle;
	//	移動方向
	DirectX::SimpleMath::Vector3 m_velocity;

	//	ビヘイビアツリー
	std::unique_ptr<TreeManager> m_tree;

	//	影
	std::unique_ptr<Shadow> m_shadow;

};