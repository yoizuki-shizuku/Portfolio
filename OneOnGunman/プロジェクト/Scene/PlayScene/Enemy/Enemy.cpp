/*/
 *  �t�@�C����		�F	Enemy.cpp
 *  �T�v			�F	�G�̃N���X�̃C���^�[�t�F�[�X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/18
/*/
#include "pch.h"
#include "Enemy.h"

using namespace DirectX;

Enemy::Enemy()
{
	m_observer = std::make_unique<Observer>();
}

void Enemy::SetPosition(DirectX::SimpleMath::Vector3 pos)
{
	m_position = pos;
	m_collider->SetPosition(pos + m_offset);
}
