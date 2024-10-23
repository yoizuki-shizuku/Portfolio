/*/
 *  ファイル名		：	Guardian.h
 *  概要			：	ステージ1の敵
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
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
	//	ビヘイビアツリーを作成する
	void CreateBehaviour();

	//	移動先を決めるべき状態か
	NodeJudge DestinationCondition();
	//	移動先を設定する
	NodeStatus Destination();
	//	移動用タイマーを設定する
	NodeStatus SettingMoveTimer();
	//	移動
	NodeStatus Move();
	//	球を打てる状態か
	NodeJudge ShotBulletCondition();
	//	通常弾を撃つかどうか
	NodeJudge NormalBulletCondition();
	//	通常弾を撃つ
	NodeJudge ShotNormalBullet();
	//	3Way弾を撃つかどうか
	NodeJudge ThreeWayCondition();
	//	3Way弾を撃つ
	NodeStatus ShotThreeWay();
	//	ランダム方向に弾を撃つ
	NodeStatus ShotRandomBullet();
	//	攻撃のクールタイム設定
	NodeStatus SettingShotTimer();


private:
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
	//	プレイヤークラスのポインタ
	Player* m_player;
	//	弾管理用クラスのポインタ
	EnemyBulletManager* m_enemyBulletManger;
	//	モデルの回転
	DirectX::SimpleMath::Quaternion m_quaternion;
	//	次の行き先
	DirectX::SimpleMath::Vector3 m_nextPos;

	//	ゲーム内の1fに経過する時間
	float m_elapsedTime;
	
	//	移動を制御するタイマー
	float m_moveTimer;
	//	攻撃を制御するタイマー
	float m_shotTimer;
	//	移動可能になる時間
	float m_canMoveTime;
	//	攻撃可能になる時間
	float m_canShotTime;
	//	3段階目の移動回数カウント
	int m_moveCount;

	//	ビヘイビアツリー
	std::unique_ptr<TreeManager> m_tree;

	//	影
	std::unique_ptr<Shadow> m_shadow;

};