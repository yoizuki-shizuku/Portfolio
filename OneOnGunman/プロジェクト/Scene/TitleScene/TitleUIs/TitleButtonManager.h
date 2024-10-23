/*/
 *  �t�@�C����		�F	TitleButtonManager.h
 *  �T�v			�F	�^�C�g����ʂł̃{�^�����Ǘ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/08
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include <array>

class TitleButtonManager
{
public:
	enum Process
	{
		MAINTAIN = -1,
		START	 = 0,
		OPTION	 = 1,
		CREDIT	 = 2,
		EXIT	 = 3
	};

public:
	TitleButtonManager();
	~TitleButtonManager() = default;

	void Initialize();
	int Update();
	void Render();
	void Finalize();

private:
	//	�z��̃T�C�Y
	static const int SIZE = 4;

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
};