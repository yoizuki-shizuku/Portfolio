/*/
 *  ファイル名		：	MoveAnimation.cpp
 *  概要			：	移動コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
/*/
#include "pch.h"
#include "MoveAnimation.h"

using namespace DirectX;

#include "Library/Easing.h"

MoveAnimation::MoveAnimation(UserInterface* ui,
	DirectX::SimpleMath::Vector2 startPos,
	DirectX::SimpleMath::Vector2 endPos,
	float startTime,
	float endTime,
	std::function<float(float)> easing) :
	UIAnimation(startTime, endTime),
	m_userInterface(ui),
	m_startPosition(startPos),
	m_endPosition(endPos),
	m_easing(easing)
{
}

void MoveAnimation::Execute()
{
	//	割合を計算
	float ratio = GetRatio();

	//	割合に対する位置を算出
	float x = Easing::Lerp(m_startPosition.x, m_endPosition.x, ratio, m_easing);
	float y = Easing::Lerp(m_startPosition.y, m_endPosition.y, ratio, m_easing);

	//	適用
	m_userInterface->SetPosition(SimpleMath::Vector2(x, y));
}
