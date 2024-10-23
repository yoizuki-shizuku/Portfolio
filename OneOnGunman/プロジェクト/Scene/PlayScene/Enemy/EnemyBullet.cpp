/*/
 *  ファイル名		：	EnemyBullet.cpp
 *  概要			：	敵が打つ弾を管理しやすくするためのインターフェース
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/22
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

