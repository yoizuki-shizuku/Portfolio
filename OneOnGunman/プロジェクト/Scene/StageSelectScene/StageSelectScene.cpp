/*/
 *  ファイル名		：	StageSelectScene.cpp
 *  概要			：	ステージ選択をするシーン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
/*/
#include "pch.h"
#include "StageSelectScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Scene/TitleScene/TitleScene.h"
#include "../PlayScene/PlayScene.h"
#include "Library/Input.h"
#include "ReadData.h"
#include "../Audio/AudioManager.h"

StageSelectScene::StageSelectScene(int stageIndex) :
	Scene(),
	m_stageIndex(stageIndex),
	m_timer(0.f)
{
}

void StageSelectScene::Initialize()
{
	//	カメラの生成と初期設定
	m_camera = std::make_unique<Camera>();
	m_camera->SetEye(SimpleMath::Vector3(0.f, 1.f, -5.f));
	m_camera->SetTarget(SimpleMath::Vector3(0.f, 1.f, 0.f));
	m_camera->Initialize();

	m_hologram = std::make_unique<HologramManager>();
	m_hologram->Initialize(m_stageIndex);

	m_plane = std::make_unique<InfinitePlane>();

	m_infomation = std::make_unique<Information>();
	m_infomation->Initialize();

	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_stageSelect = std::make_unique<UserInterface>();
	m_stageSelect->LoadTexture(L"Resources/Texture/StageSelect/StageSelect.png");
	m_stageSelect->SetAnchor(Anchor::MIDDLE_CENTER);
	m_stageSelect->SetPosition(SimpleMath::Vector2(width / 2.f, height * 0.15f));
	m_stageSelect->SetScale(SimpleMath::Vector2::One * (width / 1280.f));
	
	m_start = std::make_unique<UserInterface>();
	m_start->LoadTexture(L"Resources/Texture/StageSelect/Start.png");
	m_start->SetAnchor(Anchor::MIDDLE_CENTER);
	m_start->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.85f));
	m_start->SetScale(SimpleMath::Vector2::One * 0.65f);
	m_startButton = std::make_unique<Button>(m_start.get());
	m_startAnimation = std::make_unique<Animator>(m_start.get());
	m_startAnimation->LoadFile("Resources/UIAnimation/StageSelect/ButtonSelect.json");

	m_title = std::make_unique<UserInterface>();
	m_title->LoadTexture(L"Resources/Texture/StageSelect/Title.png");
	m_title->SetAnchor(Anchor::MIDDLE_CENTER);
	m_title->SetPosition(SimpleMath::Vector2(width * 0.1f, height * 0.1f));
	m_title->SetScale(SimpleMath::Vector2::One * 0.5f);
	m_titleButton = std::make_unique<Button>(m_title.get());
	m_titleAnimation = std::make_unique<Animator>(m_title.get());
	m_titleAnimation->LoadFile("Resources/UIAnimation/StageSelect/ButtonSelect.json");

	//  シェーダー読み込み
	std::vector<uint8_t> psBlob;
	psBlob = DX::ReadData(L"Resources/CSO/PostProcess/CathodeRayTube.cso");
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_postProcess.ReleaseAndGetAddressOf())
	);

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	//	BGMの再生
	AudioManager::GetInstance()->StartBGM(CRI_BGM_IYA_IYA_UNCONTROLLABLE);
}

void StageSelectScene::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_hologram->Update();
	m_plane->Update();
	m_startButton->Update();
	m_titleButton->Update();
	m_startAnimation->Update(elapsedTime);
	m_titleAnimation->Update(elapsedTime);
	m_infomation->Update(m_hologram->GetNowChoice());

	m_timer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	効果音
	if (m_startButton->OnMouse()) AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);
	
	if (m_titleButton->OnMouse()) AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);

	//	選択時アニメーション
	if (m_startButton->StayMouse()) m_startAnimation->PlayAnimation();
	else m_startAnimation->ResetAnimation();

	if (m_titleButton->StayMouse()) m_titleAnimation->PlayAnimation();
	else m_titleAnimation->ResetAnimation();

	//	シーン移動
	if (m_startButton->IsPresed())
	{
		AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
		AudioManager::GetInstance()->StopBGM();
		ChangeScene<PlayScene>(m_hologram->GetNowChoice());
	}
	if (m_titleButton->IsPresed())
	{
		AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
		AudioManager::GetInstance()->StopBGM();
		ChangeScene<TitleScene>();
	}
}

void StageSelectScene::Render()
{
	//	カメラのビューを更新
	m_camera->CreateView();

	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	m_plane->Render(view, proj);
	m_hologram->Render(view, proj);
	m_stageSelect->Render();
	m_infomation->Render();
	m_start->Render();
	m_title->Render();

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	ConstBuffer cbuff;
	cbuff.Time = SimpleMath::Vector4(m_timer, 0.f, 0.f, 0.f);
	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
}

void StageSelectScene::Finalize()
{
}
