/*/
 *  ファイル名		：	RotationAnimation.cpp
 *  概要			：	回転コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/07
/*/
#include "pch.h"
#include "RotationAnimation.h"

using namespace DirectX;

#include "Library/Easing.h"

RotationAnimation::RotationAnimation(UserInterface* ui,
	float startDegree,
	float endDegree,
	float startTime,
	float endTime,
	std::function<float(float)> easing) :
	UIAnimation(startTime, endTime),
	m_userInterface(ui),
	m_easing(easing)
{
	//	ファイルなどで指定する際に分かり辛いので
	//	内部でラジアンに変換
	m_startRadian = XMConvertToRadians(startDegree);
	m_endRadian = XMConvertToRadians(endDegree);
}

void RotationAnimation::Execute()
{
	//	割合を計算
	float ratio = GetRatio();

	//	割合に対する角度を算出
	float radian = Easing::Lerp(m_startRadian, m_endRadian, ratio, m_easing);

	//	適用
	m_userInterface->SetRadian(radian);
}
