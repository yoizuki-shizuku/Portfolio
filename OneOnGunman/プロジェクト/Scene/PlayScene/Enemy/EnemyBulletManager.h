/*/
 *  ファイル名		：	EnemyBulletManager.h
 *  概要			：	敵の弾を一括管理するクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/25
/*/
#pragma once

#include "EnemyBullet.h"
#include <vector>
#include "../Effect/LandingEffect.h"

class EnemyBulletManager
{
public:
	EnemyBulletManager();
	~EnemyBulletManager() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	//	弾を追加する
	void AddBullet(std::unique_ptr<EnemyBullet> bullet);
	//	当たり判定をとる
	bool CheckHitBullet(Collider* col, Collider* dodge);

	//	リトライ時のフラグ初期化
	void RetryGame();

private:
	//	弾を格納する配列
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets;
	//	ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	着弾エフェクト
	std::unique_ptr<LandingEffect> m_landingEffect;

};