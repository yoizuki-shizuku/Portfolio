/*/
 *  ファイル名		：	ScaleAnimation.cpp
 *  概要			：	拡縮コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/07
/*/
#include "pch.h"
#include "ScaleAnimation.h"

using namespace DirectX;

#include "Library/Easing.h"

ScaleAnimation::ScaleAnimation(UserInterface* ui,
	DirectX::SimpleMath::Vector2 startScale,
	DirectX::SimpleMath::Vector2 endScale,
	float startTime,
	float endTime,
	std::function<float(float)> easing) :
	UIAnimation(startTime, endTime),
	m_userInterface(ui),
	m_startScale(startScale),
	m_endScale(endScale),
	m_easing(easing)
{
}

void ScaleAnimation::Execute()
{
	//	割合を計算
	float ratio = GetRatio();

	//	割合に対するスケールを算出
	float x = Easing::Lerp(m_startScale.x, m_endScale.x, ratio, m_easing);
	float y = Easing::Lerp(m_startScale.y, m_endScale.y, ratio, m_easing);

	//	適用
	m_userInterface->SetScale(SimpleMath::Vector2(x, y));
}
