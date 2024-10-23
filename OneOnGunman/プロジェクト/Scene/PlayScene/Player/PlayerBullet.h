/*/
 *  �t�@�C����		�F	PlayerBullet.h
 *  �T�v			�F	�v���C���[�̒e
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#pragma once

#include "GeometricPrimitive.h"
#include "Library/Collider.h"
#include "../Effect/LandingEffect.h"

//	�e�̊O����`�悷��N���X���Ǘ�
class BulletAppearance
{
public:
	BulletAppearance();
	~BulletAppearance() = default;

	void Initialize();
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 position);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void CreateBullet(DirectX::SimpleMath::Vector3 velocity);

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector3 position;
		float time;
		DirectX::SimpleMath::Matrix rotate;
	};

private:
	//	���ۂɃG�t�F�N�g��`�悷��N���X
	class BulletEffect
	{
	public:
		BulletEffect(float d, const wchar_t* vertex, const wchar_t* pixel);
		~BulletEffect() = default;

		void Render(const DirectX::SimpleMath::Matrix& world,
					const DirectX::SimpleMath::Matrix& view,
					const DirectX::SimpleMath::Matrix& proj,
					ID3D11Buffer* cbuff);
		void Finalize();

	private:
		//	���f��
		std::unique_ptr<DirectX::GeometricPrimitive> m_model;
		//	�s�N�Z���V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		//	���_�V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		//	�C���v�b�g���C�A�E�g
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	};

private:
	//	����e
	std::unique_ptr<BulletEffect> m_luminous;
	//	�d�C
	std::unique_ptr<BulletEffect> m_thunder;
	//	����̃I�[��
	std::unique_ptr<BulletEffect> m_aura;

	//	�萔�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	�ʒu
	DirectX::SimpleMath::Vector3 m_position;
	//	�A�j���[�V�����p�^�C�}�[
	float m_timer;
	//	�p�x
	float m_radian;

};

class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void RetryGame();

	void CreateBullet(DirectX::SimpleMath::Vector3 createPos, DirectX::SimpleMath::Vector3 moveAngle);

	Sphere* GetCollider() { return m_collider.get(); }

private:
	//	�ʒu
	DirectX::SimpleMath::Vector3 m_position;
	//	�i�ޕ���
	DirectX::SimpleMath::Vector3 m_velocity;
	//	��������Ă���ł��o���ꂽ����
	DirectX::SimpleMath::Vector3 m_firstVel;

	//	�e���L���ȏ�Ԃ�
	bool m_isValid;
	//	�e�̗L�����Ԍv��
	float m_validTimer;

	//	�R���C�_�[
	std::unique_ptr<Sphere> m_collider;
	//	���e�G�t�F�N�g
	std::unique_ptr<LandingEffect> m_landingEffect;
	//	�e�̌�����
	std::unique_ptr<BulletAppearance> m_model;

};