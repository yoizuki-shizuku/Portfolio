/*/
 *  ファイル名		：	Idle.cpp
 *  概要			：	待機状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/21
/*/
#include "pch.h"
#include "Idle.h"

using namespace DirectX;

#include "Library/Input.h"

#include "../Player.h"

Idle::Idle(Player* player) :
	State(),
	m_player(player)
{
	//	アニメーションの設定
	SetAnimEndTime(1.5);
	SetAnimLoop(true);
}

void Idle::Initialize()
{
	m_player->SetVelocity(SimpleMath::Vector3::Zero);
}

void Idle::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	auto kb = Keyboard::Get().GetState();

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
	}

	//	WASDが押されたら移動状態にする
	if (kb.W || kb.A || kb.S || kb.D)
	{
		m_player->ChangeState(m_player->GetRun());
	}
}

void Idle::Finalize()
{
}