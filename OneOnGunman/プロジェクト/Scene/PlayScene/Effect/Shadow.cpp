/*/
 *  ファイル名		：	Shadow.cpp
 *  概要			：	ポリゴンを使い簡易的な影を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/06
/*/
#include "pch.h"
#include "Shadow.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "WICTextureLoader.h"

//	影を表示する高さ
#define SHADOW_VIEW_HEIGHT	(0.1f)

Shadow::Shadow()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetTextureEnabled(true);

	//	画像データ読み込み
	DirectX::CreateWICTextureFromFile(device, L"Resources/Texture/Play/Particle.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(device, m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	m_size = SimpleMath::Vector2::One;
}

void Shadow::Update(DirectX::SimpleMath::Vector3 position)
{
	m_position = position;
	m_position.y = SHADOW_VIEW_HEIGHT;
}

void Shadow::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_effect->SetView(view);
	m_effect->SetProjection(proj);
	m_effect->SetWorld(SimpleMath::Matrix::CreateTranslation(m_position));
	m_effect->SetTexture(m_texture.Get());
	m_effect->SetColorAndAlpha(SimpleMath::Vector4(0.f, 0.f, 0.f, 1.f));
	m_effect->Apply(context);

	//	半透明描画指定
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFF);
	//	カリングは左周り
	context->RSSetState(states->CullCounterClockwise());
	//	深度バッファの設定
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	デフォルトの頂点位置
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-m_size.x, 0.f, -m_size.y), SimpleMath::Vector2(0.f, 0.f)),	//	左上
		VertexPositionTexture(SimpleMath::Vector3( m_size.x, 0.f, -m_size.y), SimpleMath::Vector2(1.f, 0.f)),	//	右上
		VertexPositionTexture(SimpleMath::Vector3( m_size.x, 0.f,  m_size.y), SimpleMath::Vector2(1.f, 1.f)),	//	右下
		VertexPositionTexture(SimpleMath::Vector3(-m_size.x, 0.f,  m_size.y), SimpleMath::Vector2(0.f, 1.f)),	//	左下
	};

	//	描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

void Shadow::Finalize()
{
}
