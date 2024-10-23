/*/
 *  �t�@�C����		�F	MoveAnimation.h
 *  �T�v			�F	�ړ��R�}���h
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/04
/*/
#pragma once

#include "../UIAnimation.h"
#include "../UserInterface.h"

class MoveAnimation : public UIAnimation
{
public:
	MoveAnimation(UserInterface* ui,
		DirectX::SimpleMath::Vector2 startPos,
		DirectX::SimpleMath::Vector2 endPos,
		float startTime,
		float endTime,
		std::function<float(float)> easing);
	~MoveAnimation() = default;

	void Execute() override;

private:
	UserInterface* m_userInterface;

	//	�J�n�ʒu
	DirectX::SimpleMath::Vector2 m_startPosition;
	//	�I���ʒu
	DirectX::SimpleMath::Vector2 m_endPosition;

	//	�C�[�W���O�֐�
	std::function<float(float)> m_easing;

};