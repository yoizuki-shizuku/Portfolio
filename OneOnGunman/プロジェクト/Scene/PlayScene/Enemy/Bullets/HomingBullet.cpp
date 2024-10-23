/*/
 *  ファイル名		：	HomingBullet.cpp
 *  概要			：	プレイヤーを追尾する弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/21
/*/
#include "pch.h"
#include "HomingBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"

//	弾の生存時間
#define ALIVE_TIME	(3.f)

HomingBullet::HomingBullet(SimpleMath::Vector3 position, Player* target, float speed) :
	EnemyBullet(),
	m_target(target),
	m_speed(speed),
	m_timer(0.f)
{
	SetCollider(new Sphere(0.4f));

	SetPosition(position);

	m_direction = ToPlayerVector();
}

void HomingBullet::Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	m_model = GeometricPrimitive::CreateSphere(context, 0.8f);

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

bool HomingBullet::Update(float elapsedTime)
{
	//	タイマーの更新
	m_timer += elapsedTime;

	//	ターゲットへの方向ベクトルを取得
	SimpleMath::Vector3 toPlayerVec = ToPlayerVector();
	//	プレイヤーへの方向が進行方向に対して許容範囲なら追尾する
	if (m_direction.Dot(toPlayerVec) >= 0.997f)
	{
		m_direction += toPlayerVec - m_direction * 0.1f * elapsedTime;
		m_direction.Normalize();
	}

	//	位置の更新
	SimpleMath::Vector3 delta = GetPosition() + (m_direction * m_speed * elapsedTime);
	SetPosition(delta);

	//	生存時間を過ぎたら消去
	if (m_timer > ALIVE_TIME) return false;

	//	他のオブジェクトに当たったら消去
	if (GetCollider()->GetIsHit()) return false;

	return true;
}

void HomingBullet::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::Yellow, nullptr, false, [&] {
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

void HomingBullet::Finalize()
{
}

SimpleMath::Vector3 HomingBullet::ToPlayerVector()
{
	SimpleMath::Vector3 ret = m_target->GetCenter() - GetPosition();
	ret.Normalize();
	
	return ret;
}
