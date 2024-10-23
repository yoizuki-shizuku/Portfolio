/*/
 *  ファイル名		：	InfinitePlane.h
 *  概要			：	背景用の平面
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/24
/*/
#include "pch.h"
#include "InfinitePlane.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "ReadData.h"

InfinitePlane::InfinitePlane() :
	m_timer (0.f)
{
	//	プリミティブバッチの作成
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	CreateShader();
}


void InfinitePlane::Update()
{
	m_timer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
}

void InfinitePlane::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	シェーダーに渡すバッファを作成する
	ConstBuffer cbuff{};
	cbuff.view = view.Transpose();
	cbuff.proj = proj.Transpose();
	cbuff.time = SimpleMath::Vector4(m_timer, 0.f, 0.f, 0.f);

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateScale(10.f, 1.f, 3.f);
	cbuff.world = world.Transpose();

	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	半透明描画指定
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);

	//	カリングは左周り
	context->RSSetState(states->CullNone());

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//	シェーダーをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	板ポリゴンを描画
	m_batch->Begin();

	for (int i = 0; i < 2; i++)
	{
		VertexPositionTexture vertex[4] =
		{
			VertexPositionTexture(SimpleMath::Vector3( 1.f, 0.f,  1.f), SimpleMath::Vector2(0.f, 0.f)),	//	左上
			VertexPositionTexture(SimpleMath::Vector3(-1.f, 0.f,  1.f), SimpleMath::Vector2(1.f, 0.f)),	//	右上
			VertexPositionTexture(SimpleMath::Vector3(-1.f, 0.f, -1.f), SimpleMath::Vector2(1.f, 1.f)),	//	右下
			VertexPositionTexture(SimpleMath::Vector3( 1.f, 0.f, -1.f), SimpleMath::Vector2(0.f, 1.f)),	//	左下
		};

		for (int j = 0; j < 4; j++)
		{
			vertex[j].position.y += 2.f * i;
		}

		m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	}

	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3( 2.f,  3.f, 3.6f), SimpleMath::Vector2(0.f, 0.f)),	//	左上
		VertexPositionTexture(SimpleMath::Vector3(-2.f,  3.f, 3.6f), SimpleMath::Vector2(1.f, 0.f)),	//	右上
		VertexPositionTexture(SimpleMath::Vector3(-2.f, -1.f, 3.6f), SimpleMath::Vector2(1.f, 1.f)),	//	右下
		VertexPositionTexture(SimpleMath::Vector3( 2.f, -1.f, 3.6f), SimpleMath::Vector2(0.f, 1.f)),	//	左下
	};

	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	m_batch->End();

	//	シェーダーの登録を解除
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void InfinitePlane::Finalize()
{

}

void InfinitePlane::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, gsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/InfinitePlane/InfinitePlaneVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/InfinitePlane/InfinitePlanePS.cso");

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
