/*/
 *  ファイル名		：	EnemyBullet.h
 *  概要			：	敵が打つ弾を管理しやすくするためのインターフェース
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/22
/*/
#pragma once

#include "Library/Collider.h"

class EnemyBullet
{
public:
	EnemyBullet();
	virtual ~EnemyBullet() = default;

	virtual void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) = 0;
	virtual bool Update(float elapsedTime) = 0;
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	virtual void Finalize() = 0;

	void SetPosition(DirectX::SimpleMath::Vector3 position);
	void SetCollider(Sphere* collider) { m_collider.reset(collider); }

	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	Collider* GetCollider()                    const { return m_collider.get(); }
	virtual DirectX::SimpleMath::Vector3 GetColor() const = 0;

private:
	//	位置
	DirectX::SimpleMath::Vector3 m_position;

	//	当たり判定
	std::unique_ptr<Sphere> m_collider;

};