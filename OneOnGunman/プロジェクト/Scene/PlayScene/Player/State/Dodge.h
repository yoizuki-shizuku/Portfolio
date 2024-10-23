/*/
 *  �t�@�C����		�F	Dodge.h
 *  �T�v			�F	������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/05
/*/
#pragma once

#include "State.h"

class Player;

class Dodge final : public State
{
public:
	Dodge(Player* player, float* timeSpeed);
	~Dodge() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;


private:
	//	�X���[�A�j���[�V�����̎��ԑ��x����
	void SlowAnimation();

private:
	//	�v���C���[�̃N���X
	Player* m_player;
	//	�J�ڂ��Ă���̎��Ԃ��v��
	float m_timer;
	//	�Q�[�������Ԃ̌o�ߑ��x
	float* m_timeSpeed;
	//	�X���[���o�Ŏg���^�C�}�[
	float m_slowTimer;
	//	�W���X�g����ɂȂ������ǂ���
	bool m_justDodge;
	//	�V���b�g��Ԃւ̃L�����Z���\����
	float m_shotCancelTime;
	//	���G����
	float m_invincibleTime;

};