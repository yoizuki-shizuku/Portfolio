/*/
 *  ファイル名		：	Run.cpp
 *  概要			：	移動状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#include "pch.h"
#include "Run.h"

using namespace DirectX;

#include "Library/Input.h"

#include "../Player.h"

//	移動速度
#define MOVE_SPEED		(2.5f)

Run::Run(Player* player) :
	State(),
	m_player(player)
{
	//	アニメーションの設定
	SetAnimEndTime(0.7);
	SetAnimLoop(true);
}

void Run::Initialize()
{
}

void Run::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	//	プレイヤーが生存していなければやられ状態にする
	if (!m_player->GetIsAlive())
	{
		m_player->ChangeState(m_player->GetDie());
		return;
	}

	//	マウスの左クリックを押したらショット状態へ
	auto msTracker = Input::GetInstace()->GetMouseTracker();
	if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED && m_player->CanShot())
	{
		m_player->ChangeState(m_player->GetShot());
		return;
	}

	auto kb = Keyboard::Get().GetState();

	//	移動量
	SimpleMath::Vector3 move = SimpleMath::Vector3::Zero;
	//	前方向ベクトル
	SimpleMath::Vector3 front = m_player->GetFront();
	//	横方向ベクトル
	SimpleMath::Vector3 hori = SimpleMath::Vector3(front.z, 0.f, -front.x);

	//	前後
	if (kb.W) { move += front; }
	else if (kb.S) { move -= front; }
	//	左右
	if (kb.A) { move += hori; }
	else if (kb.D) { move -= hori; }

	//	正規化
	move.Normalize();

	//	動いていなければ待機ステートに変更
	if (move == SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetIdle());
		return;
	}

	//	進行方向へ向く角度
	float angle = atan2f(move.x, move.z) + XM_PI;
	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(angle, 0.f, 0.f);

	//	Velocityに適用する
	m_player->SetVelocity(move * MOVE_SPEED);

	//	マウスの右クリックを押されたら回避状態へ
	if (msTracker->rightButton == Mouse::ButtonStateTracker::PRESSED && m_player->CanDodge())
	{
		//	回避方向へ即座に向ける
		m_player->SetAngle(q);

		m_player->ChangeState(m_player->GetDodge());
		return;
	}

	//	徐々にその方向へ向ける
	m_player->SetAngle(SimpleMath::Quaternion::Lerp(m_player->GetAngle(), q, 0.2f));
}

void Run::Finalize()
{
}
