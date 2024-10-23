/*/
 *  �t�@�C����		�F	BeginEvent.h
 *  �T�v			�F	�Q�[���J�n�O�̉��o
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/01
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"

class BeginEvent
{
public:
	BeginEvent();
	~BeginEvent() = default;

	void Initialize();
	bool Update();
	void Render();
	void Finalize();

	//	���g���C���̃t���O������
	void RetryGame();

private:
	//	Ready�̕���
	std::unique_ptr<UserInterface> m_ready;
	//	GO�̕���
	std::unique_ptr<UserInterface> m_go;
	//	���^�[�{�b�N�X
	std::unique_ptr<UserInterface> m_letterBox_1;
	std::unique_ptr<UserInterface> m_letterBox_2;
	//	Ready�̃A�j���[�V����
	std::unique_ptr<Animator> m_readyAnim;
	//	GO�̃A�j���[�V����
	std::unique_ptr<Animator> m_goAnim;
	//	���^�[�{�b�N�X�̃A�j���[�V�����^�C�}�[
	float m_letterBoxTimer;

};