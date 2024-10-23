/*/
 *  �t�@�C����		�F	EnemyBullet.cpp
 *  �T�v			�F	�G���łe���Ǘ����₷�����邽�߂̃C���^�[�t�F�[�X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/22
/*/
#include "pch.h"
#include "EnemyBullet.h"

using namespace DirectX;

EnemyBullet::EnemyBullet()
{

}

void EnemyBullet::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	m_position = position;
	m_collider->SetPosition(position);
}

