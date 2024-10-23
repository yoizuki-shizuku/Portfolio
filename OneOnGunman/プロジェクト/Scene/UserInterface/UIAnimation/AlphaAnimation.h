/*/
 *  ファイル名		：	AlphaAnimation.h
 *  概要			：	透過コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/07
/*/
#pragma once

#include "../UIAnimation.h"
#include "../UserInterface.h"

class AlphaAnimation : public UIAnimation
{
public:
	AlphaAnimation(UserInterface* ui,
		float startAlpha,
		float endAlpha,
		float startTime,
		float endTime,
		std::function<float(float)> easing);
	~AlphaAnimation() = default;

	void Execute() override;

private:
	UserInterface* m_userInterface;

	//	開始透明度
	float m_startAlpha;
	//	終了透明度
	float m_endAlpha;

	//	イージング関数
	std::function<float(float)> m_easing;

};