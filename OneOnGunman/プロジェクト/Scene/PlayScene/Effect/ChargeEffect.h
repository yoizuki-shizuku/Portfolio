/*/
 *  ファイル名		：	ChargeEffect.h
 *  概要			：	プレイヤーのゲージが最大になった際のエフェクト
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/01/29
/*/
#pragma once

class ChargeEffect
{
public:
	//	定数バッファ(送信側)
	struct ConstBuffer
	{
		float timer;
		DirectX::SimpleMath::Vector3 none;
	};

public:
	ChargeEffect();
	~ChargeEffect() = default;

	void Initialize();
	void Update(float elapsedTime);
	void Render();

private:
	//	シェーダーに情報を渡すためのバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	ステンシルマスク用
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	//	タイマー
	float m_timer;
};
