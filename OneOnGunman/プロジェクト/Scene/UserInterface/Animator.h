/*/
 *  �t�@�C����		�F	Animator.h
 *  �T�v			�F	�A�j���[�V�������t�@�C�����琶���A�ێ����Ă����A���s���Ă����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/04
/*/
#pragma once

#include "UIAnimation.h"
#include <array>
#include <sstream>
#include "Library/Easing.h"
#include "picojson.h"

class UserInterface;

class Animator
{
public:
	Animator(UserInterface* ui);
	~Animator() = default;

	void LoadFile(const std::string path);
	void Update(float elapsedTime);
	void Finalize();

public:
	//	�A�j���[�V�������n�߂���s��
	void StartAnimation() { m_isAnimation = true; m_timer = 0.f; m_reverse = false; }
	//	�A�j���[�V�������t�Đ�����
	void StartReverseAnimation() { m_isAnimation = true; m_timer = m_requiredTime; m_reverse = true; }
	//	�A�j���[�V�������~�߂�
	void StopAnimation() { m_isAnimation = false; }
	//	�A�j���[�V�������ĊJ����(�r������)
	void PlayAnimation() { m_isAnimation = true; m_reverse = false; }
	//	�A�j���[�V�������ĊJ����(�t�Đ�)
	void PlayReverseAnimation() { m_isAnimation = true; m_reverse = true; }
	//	�A�j���[�V�������I������
	void FinishAnimation();
	//	�A�j���[�V���������Z�b�g����(0�b�̏�Ԃɂ���)
	void ResetAnimation();

	//	�A�j���[�V�������Đ�����Ă��邩�ǂ���
	bool GetIsAnimation() const { return m_isAnimation; }
	//	�A�j���[�V���������łɍĐ�����A�I���������ǂ���
	//	(���[�v���Ȃ��A�j���[�V�����̂�)
	bool GetFinishAnimation() const { return (m_timer >= m_requiredTime && !m_reverse) || (m_timer <= 0.f && m_reverse) && !m_isAnimation; }

private:
	//	MoveCommand�𐶐�
	void MakeMoveCommand(picojson::array move);
	//	RotationCommand�𐶐�
	void MakeRotationCommand(picojson::array rota);
	//	ScaleCommand�𐶐�
	void MakeScaleCommand(picojson::array scale);
	//	AlphaCommand���쐬
	void MakeAlphaCommand(picojson::array alpha);

private:
	//	UserInterface�̃|�C���^
	UserInterface* m_userInterface;

	//	�R�}���h
	std::vector<std::unique_ptr<UIAnimation>> m_commands;

	//	�A�j���[�V�����̎��s����
	float m_timer;
	//	�ǂݍ��񂾃A�j���[�V�����̏��v����
	float m_requiredTime;

	//	�A�j���[�V���������s�����ǂ���
	bool m_isAnimation;

	//	���[�v����̂��ǂ���
	bool m_isLoop;
	//	���΃A�j���[�V�������ǂ���
	//	(UserInterface�̏�����Ԃ����ɃA�j���[�V����������)
	//	(�����悤�ȃA�j���[�V������1�̃t�@�C���Ŏ����ł���)
	bool m_relative;
	//	�t�Đ������ǂ���
	bool m_reverse;

};
