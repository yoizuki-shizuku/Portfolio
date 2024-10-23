/*/
 *  ファイル名		：	Numeral.cpp
 *  概要			：	一枚の画像から部分的に切り取り、数字を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/17
/*/
#include "pch.h"
#include "Numeral.h"

using namespace DirectX;

#include "WICTextureLoader.h"
#include "ReadData.h"
#include "Library/UserResources.h"

Numeral::Numeral() :
	m_radian(0.f),
	m_alpha(1.f),
	m_number(0)
{
	//	変数初期化
	m_position = SimpleMath::Vector2::Zero;
	m_size = SimpleMath::Vector2(100.f, 100.f);

	//	プリミティブバッチの作成
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	シェーダーの作成
	CreateShader();
}


void Numeral::LoadTexture(const wchar_t* path)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	画像データ読み込み
	DirectX::CreateWICTextureFromFile(device, path, nullptr, m_texture.ReleaseAndGetAddressOf());
}

void Numeral::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	頂点情報
	//	Position.xy : 位置
	//	Position.z  : 角度
	//	Tex.xy		: サイズ
	VertexPositionTexture vertex =
	{
		SimpleMath::Vector3(m_position.x, m_position.y, m_radian),
		SimpleMath::Vector2(m_size)
	};

	RECT windowSize = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();

	//	シェーダーに渡すバッファを作成する
	ConstBuffer cbuff{};
	cbuff.windowWidth = static_cast<float>(windowSize.right);
	cbuff.windowHeight = static_cast<float>(windowSize.bottom);
	cbuff.alpha = m_alpha;
	cbuff.numeral = m_number;

	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	半透明描画指定
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);

	//	カリングは左周り
	context->RSSetState(states->CullNone());

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	シェーダーをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	ピクセルシェーダーにテクスチャを登録する
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//	板ポリゴンを描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex, 1);
	m_batch->End();

	//	シェーダーの登録を解除
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void Numeral::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/Numeral/NumeralVS.cso");
	gsBlob = DX::ReadData(L"Resources/CSO/Numeral/NumeralGS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/Numeral/NumeralPS.cso");

	//	各シェーダーの作成
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreateGeometryShader(gsBlob.data(), gsBlob.size(), nullptr, m_geometryShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	インプットレイアウトの作成
	device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		vsBlob.data(),
		vsBlob.size(),
		m_inputLayout.GetAddressOf()
	);

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

