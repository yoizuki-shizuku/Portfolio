/*/
 *  �t�@�C����		�F	HomingBullet.h
 *  �T�v			�F	�v���C���[��ǔ�����e
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/21
/*/
#pragma once

#include "../EnemyBullet.h"
#include "GeometricPrimitive.h"
#include "../../Player/Player.h"

class HomingBullet final : public EnemyBullet
{
public:
	HomingBullet(DirectX::SimpleMath::Vector3 position, Player* target, float speed);
	~HomingBullet() = default;

	void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) override;
	bool Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	DirectX::SimpleMath::Vector3 GetColor() const override { return DirectX::SimpleMath::Vector3(1.f, 1.f, 0.f); }

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 color;
	};

private:
	//	�v���C���[�ւ̕����x�N�g��
	DirectX::SimpleMath::Vector3 ToPlayerVector();

private:
	//	�v���C���[�̃|�C���^
	Player* m_target;
	//	�i�s�����x�N�g��
	DirectX::SimpleMath::Vector3 m_direction;
	//	�e�̑��x
	float m_speed;
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