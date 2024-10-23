/*/
 *  �t�@�C����		�F	ScaleAnimation.cpp
 *  �T�v			�F	�g�k�R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/07
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
	//	�������v�Z
	float ratio = GetRatio();

	//	�����ɑ΂���X�P�[�����Z�o
	float x = Easing::Lerp(m_startScale.x, m_endScale.x, ratio, m_easing);
	float y = Easing::Lerp(m_startScale.y, m_endScale.y, ratio, m_easing);

	//	�K�p
	m_userInterface->SetScale(SimpleMath::Vector2(x, y));
}
