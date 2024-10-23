/*/
 *  ファイル名		：	Numeral.h
 *  概要			：	一枚の画像から部分的に切り取り、数字を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/17
/*/
#pragma once

class Numeral
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		float windowWidth;		//	画面の横幅
		float windowHeight;		//	画面の縦幅
		float alpha;			//	透明度
		int numeral;			//	表示する数字
	};

public:
	Numeral();
	~Numeral() = default;

	//	画像データを読み込む
	void LoadTexture(const wchar_t* path);

	void Render();

public:
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	void SetRadian(const float& radian) { m_radian = radian; }
	void SetSize(const DirectX::SimpleMath::Vector2 size) { m_size = size; }
	void SetAlpha(const float& alpha) { m_alpha = alpha; }
	void SetNumber(const int& number) { m_number = number; }

private:
	//	シェーダーの作成
	void CreateShader();

private:
	//	シェーダーに情報を渡すためのバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
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
	//	角度
	float m_radian;
	//	サイズ(ピクセル単位)
	DirectX::SimpleMath::Vector2 m_size;
	//	透明度
	float m_alpha;
	//	表示する数字
	int m_number;

};