/*/
 *  ファイル名		：	Shadow.h
 *  概要			：	ポリゴンを使い簡易的な影を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/06
/*/
#pragma once

class Shadow
{
public:
	Shadow();
	~Shadow() = default;

	void Update(DirectX::SimpleMath::Vector3 position);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void SetSize(DirectX::SimpleMath::Vector2 size) { m_size = size; }

private:
	//	板ポリゴンを表示するプリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//	描画する位置
	DirectX::SimpleMath::Vector3 m_position;
	//	サイズ
	DirectX::SimpleMath::Vector2 m_size;

};
