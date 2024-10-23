/*/
 *  ファイル名		：	RotationAnimation.h
 *  概要			：	回転コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/07
/*/
#pragma once

#include "../UIAnimation.h"
#include "../UserInterface.h"

class RotationAnimation : public UIAnimation
{
public:
	RotationAnimation(UserInterface* ui,
		float startDegree,
		float endDegree,
		float startTime,
		float endTime,
		std::function<float(float)> easing);
	~RotationAnimation() = default;

	void Execute() override;

private:
	UserInterface* m_userInterface;

	//	開始角度
	float m_startRadian;
	//	終了角度
	float m_endRadian;

	//	イージング関数
	std::function<float(float)> m_easing;

};