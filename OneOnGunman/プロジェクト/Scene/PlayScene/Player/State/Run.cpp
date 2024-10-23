/*/
 *  �t�@�C����		�F	Run.cpp
 *  �T�v			�F	�ړ����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#include "pch.h"
#include "Run.h"

using namespace DirectX;

#include "Library/Input.h"

#include "../Player.h"

//	�ړ����x
#define MOVE_SPEED		(2.5f)

Run::Run(Player* player) :
	State(),
	m_player(player)
{
	//	�A�j���[�V�����̐ݒ�
	SetAnimEndTime(0.7);
	SetAnimLoop(true);
}

void Run::Initialize()
{
}

void Run::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

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
		return;
	}

	auto kb = Keyboard::Get().GetState();

	//	�ړ���
	SimpleMath::Vector3 move = SimpleMath::Vector3::Zero;
	//	�O�����x�N�g��
	SimpleMath::Vector3 front = m_player->GetFront();
	//	�������x�N�g��
	SimpleMath::Vector3 hori = SimpleMath::Vector3(front.z, 0.f, -front.x);

	//	�O��
	if (kb.W) { move += front; }
	else if (kb.S) { move -= front; }
	//	���E
	if (kb.A) { move += hori; }
	else if (kb.D) { move -= hori; }

	//	���K��
	move.Normalize();

	//	�����Ă��Ȃ���Αҋ@�X�e�[�g�ɕύX
	if (move == SimpleMath::Vector3::Zero)
	{
		m_player->ChangeState(m_player->GetIdle());
		return;
	}

	//	�i�s�����֌����p�x
	float angle = atan2f(move.x, move.z) + XM_PI;
	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(angle, 0.f, 0.f);

	//	Velocity�ɓK�p����
	m_player->SetVelocity(move * MOVE_SPEED);

	//	�}�E�X�̉E�N���b�N�������ꂽ������Ԃ�
	if (msTracker->rightButton == Mouse::ButtonStateTracker::PRESSED && m_player->CanDodge())
	{
		//	�������֑����Ɍ�����
		m_player->SetAngle(q);

		m_player->ChangeState(m_player->GetDodge());
		return;
	}

	//	���X�ɂ��̕����֌�����
	m_player->SetAngle(SimpleMath::Quaternion::Lerp(m_player->GetAngle(), q, 0.2f));
}

void Run::Finalize()
{
}
