/*/
 *  �t�@�C����		�F	AlphaAnimation.cpp
 *  �T�v			�F	���߃R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/07
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
	//	�������v�Z
	float ratio = GetRatio();

	//	�����ɑ΂���p�x���Z�o
	float radian = Easing::Lerp(m_startAlpha, m_endAlpha, ratio, m_easing);

	//	�K�p
	m_userInterface->SetAlpha(radian);
}
