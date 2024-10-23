/*/
 *  ファイル名		：	InfinitePlane.h
 *  概要			：	背景用の平面
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/24
/*/
#pragma once

class InfinitePlane
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix proj;
		DirectX::SimpleMath::Matrix world;
		DirectX::SimpleMath::Vector4 time;
	};

public:
	InfinitePlane();
	~InfinitePlane() = default;

	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	void CreateShader();

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


	//	アニメーション用のタイマー
	float m_timer;

};
