/*/
 *  ファイル名		：	EnemyBulletManager.cpp
 *  概要			：	敵の弾を一括管理するクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/25
/*/
#include "pch.h"
#include "EnemyBulletManager.h"

using namespace DirectX;

#include "ReadData.h"
#include "Library/UserResources.h"

EnemyBulletManager::EnemyBulletManager()
{
}

void EnemyBulletManager::Initialize()
{
	m_bullets.clear();

	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, psBlob;
	vsBlob = DX::ReadData(L"Resources/CSO/BulletEffect/EnemyBulletVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/BulletEffect/EnemyBulletPS.cso");

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	//	インプットレイアウトの作成
	device->CreateInputLayout(
		DirectX::VertexPositionNormalTexture::InputElements,
		DirectX::VertexPositionNormalTexture::InputElementCount,
		vsBlob.data(),
		vsBlob.size(),
		m_inputLayout.GetAddressOf()
	);

	m_landingEffect = std::make_unique<LandingEffect>();
	m_landingEffect->Initialize();
}

void EnemyBulletManager::Update(float elapsedTime)
{
	//	各弾のUpdate関数を処理する
	for (std::vector<std::unique_ptr<EnemyBullet>>::iterator it = m_bullets.begin(); it != m_bullets.end();)
	{
		//	戻り値がfalseなら弾を消去する
		if (it->get()->Update(elapsedTime))
		{
			++it;
			continue;
		}

		it->get()->Finalize();
		it->reset();
		it = m_bullets.erase(it);
	}

	//	エフェクトの更新処理
	m_landingEffect->Update(elapsedTime);
}

void EnemyBulletManager::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	描画
	for (auto& it : m_bullets)
	{
		it->Render(view, proj);
	}

	m_landingEffect->Render(view, proj);
}

void EnemyBulletManager::Finalize()
{
	auto it = m_bullets.begin();

	//	後処理
	while (m_bullets.size() != 0)
	{
		it->get()->Finalize();
		it->reset();
		it = m_bullets.erase(it);
	}

	m_landingEffect->Finalize();
}

void EnemyBulletManager::AddBullet(std::unique_ptr<EnemyBullet> bullet)
{
	//	初期化処理を行ってから配列に追加する
	bullet->Initialize(m_inputLayout.Get(), m_vertexShader.Get(), m_pixelShader.Get());
	m_bullets.emplace_back(std::move(bullet));
}

bool EnemyBulletManager::CheckHitBullet(Collider* col, Collider* dodge)
{
	//	ジャスト回避の判定用フラグ
	bool justDodge = false;

	for (auto& it : m_bullets)
	{
		//	コライダークラスの取得
		Collider* bCol = it->GetCollider();

		//	ジャスト回避の判定を取る
		//	プレイヤーの当たり判定はこの境界球に含まれている
		//	まずこれに当たっていなければカプセルとの判定はしない
		if (!dodge->CheckHit(bCol)) continue;

		//	1つでもジャスト回避の境界球に当たっている
		justDodge = true;

		//	当たり判定を取る
		bool hit = bCol->CheckHit(col);

		//	一つでもあたっていれば、他は処理しない
		if (hit)
		{
			//	着弾エフェクトを表示
			SimpleMath::Vector3 hitPos = it->GetPosition();
			SimpleMath::Vector3 bulletColor = it->GetColor();
			m_landingEffect->StartEffect(hitPos, bulletColor);

			return true;
		}
	}

	//	このフレームで境界球が接触していたかどうか
	dodge->SetIsHit(justDodge);

	//	どの弾とも当たっていない
	return false;
}

void EnemyBulletManager::RetryGame()
{
	m_bullets.clear();
	m_landingEffect->StopEffect();
}
