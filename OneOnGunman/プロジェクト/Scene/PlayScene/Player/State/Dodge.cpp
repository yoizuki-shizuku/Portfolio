/*/
 *  �t�@�C����		�F	Dodge.cpp
 *  �T�v			�F	������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/05
/*/
#include "pch.h"
#include "Dodge.h"

using namespace DirectX;

#include "../Player.h"
#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

//	������̑��x
#define DODGE_SPEED			(9.f)
//	�����W��
#define FRICTION			(5.5f)

//	�ҋ@��Ԃֈڍs����
#define IDLE_BASE			(1.2f)

//	�ˌ���Ԃւ̃L�����Z���
#define SHOT_BASE			(0.8f)
//	�ˌ���Ԃւ̃L�����Z���(�W���X�g�����)
#define JUST_SHOT_BASE		(0.4f)

//	���G����
#define INVINCIBLE			(0.8f)
//	�W���X�g������̖��G����
#define JUST_INVINCIBLE		(1.1f)

//	�W���X�g������̃N�[���^�C���ւ̃{�[�i�X
#define JUST_DODGE_BONUS	(2.f)

//	�X���[�A�j���[�V�����p�萔
#define RELEASE_SLOW		(0.6f)

Dodge::Dodge(Player* player, float* timeSpeed) :
	State(),
	m_player(player),
	m_timer(0.f),
	m_timeSpeed(timeSpeed),
	m_slowTimer(0.f),
	m_justDodge(false),
	m_shotCancelTime(SHOT_BASE),
	m_invincibleTime(INVINCIBLE)
{
	//	�A�j���[�V�����̐ݒ�
	SetAnimSpeed(1.6f);
	SetAnimStartTime(0.2);
	SetAnimLoop(false);
}

void Dodge::Initialize()
{
	//	�A�j���[�V�������͂��߂���ɂ���
	RestartAnimetion();

	//	�^�C�}�[�����Z�b�g
	m_timer = 0.f;
	m_slowTimer = 0.f;

	//	�ړ����Ă�������։������
	m_player->SetVelocity(m_player->GetVelocity() * DODGE_SPEED);

	//	���G��Ԃɂ���
	m_player->SetColliderValid(false);

	//	�N�[���^�C�������Z�b�g
	m_player->ResetDodgeCoolTime();

	//	�W���X�g����Ȃ�Ύ��Ԃ�x�����鉉�o���J�n
	m_justDodge = m_player->GetJustDodge()->GetIsHit();

	//	�W���X�g������������ۂɕK�v�ȏ���
	if (m_justDodge)
	{
		//	�Q�[�W�Ƀ{�[�i�X��t�^
		m_player->ShorteningShotCoolTime(JUST_DODGE_BONUS);

		//	�L�����Z���△�G���Ԃ̐ݒ�
		m_shotCancelTime = JUST_SHOT_BASE;
		m_invincibleTime = JUST_INVINCIBLE;
	}
	else
	{
		m_shotCancelTime = SHOT_BASE;
		m_invincibleTime = INVINCIBLE;
	}
}

void Dodge::Update(float elapsedTime)
{
	//	�v���C���[���������Ă��Ȃ���΂����Ԃɂ���
	if (!m_player->GetIsAlive())
	{
		m_player->ChangeState(m_player->GetDie());
		return;
	}

	//	�^�C�}�[�X�V
	m_timer += elapsedTime;

	//	�X���[���[�V�������o
	SlowAnimation();

	//	���x�̌����������ԏ�����
	//	���b���X�Ɍ�������
	SimpleMath::Vector3 friction = m_player->GetVelocity() * FRICTION * elapsedTime;
	m_player->SetVelocity(m_player->GetVelocity() - friction);

	//	��莞�Ԍo�ߌ�ɃV���b�g��ԂփL�����Z���\
	if (m_timer >= m_shotCancelTime)
	{
		//	�}�E�X�̍��N���b�N����������V���b�g��ԂփL�����Z������
		auto msTracker = Input::GetInstace()->GetMouseTracker();
		if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED && m_player->CanShot())
		{
			*m_timeSpeed = 1.f;
			m_player->SetColliderValid(true);
			m_player->ChangeState(m_player->GetShot());
		}
	}

	//	��莞�Ԍ�ɖ��G������
	if (m_timer >= m_invincibleTime)
	{
		m_player->SetColliderValid(true);
	}

	//	��莞�Ԍo�ĂΑҋ@��Ԃ�
	if (m_timer < IDLE_BASE) return;

	m_player->ChangeState(m_player->GetIdle());
}

void Dodge::Finalize()
{
}

void Dodge::SlowAnimation()
{
	//	�W���X�g������Ă��Ȃ���Ώ������Ȃ�
	if (!m_justDodge) return;

	//	�^�C�}�[�X�V(�������ԏ���)
	m_slowTimer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	���ۂ̃A�j���[�V��������
	if (m_slowTimer < RELEASE_SLOW)
	{
		*m_timeSpeed = Easing::Lerp(0.05f, 1.f, 
			Useful::Ratio01(m_slowTimer, RELEASE_SLOW), Easing::InQuint);
	}
	else
	{
		*m_timeSpeed = 1.f;
	}
}
