/*/
 *  ファイル名		：	Loading.cpp
 *  概要			：	ロード画面を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/27
/*/
#include "pch.h"
#include "Loading.h"

using namespace DirectX;

#include "UserResources.h"

Loading::Loading()
{
}

void Loading::Initialize()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	//	ベーシックエフェクトの作成
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetVertexColorEnabled(true);

	//	インプットレイアウトの作成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	//	画面の大きさ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_texture = std::make_unique<GlitchEffect>();
	m_texture->LoadTexture(L"Resources/Texture/NowLoading.png");
	m_texture->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_texture->SetPosition(SimpleMath::Vector2(width, height));
}

void Loading::Update()
{
	m_texture->Update();
}

void Loading::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	頂点情報
	VertexPositionColor vertex[4] =
	{
		VertexPositionColor(SimpleMath::Vector3(-1.f, 1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	左上
		VertexPositionColor(SimpleMath::Vector3( 1.f, 1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	右上
		VertexPositionColor(SimpleMath::Vector3( 1.f,-1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	右下
		VertexPositionColor(SimpleMath::Vector3(-1.f,-1.f,0.f) ,SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f)),	//	左下
	};

	//半透明描画指定
	auto states = UserResources::GetInstance()->GetCommonStates();
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// カリングは左周り
	context->RSSetState(states->CullNone());

	//	深度バッファを設定
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	//	ベーシックエフェクトを設定
	m_effect->Apply(context);
	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	m_texture->Render();
}

void Loading::Finalize()
{
}
