/*/
 *  �t�@�C����		�F	TorpedoBullet.h
 *  �T�v			�F	�����̂悤�ɏ��X�ɉ������Ă����e
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/20
/*/
#pragma once

#include "../EnemyBullet.h"
#include "GeometricPrimitive.h"

class TorpedoBullet final : public EnemyBullet
{
public:
	TorpedoBullet(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 maxVelocity);
	~TorpedoBullet() = default;

	void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) override;
	bool Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	DirectX::SimpleMath::Vector3 GetColor() const override { return DirectX::SimpleMath::Vector3(1.f, 0.647f, 0.f); }

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 color;
	};

private:
	//	�ő呬�x
	DirectX::SimpleMath::Vector3 m_maxVelocity;
	//	���ݑ��x
	DirectX::SimpleMath::Vector3 m_nowVelocity;
	//	�^�C�}�[
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