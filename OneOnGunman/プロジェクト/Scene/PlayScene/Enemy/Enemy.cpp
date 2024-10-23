/*/
 *  ファイル名		：	Enemy.cpp
 *  概要			：	敵のクラスのインターフェース
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/18
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
