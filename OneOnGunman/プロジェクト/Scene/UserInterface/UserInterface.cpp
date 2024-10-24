/*/
 *  ファイル名		：	UserInterface.cpp
 *  概要			：	UIを表示するためのクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/02
/*/
#include "pch.h"
#include "UserInterface.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "WICTextureLoader.h"
#include "ReadData.h"

UserInterface::UserInterface() :
	m_radian(0.f),
	m_alpha(1.f),
	m_textureWidth(0),
	m_textureHeight(0),
	m_ratio(1.f)
{
	//	変数初期化
	m_position = SimpleMath::Vector2::Zero;
	m_scale = SimpleMath::Vector2::One;
	m_anchor = Anchor::TOP_LEFT;
	RECT windowSize = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_windowWidth = windowSize.right;
	m_windowHeight = windowSize.bottom;
	m_direction = Direction::Right;
	m_color = SimpleMath::Vector3(1.f);

	//	プリミティブバッチの作成
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(context);

	//	シェーダーの作成
	CreateShader();
}

void UserInterface::LoadTexture(const wchar_t* path)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	画像の大きさを読み込むためのリソース
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	//	画像データ読み込み
	DirectX::CreateWICTextureFromFile(device, path, resource.ReleaseAndGetAddressOf(), m_texture.ReleaseAndGetAddressOf());

	//	読み込んだ画像データから縦と横のサイズのデータを読み出す
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(resource.As(&tex));
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
}

void UserInterface::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	頂点情報
	//	Position.xy : 位置
	//	Position.z  : 角度
	//	Color.xy    : 拡縮
	//	Color.z	    : アンカーポイント
	//	Color.w     : 透明度
	//	Tex.xy		: 画像のサイズ
	VertexPositionColorTexture vertex =
	{
		SimpleMath::Vector3(m_position.x, m_position.y, m_radian),
		SimpleMath::Vector4(m_scale.x, m_scale.y, static_cast<float>(m_anchor), m_alpha),
		SimpleMath::Vector2(static_cast<float>(m_textureWidth),static_cast<float>(m_textureHeight))
	};

	//	シェーダーに渡すバッファを作成する
	ConstBuffer cbuff{};
	cbuff.windowWidth = static_cast<float>(m_windowWidth);
	cbuff.windowHeight = static_cast<float>(m_windowHeight);
	cbuff.ratio = m_ratio;
	cbuff.direction = static_cast<int>(m_direction);
	cbuff.color = SimpleMath::Vector4(m_color.x, m_color.y, m_color.z, 1.f);

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
	context->RSSetState(states->CullClockwise());

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

void UserInterface::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/UserInterface/UserInterfaceVS.cso");
	gsBlob = DX::ReadData(L"Resources/CSO/UserInterface/UserInterfaceGS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/UserInterface/UserInterfacePS.cso");

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
		DirectX::VertexPositionColorTexture::InputElements,
		DirectX::VertexPositionColorTexture::InputElementCount,
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
