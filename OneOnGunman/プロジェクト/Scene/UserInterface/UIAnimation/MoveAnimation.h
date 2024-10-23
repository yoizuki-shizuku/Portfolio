/*/
 *  ファイル名		：	MoveAnimation.h
 *  概要			：	移動コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
/*/
#pragma once

#include "../UIAnimation.h"
#include "../UserInterface.h"

class MoveAnimation : public UIAnimation
{
public:
	MoveAnimation(UserInterface* ui,
		DirectX::SimpleMath::Vector2 startPos,
		DirectX::SimpleMath::Vector2 endPos,
		float startTime,
		float endTime,
		std::function<float(float)> easing);
	~MoveAnimation() = default;

	void Execute() override;

private:
	UserInterface* m_userInterface;

	//	開始位置
	DirectX::SimpleMath::Vector2 m_startPosition;
	//	終了位置
	DirectX::SimpleMath::Vector2 m_endPosition;

	//	イージング関数
	std::function<float(float)> m_easing;

};