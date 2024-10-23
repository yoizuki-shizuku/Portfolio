/*/
 *  ファイル名		：	PlayerBullet.cpp
 *  概要			：	プレイヤーの弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#include "pch.h"
#include "PlayerBullet.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "ReadData.h"

//	弾のスピード
#define SPEED	(30.f)
//	弾の有効時間（秒）
#define VALID_TIME	(5.f)

PlayerBullet::PlayerBullet() :
	m_isValid(false),
	m_validTimer(0.f)
{
}

void PlayerBullet::Initialize()
{
	m_collider = std::make_unique<Sphere>(0.25f);

	m_model = std::make_unique<BulletAppearance>();
	m_model->Initialize();

	m_landingEffect = std::make_unique<LandingEffect>();
	m_landingEffect->Initialize();
}

void PlayerBullet::Update(float elapsedTime)
{
	m_landingEffect->Update(elapsedTime);

	//	時間を計測
	m_validTimer += elapsedTime;

	//	指定の時間を超えていたら弾を消す
	if (m_validTimer > VALID_TIME) m_isValid = false;

	//	当たり判定の有効フラグを更新
	m_collider->SetIsValid(m_isValid);

	//	他のオブジェクトに接触したら弾を消す
	if (m_collider->GetIsHit())
	{
		//	エフェクトの再生
		if (m_isValid) m_landingEffect->StartEffect(m_position, SimpleMath::Vector3(0.f, 0.f, 1.f));
	}

	//	弾が有効な状態でなければ、処理しない
	if (!m_isValid) return;

	//	弾を移動させる
	m_position += m_velocity * SPEED * elapsedTime;

	//	モデルを移動
	m_model->Update(elapsedTime, m_position);

	//	当たり判定を追従
	m_collider->SetPosition(m_position);
}

void PlayerBullet::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	m_landingEffect->Render(view, proj);

	//	有効な状態でなければ処理しない
	if (!m_isValid) return;

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(m_position);

	m_model->Render(view, proj);
}

void PlayerBullet::Finalize()
{
	m_model->Finalize();
	m_landingEffect->Finalize();
}

void PlayerBullet::RetryGame()
{
	m_landingEffect->StopEffect();
}

void PlayerBullet::CreateBullet(DirectX::SimpleMath::Vector3 createPos, DirectX::SimpleMath::Vector3 moveAngle)
{
	m_position = createPos;
	m_velocity = moveAngle;
	m_firstVel = moveAngle;
	m_isValid = true;
	m_validTimer = 0.f;
	m_model->CreateBullet(moveAngle);
}



BulletAppearance::BulletAppearance() :
	m_timer(0.f),
	m_radian(0.f)
{
}

void BulletAppearance::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//シェーダーにデータを渡すための定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_luminous = std::make_unique<BulletEffect>(0.4f, L"Resources/CSO/BulletEffect/BulletEffectVS.cso", L"Resources/CSO/BulletEffect/LuminousPS.cso");
	m_thunder = std::make_unique<BulletEffect>(0.4f, L"Resources/CSO/BulletEffect/BulletEffectVS.cso", L"Resources/CSO/BulletEffect/ThuderPS.cso");
	m_aura = std::make_unique<BulletEffect>(0.5f, L"Resources/CSO/BulletEffect/AuraVS.cso", L"Resources/CSO/BulletEffect/AuraPS.cso");
}

void BulletAppearance::Update(float elapsedTime, DirectX::SimpleMath::Vector3 position)
{
	m_timer += elapsedTime;
	m_position = position;
}

void BulletAppearance::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateRotationY(m_radian);
	world *= rotate;
	world *= SimpleMath::Matrix::CreateTranslation(m_position);

	ConstBuffer cbuff;
	cbuff.position = SimpleMath::Vector3(m_position.x, m_position.y, m_position.z);
	cbuff.time = m_timer;
	cbuff.rotate = rotate.Transpose();
	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	各エフェクトを描画
	m_luminous->Render(world, view, proj, m_CBuffer.Get());
	m_thunder->Render(world, view, proj, m_CBuffer.Get());
	m_aura->Render(world, view, proj, m_CBuffer.Get());
}

void BulletAppearance::Finalize()
{
}

void BulletAppearance::CreateBullet(SimpleMath::Vector3 velocity)
{
	m_timer = 0.f;
	m_radian = atan2f(velocity.x, velocity.z);
}



BulletAppearance::BulletEffect::BulletEffect(float d, const wchar_t* vertex, const wchar_t* pixel)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	m_model = GeometricPrimitive::CreateSphere(context, d);

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, psBlob;
	vsBlob = DX::ReadData(vertex);
	psBlob = DX::ReadData(pixel);

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

}

void BulletAppearance::BulletEffect::Render(const SimpleMath::Matrix& world,
											const SimpleMath::Matrix& view,
											const SimpleMath::Matrix& proj,
											ID3D11Buffer* cbuff)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	m_model->Draw(world, view, proj, Colors::White, nullptr, false, [&] {

		//	シェーダーにバッファを渡す
		ID3D11Buffer* cb[1] = { cbuff };
		context->VSSetConstantBuffers(1, 1, cb);
		context->PSSetConstantBuffers(1, 1, cb);

		//	半透明描画指定
		context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);
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

void BulletAppearance::BulletEffect::Finalize()
{
}
