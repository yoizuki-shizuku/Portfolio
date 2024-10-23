/*/
 *  ファイル名		：	Run.h
 *  概要			：	移動状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#pragma once

#include "State.h"

class Player;

class Run final : public State
{
public:
	Run(Player* player);
	~Run() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	プレイヤークラス
	Player* m_player;

};