/*/
 *  �t�@�C����		�F	FinishEvent.h
 *  �T�v			�F	�Q�[���I�����̉��o
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/02
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include "Scene/Observer/Observer.h"
#include <array>

class FinishEvent
{
public:
	enum class Process
	{
		MAINTAIN = -1,
		RETRY    = 0,
		SELECT   = 1,
		CLEAR    = 2
	};

public:
	FinishEvent();
	~FinishEvent() = default;

	void Initialize();
	Process Update();
	void Render();

	//	���g���C���̃t���O������
	void RetryGame();

public:
	Observer* GetClearObserver() { return m_clearObs.get(); }
	Observer* GetFailureObserver() { return m_failureObs.get(); }

private:
	//	�N���A
	std::unique_ptr<UserInterface> m_clear;
	//	���s
	std::unique_ptr<UserInterface> m_failure;

	//	���g���C�{�^��
	std::unique_ptr<UserInterface> m_retry;
	//	�Z���N�g�֖߂�{�^��
	std::unique_ptr<UserInterface> m_select;

	//	���^�[�{�b�N�X
	std::unique_ptr<UserInterface> m_letterBox_1;
	std::unique_ptr<UserInterface> m_letterBox_2;

	//	�N���A�A�j���[�V����
	std::unique_ptr<Animator> m_clearAnim;
	//	���s�A�j���[�V����
	std::unique_ptr<Animator> m_failureAnim;

	//	�t�F�[�h�C���A�j���[�V����
	std::array<std::unique_ptr<Animator>, 2> m_FadeIn;
	//	�I�����A�j���[�V����
	std::array<std::unique_ptr<Animator>, 2> m_choise;
	//	�{�^��
	std::array<std::unique_ptr<Button>, 2> m_button;

	//	�N���A���̃C�x���g���Ď�
	std::unique_ptr<Observer> m_clearObs;
	//	���s���̃C�x���g���Ď�
	std::unique_ptr<Observer> m_failureObs;

	//	���^�[�{�b�N�X�̃A�j���[�V�����^�C�}�[
	float m_letterBoxTimer;
	//	�{�^�������������ǂ���
	bool m_pushButton;

};
