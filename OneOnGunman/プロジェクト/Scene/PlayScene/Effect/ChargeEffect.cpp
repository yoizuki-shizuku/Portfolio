/*/
 *  ファイル名		：	ChargeEffect.cpp
 *  概要			：	プレイヤーのゲージが最大になった際のエフェクト
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/01/29
/*/
#include "pch.h"
#include "ChargeEffect.h"

using namespace DirectX;

#include "ReadData.h"
#include "Library/UserResources.h"

ChargeEffect::ChargeEffect() :
	m_timer(0.f)
{
}

void ChargeEffect::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/ChargeEffect/ChargeEffectVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/ChargeEffect/ChargeEffectPS.cso");

	//	各シェーダーの作成
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	インプットレイアウトの作成
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		vsBlob.data(), vsBlob.size(),
		m_inputLayout.GetAddressOf());

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	深度ステンシルの作成 
	D3D11_DEPTH_STENCIL_DESC desc;

	desc.DepthEnable = FALSE;									// 深度テストを行う
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;			// 深度バッファを更新しない
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// 深度値以下なら更新する

	desc.StencilEnable = TRUE;									// ステンシルテストを行う
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// 表面
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;		// １以上なら
	//desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// １以上なら
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// OK　何もしない
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG　何もしない
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG　何もしない

	desc.BackFace = desc.FrontFace;	// 裏面も同じ

	DX::ThrowIfFailed(
		device->CreateDepthStencilState(&desc, m_depthStencilState.ReleaseAndGetAddressOf())
	);
}

void ChargeEffect::Update(float elapsedTime)
{
	m_timer += elapsedTime;
}

void ChargeEffect::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-1.f, 1.f,0.f) ,SimpleMath::Vector2(0.f, 1.f)),	//	左上
		VertexPositionTexture(SimpleMath::Vector3(1.f, 1.f,0.f) ,SimpleMath::Vector2(1.f, 1.f)),	//	右上
		VertexPositionTexture(SimpleMath::Vector3(1.f,-1.f,0.f) ,SimpleMath::Vector2(1.f, 0.f)),	//	右下
		VertexPositionTexture(SimpleMath::Vector3(-1.f,-1.f,0.f) ,SimpleMath::Vector2(0.f, 0.f)),	//	左下
	};

	//	シェーダーに渡すバッファを作成する
	ConstBuffer cbuff;
	cbuff.timer = m_timer;

	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//半透明描画指定
	auto states = UserResources::GetInstance()->GetCommonStates();
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// カリングはしない
	context->RSSetState(states->CullNone());

	//	深度バッファの設定
	context->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	シェーダーをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	//	シェーダーの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
