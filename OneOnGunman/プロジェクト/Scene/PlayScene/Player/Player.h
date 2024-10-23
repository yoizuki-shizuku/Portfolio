/*/
 *  ファイル名		：	Player.h
 *  概要			：	プレイヤーを制御する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/09
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

	//	弾を撃つ
	void ShotBullet();
	//	ショットのクールタイムが終わっているか
	bool CanShot();
	//	ショットのクールタイム設定
	void ResetShotCoolTime() { m_shotCoolTime = 0.f; }
	//	ショットのクールタイム短縮
	void ShorteningShotCoolTime(const float shortening) { m_shotCoolTime += shortening; }
	//	ショットのクールタイム割合
	float GetShotRatio() const;
	//	回避のクールタイムが終わっているか
	bool CanDodge();
	//	回避後のクールタイム設定
	void ResetDodgeCoolTime() { m_dodgeCoolTime = 0.f; }
	//	回避のクールタイム割合
	float GetDodgeRatio() const;
	//	モデルの中心位置
	DirectX::SimpleMath::Vector3 GetCenter();

	//	リトライ時のフラグ初期化
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

	//	待機状態を取得する
	State* GetIdle() { return m_idle.get(); }
	//	移動状態を取得する
	State* GetRun() { return m_run.get(); }
	//	ショット状態を取得する
	State* GetShot() { return m_shot.get(); }
	//	回避状態を取得する
	State* GetDodge() { return m_dodge.get(); }
	//	やられ状態を取得する
	State* GetDie() { return m_die.get(); }

	//	ステートを変更する
	void ChangeState(State* changeState);

private:
	//	プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_position;
	//	プレイヤーの移動ベクトル
	DirectX::SimpleMath::Vector3 m_velocity;
	//	カメラから見た正面ベクトル(移動方向)
	DirectX::SimpleMath::Vector3 m_front;
	//	プレイヤーの角度
	DirectX::SimpleMath::Quaternion m_angle;
	//	当たり判定
	std::unique_ptr<Capsule> m_collider;
	//	ジャスト回避の判定
	std::unique_ptr<Sphere> m_justDodge;
	//	プレイヤーの弾
	PlayerBullet* m_playerBullet;
	//	ショットのクールタイム計測
	float m_shotCoolTime;
	//	回避のクールタイム計測
	float m_dodgeCoolTime;
	//	生存しているか
	bool m_isAlive;
	//	弾を撃った回数
	int m_shotNum;

	//	ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//	射影行列
	DirectX::SimpleMath::Matrix m_proj;

	//	モデル
	std::unique_ptr<DirectX::Model> m_model;
	//	現在のステート
	State* m_currentState;
	//	待機状態
	std::unique_ptr<State> m_idle;
	//	移動状態
	std::unique_ptr<State> m_run;
	//	ショット状態
	std::unique_ptr<State> m_shot;
	//	回避状態
	std::unique_ptr<State> m_dodge;
	//	やられ状態
	std::unique_ptr<State> m_die;

	//	影
	std::unique_ptr<Shadow> m_shadow;
	//	やられた通知
	std::unique_ptr<Observer> m_failureObs;
	//	チャージ完了のエフェクト
	std::unique_ptr<ChargeEffect> m_chargeEffect;

};