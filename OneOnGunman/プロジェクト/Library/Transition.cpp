/*/
 *  ファイル名		：	Transition.cpp
 *  概要			：	画面遷移を行う
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/16
/*/
#include "pch.h"
#include "Transition.h"

#include "Easing.h"
#include "Library/UserResources.h"
#include "ReadData.h"

using namespace DirectX;

#define MAX_ALPHA	(1.f)
#define MIN_ALPHA	(0.f)

//	フェードに適応するLerp関数
#define LERP_FUNCTION	(Easing::InOutQuart)

//	フェードにかかる時間のデフォルト値
#define DEFAULT_FADE_TIME	(1.f)

//	スタティック変数の初期化
std::unique_ptr<Transition> Transition::s_instance = nullptr;

Transition::Transition() :
	m_screenW(0),
	m_screenH(0),
	m_fadeIn(false),
	m_fadeOut(false),
	m_beforeFadeIn(false),
	m_beforeFadeOut(false),
	m_alpha(0.f),
	m_fadeSeconds(0.f),
	m_processTimer(0.f)
{

}

void Transition::Initialize()
{
	//	シェーダーの作成
	CreateShader();

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	画面の大きさを設定
	RECT size = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_screenW = size.right;
	m_screenH = size.bottom;

	//	初期値設定
	m_alpha = MAX_ALPHA;
	m_fadeSeconds = DEFAULT_FADE_TIME;
	m_processTimer = 0.f;
}

void Transition::Update()
{
	float elapsedTime = float(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_beforeFadeIn = m_fadeIn;
	m_beforeFadeOut = m_fadeOut;

	FadeInUpdate(elapsedTime);
	FadeOutUpdate(elapsedTime);
}

void Transition::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-1.f, 1.f,0.f) ,SimpleMath::Vector2(0.f, 1.f)),	//	左上
		VertexPositionTexture(SimpleMath::Vector3( 1.f, 1.f,0.f) ,SimpleMath::Vector2(1.f, 1.f)),	//	右上
		VertexPositionTexture(SimpleMath::Vector3( 1.f,-1.f,0.f) ,SimpleMath::Vector2(1.f, 0.f)),	//	右下
		VertexPositionTexture(SimpleMath::Vector3(-1.f,-1.f,0.f) ,SimpleMath::Vector2(0.f, 0.f)),	//	左下
	};

	//	シェーダーに渡すバッファを作成する
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector2(static_cast<float>(m_screenW), static_cast<float>(m_screenH));
	cbuff.ratio = SimpleMath::Vector2(m_alpha, 0.f);

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

	// カリングは左周り
	context->RSSetState(states->CullNone());

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

void Transition::Finalize()
{
}

void Transition::StartFadeIn()
{
	//	フェードアウト実行中に処理できないようにする
	if (m_fadeOut) return;

	m_processTimer = 0.f;
	m_fadeIn = true;
}

void Transition::StartFadeOut()
{
	//	フェードイン実行中に処理できないようにする
	if (m_fadeIn) return;

	m_processTimer = 0.f;
	m_fadeOut = true;
}

void Transition::CreateInstance()
{
	//	生成されていなければインスタンスを生成
	if (s_instance == nullptr)
	{
		s_instance.reset(new Transition());
	}
}

//	インスタンス取得
Transition* const Transition::GetInstance()
{
	return s_instance.get();
}

void Transition::FadeInUpdate(float elapsedTime)
{
	//	フェードイン実行中でなければ処理しない
	if (!m_fadeIn) return;

	//	タイマーを更新
	m_processTimer += elapsedTime;

	//	割合を計算
	float t = m_processTimer / m_fadeSeconds;

	//	処理時間を過ぎたら、フェードインを終了
	if (m_processTimer > m_fadeSeconds)
	{
		m_alpha = MIN_ALPHA;
		m_fadeIn = false;
	}

	//	Lerpで透明度を変更
	m_alpha = Easing::Lerp(MAX_ALPHA, MIN_ALPHA, t, LERP_FUNCTION);
}

void Transition::FadeOutUpdate(float elapsedTime)
{
	//	フェードアウト実行中でなければ処理しない
	if (!m_fadeOut) return;

	//	タイマーを更新
	m_processTimer += elapsedTime;

	//	割合を計算
	float t = m_processTimer / m_fadeSeconds;

	//	処理時間を過ぎたら、フェードインを終了
	if (m_processTimer > m_fadeSeconds)
	{
		m_alpha = MIN_ALPHA;
		m_fadeOut = false;
	}

	//	Lerpで透明度を変更
	m_alpha = Easing::Lerp(MIN_ALPHA, MAX_ALPHA, t, LERP_FUNCTION);
}

void Transition::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	シェーダーファイル読み込み
	std::vector<uint8_t> vsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/Transition/TransitionVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/Transition/TransitionPS.cso");

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
}
