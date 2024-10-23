/*/
 *  �t�@�C����		�F	Button.h
 *  �T�v			�F	�}�E�X�ő���ł���UI�̃{�^��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/11
/*/
#pragma once

#include "UserInterface.h"

class Button
{
public:
	Button(UserInterface* ui);
	~Button() = default;

	void Update();

	//	�}�E�X���e�N�X�`���Əd�Ȃ����u��
	bool OnMouse() const { return !m_beforeState && m_onTexture; }
	//	�}�E�X���e�N�X�`���ɏd�Ȃ��Ă���
	bool StayMouse() const { return m_onTexture; }
	//	�}�E�X���e�N�X�`�����痣�ꂽ�u��
	bool EnterMouse() const { return m_beforeState && !m_onTexture; }
	//	�{�^�����N���b�N���ꂽ�u��
	bool IsPresed() const { return !m_beforeButton && m_clickButton; }
	//	�{�^�����N���b�N����Ă���
	bool IsEnter() const { return m_clickButton; }
	//	�{�^�����N���b�N���ꂽ��Ԃ��������ꂽ�u��
	bool IsReleased() const { return m_beforeButton && m_clickButton; }

private:
	//	�摜
	UserInterface* m_texture;

	//	�}�E�X���e�N�X�`���Ɣ���Ă��邩
	bool m_onTexture;
	//	1F�O�̏��
	bool m_beforeState;

	//	�{�^����������Ă��邩
	bool m_clickButton;
	//	1f�O�̏��
	bool m_beforeButton;

};