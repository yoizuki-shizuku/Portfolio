/*/
 *  �t�@�C����		�F	ScaleAnimation.h
 *  �T�v			�F	�g�k�R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/07
/*/
#pragma once

#include "../UIAnimation.h"
#include "../UserInterface.h"

class ScaleAnimation : public UIAnimation
{
public:
	ScaleAnimation(UserInterface* ui,
		DirectX::SimpleMath::Vector2 startScale,
		DirectX::SimpleMath::Vector2 endScale,
		float startTime,
		float endTime,
		std::function<float(float)> easing);
	~ScaleAnimation() = default;

	void Execute() override;

private:
	UserInterface* m_userInterface;

	//	�J�n�T�C�Y
	DirectX::SimpleMath::Vector2 m_startScale;
	//	�I���T�C�Y
	DirectX::SimpleMath::Vector2 m_endScale;

	//	�C�[�W���O�֐�
	std::function<float(float)> m_easing;

};