/*/
 *  ファイル名		：	TorpedoBullet.cpp
 *  概要			：	魚雷のように徐々に加速していく弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/20
/*/
#include "pch.h"
#include "TorpedoBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

//	弾の生存時間
#define ALIVE_TIME	(3.f)
//	最大速度になるまでにかかる時間
#define MAX_TIME	(0.8f)

TorpedoBullet::TorpedoBullet(SimpleMath::Vector3 position, SimpleMath::Vector3 maxVelocity) :
	EnemyBullet(),
	m_maxVelocity(maxVelocity),
	m_nowVelocity(SimpleMath::Vector3::Zero),
	m_timer(0.f)
{
	SetCollider(new Sphere(0.3f));

	SetPosition(position);
}

void TorpedoBullet::Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	m_model = GeometricPrimitive::CreateSphere(context, 0.6f);

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

bool TorpedoBullet::Update(float elapsedTime)
{
	//	タイマーの更新
	m_timer += elapsedTime;

	//	速度を徐々に上げる
	float ratio = Useful::Ratio01(m_timer, MAX_TIME);
	float t = Easing::InQuint(ratio);
	m_nowVelocity = SimpleMath::Vector3::Lerp(SimpleMath::Vector3::Zero, m_maxVelocity, t);

	//	位置の更新
	SimpleMath::Vector3 delta = GetPosition() + (m_nowVelocity * elapsedTime);
	SetPosition(delta);

	//	生存時間を過ぎたら消去
	if (m_timer > ALIVE_TIME) return false;

	//	他のオブジェクトに当たったら消去
	if (GetCollider()->GetIsHit()) return false;

	return true;
}

void TorpedoBullet::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	タイマーが0以上で描画する
	if (m_timer < 0.f) return;

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::Orange, nullptr, false, [&] {
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

void TorpedoBullet::Finalize()
{
}
