/*/
 *  ファイル名		：	Hologram.h
 *  概要			：	ステージ選択時の敵の見た目をホログラムで表示
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/16
/*/
#pragma once

class Hologram
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	xのみ
	};

public:
	Hologram();
	~Hologram() = default;

	void Initialize(const wchar_t* model, const wchar_t* tex);
	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void SetScale(float scale) { m_scale = scale; }

private:
	void CreateShader();

private:
	//	モデル
	std::unique_ptr<DirectX::Model> m_model;
	//	定数バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//	表示アニメーション用のモデルの大きさ
	float m_scale;

	//	アニメーション用タイマー
	float m_timer;

};
