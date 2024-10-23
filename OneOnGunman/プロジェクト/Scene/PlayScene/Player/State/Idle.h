/*/
 *  ファイル名		：	Idle.h
 *  概要			：	待機状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/21
/*/
#pragma once

#include "State.h"

class Player;

class Idle final : public State
{
public:
	Idle(Player* player);
	~Idle() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	プレイヤーのクラス
	Player* m_player;

};