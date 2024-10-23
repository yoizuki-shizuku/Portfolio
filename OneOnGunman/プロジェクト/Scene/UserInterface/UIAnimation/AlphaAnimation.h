/*/
 *  �t�@�C����		�F	AlphaAnimation.h
 *  �T�v			�F	���߃R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/07
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

	//	�J�n�����x
	float m_startAlpha;
	//	�I�������x
	float m_endAlpha;

	//	�C�[�W���O�֐�
	std::function<float(float)> m_easing;

};