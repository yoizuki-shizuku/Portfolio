/*/
 *  ファイル名		：	PlayerBullet.h
 *  概要			：	プレイヤーの弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#pragma once

#include "GeometricPrimitive.h"
#include "Library/Collider.h"
#include "../Effect/LandingEffect.h"

//	弾の外見を描画するクラスを管理
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
	//	実際にエフェクトを描画するクラス
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
		//	モデル
		std::unique_ptr<DirectX::GeometricPrimitive> m_model;
		//	ピクセルシェーダー
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		//	頂点シェーダー
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		//	インプットレイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	};

private:
	//	光る弾
	std::unique_ptr<BulletEffect> m_luminous;
	//	電気
	std::unique_ptr<BulletEffect> m_thunder;
	//	周りのオーラ
	std::unique_ptr<BulletEffect> m_aura;

	//	定数バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	位置
	DirectX::SimpleMath::Vector3 m_position;
	//	アニメーション用タイマー
	float m_timer;
	//	角度
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
	//	位置
	DirectX::SimpleMath::Vector3 m_position;
	//	進む方向
	DirectX::SimpleMath::Vector3 m_velocity;
	//	生成されてから打ち出された方向
	DirectX::SimpleMath::Vector3 m_firstVel;

	//	弾が有効な状態か
	bool m_isValid;
	//	弾の有効時間計測
	float m_validTimer;

	//	コライダー
	std::unique_ptr<Sphere> m_collider;
	//	着弾エフェクト
	std::unique_ptr<LandingEffect> m_landingEffect;
	//	弾の見た目
	std::unique_ptr<BulletAppearance> m_model;

};