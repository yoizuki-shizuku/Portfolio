/*/
 *  �t�@�C����		�F	ResultButtonManager.h
 *  �T�v			�F	���U���g��ʂł̃{�^�����Ǘ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/18
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include <array>

class ResultButtonManager
{
public:
	enum Process : int
	{
		MAINTAIN = -1,
		SELECT = 0,
		RETRY = 1,
		NEXT = 2,
	};

public:
	ResultButtonManager();
	~ResultButtonManager() = default;

	void Initialize(int stageIndex);
	Process Update();
	void Render();
	void Finalize();

private:
	//	�z��̃T�C�Y
	static const int SIZE = 3;

	//	�{�^���̉摜
	std::array < std::unique_ptr<UserInterface>, SIZE> m_texture;
	//	�C���t�H���[�V�����̉摜
	std::array<std::unique_ptr<UserInterface>, SIZE> m_info;

	//	�t�F�[�h�C���A�j���[�V����
	std::array <std::unique_ptr<Animator>, SIZE> m_fadeinAnimator;
	//	�I�����̃A�j���[�V����
	std::array<std::unique_ptr<Animator>, SIZE> m_choiceAnimator;

	//	�{�^���̓����蔻��
	std::array <std::unique_ptr<Button>, SIZE> m_buttons;

	//	�e�{�^���̃^�C�~���O�����炷���߂̃^�C�}�[
	float m_timer;
	//	�ŏI�X�e�[�W���������ǂ���
	bool m_isLastStage;

};