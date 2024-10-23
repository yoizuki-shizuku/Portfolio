/*/
 *  ファイル名		：	LandingEffect.h
 *  概要			：	弾の着弾時のエフェクト
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/12
/*/
#include "pch.h"
#include "LandingEffect.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Easing.h"
#include "Library/Useful.h"
#include "WICTextureLoader.h"
#include <random>

//	アニメーションの時間
#define ANIMATION_TIME		(2.f)

LandingEffect::LandingEffect() :
	m_timer(0.f),
	m_alpha(0.f),
	m_isAnimation(false)
{
}

void LandingEffect::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	生成と初期化
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	m_effect = std::make_unique<AlphaTestEffect>(device);

	//	画像データ読み込み
	DirectX::CreateWICTextureFromFile(device, L"Resources/Texture/Play/Particle.png", nullptr, m_texture.ReleaseAndGetAddressOf());

	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(device, m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

void LandingEffect::Update(float elapsedTime)
{
	//	アニメーションしていなければ処理しない
	if (!m_isAnimation) return;

	//	タイマーの更新
	m_timer += elapsedTime;

	//	パーティクルの更新
	for (auto& it : m_particles)
	{
		//	設定された方向へ移動させる
		it.position += it.velocity * elapsedTime * Easing::Lerp(6.f, 0.f, Useful::Ratio01(m_timer, ANIMATION_TIME),Easing::OutQuint);
	}

	//	透明度の変更
	m_alpha = Easing::Lerp(1.f, 0.f, Useful::Ratio01(m_timer, ANIMATION_TIME));

	//	アニメーションの時間を超えたら止める
	if (m_timer >= ANIMATION_TIME)
	{
		m_isAnimation = false;
		m_timer = 0.f;
	}
}

void LandingEffect::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	アニメーションしていなければ処理しない
	if (!m_isAnimation) return;

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	描画で共通している部分を先に設定
	m_effect->SetView(view);
	m_effect->SetProjection(proj);
	m_effect->SetTexture(m_texture.Get());

	//	カメラの位置を求める
	SimpleMath::Vector3 eye = ClacCameraPos(view);

	SimpleMath::Vector4 color = SimpleMath::Vector4(m_color.x, m_color.y, m_color.z, m_alpha);
	m_effect->SetColorAndAlpha(color);
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateBillboard(m_startPos, eye, SimpleMath::Vector3::Up);
	m_effect->SetWorld(world);
	m_effect->Apply(context);

	//	半透明描画指定
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFF);

	//	カリングは左周り
	context->RSSetState(states->CullClockwise());

	//	深度バッファの設定
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	//	インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	//	各パーティクルの描画
	for (const auto& it : m_particles)
	{
			//	デフォルトの頂点位置
		VertexPositionTexture vertex[4] =
		{
			VertexPositionTexture(SimpleMath::Vector3(-0.15f,  0.15f, 0.f), SimpleMath::Vector2(0.f, 0.f)),	//	左上
			VertexPositionTexture(SimpleMath::Vector3( 0.15f,  0.15f, 0.f), SimpleMath::Vector2(1.f, 0.f)),	//	右上
			VertexPositionTexture(SimpleMath::Vector3( 0.15f, -0.15f, 0.f), SimpleMath::Vector2(1.f, 1.f)),	//	右下
			VertexPositionTexture(SimpleMath::Vector3(-0.15f, -0.15f, 0.f), SimpleMath::Vector2(0.f, 1.f)),	//	左下
		};

		for (int i = 0; i < 4; i++)
		{
			vertex[i].position.x += it.position.x;
			vertex[i].position.y += it.position.y;
			vertex[i].position.z += it.position.z;
		}

		//	描画
		m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	}

	m_batch->End();
}

void LandingEffect::Finalize()
{
	m_texture.Reset();
}

void LandingEffect::StartEffect(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 color)
{
	//	各変数にアニメーションを開始するためのフラグを設定
	m_alpha = 1.f;
	m_color = color;
	m_isAnimation = true;
	m_startPos = startPos;

	for (auto& it : m_particles)
	{
		it.position = SimpleMath::Vector3::Zero;

		//	方向ベクトルをランダムに設定
		SimpleMath::Vector3 velocity = SimpleMath::Vector3::Zero;

		//	乱数を生成
		std::random_device seed;
		std::mt19937	   rand{ seed() };
		std::uniform_real_distribution<> distY(-1.0, 1.0);

		//	設定
		velocity.x = static_cast<float>(distY(rand));
		velocity.y = static_cast<float>(distY(rand));
		velocity.z = static_cast<float>(distY(rand));

		//	正規化
		velocity.Normalize();

		//	速度もランダムに
		std::uniform_real_distribution<> speed(-2.0, 2.0);
		it.velocity = velocity * static_cast<float>(speed(rand));
	}
}

void LandingEffect::StopEffect()
{
	m_isAnimation = false;
	m_timer = 0.f;
	m_alpha = 0.f;
}

SimpleMath::Vector3 LandingEffect::ClacCameraPos(const SimpleMath::Matrix& view)
{
	SimpleMath::Vector3 m1(view._11, view._12, view._13);
	SimpleMath::Vector3 m2(view._21, view._22, view._23);
	SimpleMath::Vector3 m3(view._31, view._32, view._33);
	SimpleMath::Vector3 m4(view._41, view._42, view._43);

	float tx = -m1.Dot(m4);
	float ty = -m2.Dot(m4);
	float tz = -m3.Dot(m4);

	return DirectX::SimpleMath::Vector3(tx, ty, tz);
}
