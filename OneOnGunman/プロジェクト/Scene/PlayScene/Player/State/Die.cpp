/*/
 *  �t�@�C����		�F	Die.cpp
 *  �T�v			�F	������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/06
/*/
#include "pch.h"
#include "Die.h"

using namespace DirectX;

#include "../Player.h"
#include "Library/Input.h"

Die::Die(Player* player) :
	State(),
	m_player(player)
{
	//	�A�j���[�V�����̐ݒ�
	SetAnimLoop(false);
	SetAnimEndTime(2.0);
}

void Die::Initialize()
{
	//	�A�j���[�V�������͂��߂���ɂ���
	RestartAnimetion();

	//	���x��0��
	m_player->SetVelocity(SimpleMath::Vector3::Zero);

	//	�����蔻�薳����
	m_player->GetCollider()->SetIsValid(false);
}

void Die::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

void Die::Finalize()
{
}
