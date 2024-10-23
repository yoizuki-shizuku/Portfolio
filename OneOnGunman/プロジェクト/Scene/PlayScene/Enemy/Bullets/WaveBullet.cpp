/*/
 *  ファイル名		：	WaveBullet.cpp
 *  概要			：	波のように動く弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/01/06
/*/
#include "pch.h"
#include "WaveBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"

//	弾の生存時間
#define ALIVE_TIME	(5.f)

WaveBullet::WaveBullet(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 velocity) :
	EnemyBullet(),
	m_velocity(velocity),
	m_timer(0.f)
{
	//	当たり判定を設定
	SetCollider(new Sphere(0.4f));

	SetPosition(position);

	//	垂直ベクトルを計算
	m_vertical = SimpleMath::Vector3(-velocity.z, 0.f, velocity.x);
	m_vertical.Normalize();
}

void WaveBullet::Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	m_model = GeometricPrimitive::CreateSphere(context, 0.8f);
	m_beforePos = GetPosition();

	m_inputLayout = input;
	m_vertexShader = vertex;
	m_pixelShader = pixel;

	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//シェーダーにデータを渡すための定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

bool WaveBullet::Update(float elapsedTime)
{
	//	位置の更新
	m_beforePos = m_beforePos + (m_velocity * elapsedTime);
	SetPosition(m_beforePos + m_vertical * sinf(m_timer * 8.f));

	//	タイマーを更新し、生存時間を過ぎたら消去
	m_timer += elapsedTime;
	if (m_timer > ALIVE_TIME) return false;

	//	他のオブジェクトに当たったら消去
	if (GetCollider()->GetIsHit()) return false;

	return true;
}

void WaveBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::Red, nullptr, false, [&] {

		ConstBuffer cbuff;
		cbuff.position = SimpleMath::Vector4(GetPosition().x, GetPosition().y, GetPosition().z, 1.f);
		cbuff.color = SimpleMath::Vector4(GetColor().x, GetColor().y, GetColor().z, 1.f);
		//	受け渡し用バッファの内容更新
		context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

		//	シェーダーにバッファを渡す
		ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
		context->VSSetConstantBuffers(1, 1, cb);
		context->PSSetConstantBuffers(1, 1, cb);

		//	半透明描画指定
		context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFF);
		//	カリングは左周り
		context->RSSetState(states->CullCounterClockwise());
		//	深度バッファの設定
		context->OMSetDepthStencilState(states->DepthDefault(), 0);

		context->IASetInputLayout(m_inputLayout.Get());
		context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		});

	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void WaveBullet::Finalize()
{
}
