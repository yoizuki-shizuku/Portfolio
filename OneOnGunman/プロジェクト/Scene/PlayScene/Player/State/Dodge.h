/*/
 *  ファイル名		：	Dodge.h
 *  概要			：	回避状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/05
/*/
#pragma once

#include "State.h"

class Player;

class Dodge final : public State
{
public:
	Dodge(Player* player, float* timeSpeed);
	~Dodge() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;


private:
	//	スローアニメーションの時間速度制御
	void SlowAnimation();

private:
	//	プレイヤーのクラス
	Player* m_player;
	//	遷移してからの時間を計測
	float m_timer;
	//	ゲーム内時間の経過速度
	float* m_timeSpeed;
	//	スロー演出で使うタイマー
	float m_slowTimer;
	//	ジャスト回避になったかどうか
	bool m_justDodge;
	//	ショット状態へのキャンセル可能時間
	float m_shotCancelTime;
	//	無敵時間
	float m_invincibleTime;

};