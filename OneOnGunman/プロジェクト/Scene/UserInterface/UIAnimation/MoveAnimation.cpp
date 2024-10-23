/*/
 *  �t�@�C����		�F	MoveAnimation.cpp
 *  �T�v			�F	�ړ��R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/04
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
	//	�������v�Z
	float ratio = GetRatio();

	//	�����ɑ΂���ʒu���Z�o
	float x = Easing::Lerp(m_startPosition.x, m_endPosition.x, ratio, m_easing);
	float y = Easing::Lerp(m_startPosition.y, m_endPosition.y, ratio, m_easing);

	//	�K�p
	m_userInterface->SetPosition(SimpleMath::Vector2(x, y));
}
