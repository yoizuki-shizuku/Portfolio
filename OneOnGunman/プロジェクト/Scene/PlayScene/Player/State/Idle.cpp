/*/
 *  �t�@�C����		�F	Idle.cpp
 *  �T�v			�F	�ҋ@���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/21
/*/
#include "pch.h"
#include "Idle.h"

using namespace DirectX;

#include "Library/Input.h"

#include "../Player.h"

Idle::Idle(Player* player) :
	State(),
	m_player(player)
{
	//	�A�j���[�V�����̐ݒ�
	SetAnimEndTime(1.5);
	SetAnimLoop(true);
}

void Idle::Initialize()
{
	m_player->SetVelocity(SimpleMath::Vector3::Zero);
}

void Idle::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	auto kb = Keyboard::Get().GetState();

	//	�v���C���[���������Ă��Ȃ���΂����Ԃɂ���
	if (!m_player->GetIsAlive())
	{
		m_player->ChangeState(m_player->GetDie());
		return;
	}

	//	�}�E�X�̍��N���b�N����������V���b�g��Ԃ�
	auto msTracker = Input::GetInstace()->GetMouseTracker();
	if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED && m_player->CanShot())
	{
		m_player->ChangeState(m_player->GetShot());
	}

	//	WASD�������ꂽ��ړ���Ԃɂ���
	if (kb.W || kb.A || kb.S || kb.D)
	{
		m_player->ChangeState(m_player->GetRun());
	}
}

void Idle::Finalize()
{
}