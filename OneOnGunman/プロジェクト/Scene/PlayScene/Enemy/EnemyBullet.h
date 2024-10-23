/*/
 *  �t�@�C����		�F	EnemyBullet.h
 *  �T�v			�F	�G���łe���Ǘ����₷�����邽�߂̃C���^�[�t�F�[�X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/22
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
	//	�ʒu
	DirectX::SimpleMath::Vector3 m_position;

	//	�����蔻��
	std::unique_ptr<Sphere> m_collider;

};