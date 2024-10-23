/*/
 *  �t�@�C����		�F	RotationAnimation.h
 *  �T�v			�F	��]�R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/07
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

	//	�J�n�p�x
	float m_startRadian;
	//	�I���p�x
	float m_endRadian;

	//	�C�[�W���O�֐�
	std::function<float(float)> m_easing;

};