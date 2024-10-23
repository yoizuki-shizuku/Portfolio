/*/
 *  ファイル名		：	Dodge.cpp
 *  概要			：	回避状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/05
/*/
#include "pch.h"
#include "Dodge.h"

using namespace DirectX;

#include "../Player.h"
#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

//	回避時の速度
#define DODGE_SPEED			(9.f)
//	減速係数
#define FRICTION			(5.5f)

//	待機状態へ移行する基準
#define IDLE_BASE			(1.2f)

//	射撃状態へのキャンセル基準
#define SHOT_BASE			(0.8f)
//	射撃状態へのキャンセル基準(ジャスト回避時)
#define JUST_SHOT_BASE		(0.4f)

//	無敵時間
#define INVINCIBLE			(0.8f)
//	ジャスト回避時の無敵時間
#define JUST_INVINCIBLE		(1.1f)

//	ジャスト回避時のクールタイムへのボーナス
#define JUST_DODGE_BONUS	(2.f)

//	スローアニメーション用定数
#define RELEASE_SLOW		(0.6f)

Dodge::Dodge(Player* player, float* timeSpeed) :
	State(),
	m_player(player),
	m_timer(0.f),
	m_timeSpeed(timeSpeed),
	m_slowTimer(0.f),
	m_justDodge(false),
	m_shotCancelTime(SHOT_BASE),
	m_invincibleTime(INVINCIBLE)
{
	//	アニメーションの設定
	SetAnimSpeed(1.6f);
	SetAnimStartTime(0.2);
	SetAnimLoop(false);
}

void Dodge::Initialize()
{
	//	アニメーションをはじめからにする
	RestartAnimetion();

	//	タイマーをリセット
	m_timer = 0.f;
	m_slowTimer = 0.f;

	//	移動している方向へ回避する
	m_player->SetVelocity(m_player->GetVelocity() * DODGE_SPEED);

	//	無敵状態にする
	m_player->SetColliderValid(false);

	//	クールタイムをリセット
	m_player->ResetDodgeCoolTime();

	//	ジャスト回避ならば時間を遅くする演出を開始
	m_justDodge = m_player->GetJustDodge()->GetIsHit();

	//	ジャスト回避時だった際に必要な処理
	if (m_justDodge)
	{
		//	ゲージにボーナスを付与
		m_player->ShorteningShotCoolTime(JUST_DODGE_BONUS);

		//	キャンセルや無敵時間の設定
		m_shotCancelTime = JUST_SHOT_BASE;
		m_invincibleTime = JUST_INVINCIBLE;
	}
	else
	{
		m_shotCancelTime = SHOT_BASE;
		m_invincibleTime = INVINCIBLE;
	}
}

void Dodge::Update(float elapsedTime)
{
	//	プレイヤーが生存していなければやられ状態にする
	if (!m_player->GetIsAlive())
	{
		m_player->ChangeState(m_player->GetDie());
		return;
	}

	//	タイマー更新
	m_timer += elapsedTime;

	//	スローモーション演出
	SlowAnimation();

	//	速度の減少率を時間準拠に
	//	毎秒徐々に減速する
	SimpleMath::Vector3 friction = m_player->GetVelocity() * FRICTION * elapsedTime;
	m_player->SetVelocity(m_player->GetVelocity() - friction);

	//	一定時間経過後にショット状態へキャンセル可能
	if (m_timer >= m_shotCancelTime)
	{
		//	マウスの左クリックを押したらショット状態へキャンセルする
		auto msTracker = Input::GetInstace()->GetMouseTracker();
		if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED && m_player->CanShot())
		{
			*m_timeSpeed = 1.f;
			m_player->SetColliderValid(true);
			m_player->ChangeState(m_player->GetShot());
		}
	}

	//	一定時間後に無敵を解除
	if (m_timer >= m_invincibleTime)
	{
		m_player->SetColliderValid(true);
	}

	//	一定時間経てば待機状態へ
	if (m_timer < IDLE_BASE) return;

	m_player->ChangeState(m_player->GetIdle());
}

void Dodge::Finalize()
{
}

void Dodge::SlowAnimation()
{
	//	ジャスト回避していなければ処理しない
	if (!m_justDodge) return;

	//	タイマー更新(現実時間準拠)
	m_slowTimer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	実際のアニメーション部分
	if (m_slowTimer < RELEASE_SLOW)
	{
		*m_timeSpeed = Easing::Lerp(0.05f, 1.f, 
			Useful::Ratio01(m_slowTimer, RELEASE_SLOW), Easing::InQuint);
	}
	else
	{
		*m_timeSpeed = 1.f;
	}
}
