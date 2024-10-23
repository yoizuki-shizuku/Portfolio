/*/
 *  ファイル名		：	AlphaAnimation.cpp
 *  概要			：	透過コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/07
/*/
#include "pch.h"
#include "AlphaAnimation.h"

using namespace DirectX;

#include "Library/Easing.h"

AlphaAnimation::AlphaAnimation(UserInterface* ui,
	float startAlpha,
	float endAlpha,
	float startTime,
	float endTime,
	std::function<float(float)> easing) :
	UIAnimation(startTime, endTime),
	m_userInterface(ui),
	m_startAlpha(startAlpha),
	m_endAlpha(endAlpha),
	m_easing(easing)
{
}

void AlphaAnimation::Execute()
{
	//	割合を計算
	float ratio = GetRatio();

	//	割合に対する角度を算出
	float radian = Easing::Lerp(m_startAlpha, m_endAlpha, ratio, m_easing);

	//	適用
	m_userInterface->SetAlpha(radian);
}
