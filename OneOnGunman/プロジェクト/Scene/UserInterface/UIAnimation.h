/*/
 *  �t�@�C����		�F	UIAnimation.h
 *  �T�v			�F	UserInterface�Ŏg���R�}���h�p�^�[��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/04
/*/
#pragma once

class UIAnimation
{
public:
	UIAnimation(float startTime, float endTime) :
		m_nowTime(0.f),
		m_startTime(startTime),
		m_endTime(endTime)
	{
	}

	virtual ~UIAnimation() = default;

	virtual void Execute() = 0;

	virtual bool WithinTime(float time) final
	{
		m_nowTime = time;

		//	���݂̏������Ԃ��J�n����I���܂ł̊ԂȂ���s�����悤�ɂ���
		return time >= m_startTime && time <= m_endTime;
	}

	virtual float GetRatio() final
	{
		//	�������v�Z
		return (m_nowTime - m_startTime) / (m_endTime - m_startTime);
	}

private:
	//	���݂̎��s����
	float m_nowTime;
	//	�A�j���[�V�������n�܂鎞��
	float m_startTime;
	//	�A�j���[�V�������I�����鎞��
	float m_endTime;

};
