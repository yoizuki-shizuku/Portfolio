/*/
 *  �t�@�C����		�F	PleasePressAnyKey.h
 *  �T�v			�F	�n�߂ɃL�[���������܂�UI��\�����A���̃L�[�̉��������m����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/08
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"

class PleaseLeftClick
{
public:
	PleaseLeftClick();
	~PleaseLeftClick() = default;

	void Initialize();
	bool Update();
	void Render();
	void Finalize();

private:
	//	�e�N�X�`��
	std::unique_ptr<UserInterface> m_texture;
	//	�A�j���[�V�����p�R�}���_�[
	std::unique_ptr<Animator> m_fadeIn;

	//	���N���b�N��������
	bool m_isLeftClick;

};