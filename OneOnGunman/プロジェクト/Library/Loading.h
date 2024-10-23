/*/
 *  ファイル名		：	Loading.h
 *  概要			：	ロード画面を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/27
/*/
#pragma once

#include "../Scene/UserInterface/GlitchEffect.h"

class Loading
{
public:
	Loading();
	~Loading() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

private:
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	//	ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	NowLoading画像
	std::unique_ptr<GlitchEffect> m_texture;

};