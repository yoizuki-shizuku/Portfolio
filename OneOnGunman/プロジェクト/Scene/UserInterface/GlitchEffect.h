/*/
 *  ファイル名		：	GlitchEffect.h
 *  概要			：	グリッチエフェクトを適用した画像を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/11
/*/
#pragma once

#include "UserInterface.h"

class GlitchEffect
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		//	画面の大きさ
		DirectX::SimpleMath::Vector4	windowSize;
	};

public:
	GlitchEffect();
	~GlitchEffect() = default;

	//	画像データを読み込む
	void LoadTexture(const wchar_t* path);

	void Update();
	void Render();

public:
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	void SetScale(const DirectX::SimpleMath::Vector2 scale) { m_scale = scale; }
	void SetAnchor(const Anchor anchor) { m_anchor = anchor; }

private:
	//	シェーダーの作成
	void CreateShader();

private:
	//	シェーダーに情報を渡すためのバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	位置
	DirectX::SimpleMath::Vector2 m_position;
	//	大きさ
	DirectX::SimpleMath::Vector2 m_scale;
	//	アンカーポイント
	Anchor m_anchor;
	//	画面の大きさ
	int m_windowWidth, m_windowHeight;
	//	テクスチャの元の大きさ
	int m_textureWidth, m_textureHeight;

	//	時間計測用
	float m_timer;

};