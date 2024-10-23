/*/
 *  ファイル名		：	LandingEffect.h
 *  概要			：	弾の着弾時のエフェクト
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/12
/*/
#pragma once

#include <array>

struct Particle
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 velocity;
};

class LandingEffect
{
public:
	LandingEffect();
	~LandingEffect() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	//	エフェクトアニメーションの開始
	void StartEffect(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 color);

	//	エフェクトアニメーションの停止
	void StopEffect();

private:
	//	view行列からカメラの位置を求める
	DirectX::SimpleMath::Vector3 ClacCameraPos(const DirectX::SimpleMath::Matrix& view);

	//	配列のサイズ
	static const int SIZE = 80;

private:
	//	板ポリゴンを表示するプリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	エフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_effect;
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	パーティクル
	std::array<Particle, SIZE> m_particles;
	//	色
	DirectX::SimpleMath::Vector3 m_color;
	//	開始位置
	DirectX::SimpleMath::Vector3 m_startPos;

	//	タイマー
	float m_timer;
	//	パーティクルの透明度
	float m_alpha;
	//	アニメーションの実行中
	bool m_isAnimation;

};