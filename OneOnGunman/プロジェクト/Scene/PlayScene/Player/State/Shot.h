/*/
 *  ファイル名		：	Shot.h
 *  概要			：	ショット状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#pragma once

#include "State.h"

class Player;

class Shot final : public State
{
public:
	Shot(Player* player);
	~Shot() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	スクリーン座標をワールド座標に変換
	DirectX::SimpleMath::Vector3 CalcScreenToWorld(
		int screen_x, int screen_y,
		float fZ,
		int screen_w, int screen_h,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj
	);

	//	XZ平面とスクリーン座標の交点算出関数
	DirectX::SimpleMath::Vector3 CalcScreenToXZ(
		int screen_x, int screen_y,
		int screen_w, int screen_h,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj
	);

private:
	//	プレイヤーのクラス
	Player* m_player;
	//	遷移してからの時間を計測
	float m_timer;

};