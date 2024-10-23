/*/
 *  �t�@�C����		�F	NormalBullet.h
 *  �T�v			�F	���i���邾���̊�{�I�Ȓe
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/25
/*/
#pragma once

#include "../EnemyBullet.h"
#include "GeometricPrimitive.h"

class NormalBullet final : public EnemyBullet
{
public:
	NormalBullet(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 velocity);
	~NormalBullet() = default;

	void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) override;
	bool Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	DirectX::SimpleMath::Vector3 GetColor() const override { return DirectX::SimpleMath::Vector3(1.f, 0.f, 0.f); }

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 color;
	};

private:
	//	�ړ������x�N�g��
	DirectX::SimpleMath::Vector3 m_velocity;
	//	�����p�^�C�}�[
	float m_timer;
	//	�����f��
	std::unique_ptr<DirectX::GeometricPrimitive> m_model;
	//	�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�萔�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

};