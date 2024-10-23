/*/
 *  ファイル名		：	Die.cpp
 *  概要			：	やられ状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/06
/*/
#include "pch.h"
#include "Die.h"

using namespace DirectX;

#include "../Player.h"
#include "Library/Input.h"

Die::Die(Player* player) :
	State(),
	m_player(player)
{
	//	アニメーションの設定
	SetAnimLoop(false);
	SetAnimEndTime(2.0);
}

void Die::Initialize()
{
	//	アニメーションをはじめからにする
	RestartAnimetion();

	//	速度を0に
	m_player->SetVelocity(SimpleMath::Vector3::Zero);

	//	当たり判定無効化
	m_player->GetCollider()->SetIsValid(false);
}

void Die::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void Die::Finalize()
{
}
