/*/
 *  �t�@�C����		�F	RotationAnimation.cpp
 *  �T�v			�F	��]�R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/07
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
	//	�t�@�C���ȂǂŎw�肷��ۂɕ�����h���̂�
	//	�����Ń��W�A���ɕϊ�
	m_startRadian = XMConvertToRadians(startDegree);
	m_endRadian = XMConvertToRadians(endDegree);
}

void RotationAnimation::Execute()
{
	//	�������v�Z
	float ratio = GetRatio();

	//	�����ɑ΂���p�x���Z�o
	float radian = Easing::Lerp(m_startRadian, m_endRadian, ratio, m_easing);

	//	�K�p
	m_userInterface->SetRadian(radian);
}
