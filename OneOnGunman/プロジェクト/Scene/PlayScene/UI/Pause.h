/*/
 *  �t�@�C����		�F	Pause.h
 *  �T�v			�F	�|�[�Y���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/12/15
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include <array>

class Pause
{
public:
	enum class Process
	{
		PAUSE = -2,
		MAINTAIN = -1,
		CONTINUE = 0,
		RESTART = 1,
		SELECT = 2
	};


public:
	Pause();
	~Pause() = default;

	void Initialize();
	Process Update();
	void Render();
	void Finalize();

	void RetryGame();

private:
	//	�uPause�v�̕\��
	std::unique_ptr<UserInterface> m_pause;

	//	���^�[�{�b�N�X
	std::unique_ptr<UserInterface> m_letterBox_1;
	std::unique_ptr<UserInterface> m_letterBox_2;

	//	�z��̃T�C�Y
	static const int SIZE = 3;

	//	�{�^���̉摜
	std::array<std::unique_ptr<UserInterface>,SIZE> m_texture;
	//	�A�j���[�V����
	std::array<std::unique_ptr<Animator>, SIZE> m_animation;
	//	�{�^��
	std::array<std::unique_ptr<Button>, SIZE> m_button;

	//	���ݍs���Ă��鏈��
	Process m_process;
	//	���^�[�{�b�N�X�̃A�j���[�V�����^�C�}�[
	float m_letterBoxTimer;

};