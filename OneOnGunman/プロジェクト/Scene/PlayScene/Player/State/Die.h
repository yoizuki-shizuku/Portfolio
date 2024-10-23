/*/
 *  ファイル名		：	Die.h
 *  概要			：	やられ状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/06
/*/
#pragma once

#include "State.h"

class Player;

class Die : public State
{
public:
	Die(Player* player);
	~Die() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	プレイヤーのクラス
	Player* m_player;

};