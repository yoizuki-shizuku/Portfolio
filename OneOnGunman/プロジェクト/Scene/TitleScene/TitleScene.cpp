/*/
 *  ファイル名		：	TitleScene.cpp
 *  概要			：	タイトルシーンに処理するものを集めたクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/12
/*/
#include "pch.h"
#include "TitleScene.h"
#include "../StageSelectScene/StageSelectScene.h"
#include "../OptionScene/OptionScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "../Audio/AudioManager.h"

TitleScene::TitleScene() :
	Scene()
{
}

void TitleScene::Initialize()
{
	//	カメラの生成と初期設定
	m_camera = std::make_unique<Camera>();
	m_camera->SetEye(SimpleMath::Vector3(0.f, -8.f, -10.f));
	m_camera->SetTarget(SimpleMath::Vector3::Zero);
	m_camera->Initialize();

	m_skySphere = std::make_unique<SkySphere>();
	m_skySphere->Initialize();

	//	画面の大きさ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	タイトルロゴ
	m_logo = std::make_unique<GlitchEffect>();
	m_logo->LoadTexture(L"Resources/Texture/Title/Logo.png");
	m_logo->SetAnchor(Anchor::MIDDLE_CENTER);
	m_logo->SetPosition(SimpleMath::Vector2(width / 2.f, height / 3.6f));
	m_logo->SetScale(SimpleMath::Vector2::One * (static_cast<float>(rect.right) / 1280.f));

	//	クラスの作成と初期化
	m_pleaseLeftClick = std::make_unique<PleaseLeftClick>();
	m_pleaseLeftClick->Initialize();
	m_buttonManager = std::make_unique<TitleButtonManager>();
	m_buttonManager->Initialize();

	//	BGMの再生
	AudioManager::GetInstance()->StartBGM(CRI_BGM_遊泳メトロポリス);
}

void TitleScene::Update()
{
	m_logo->Update();

	//	左クリックされるまで他の操作を受けつかない
	if (!m_pleaseLeftClick->Update()) return;

	//	UIのボタンの入力を検知
	int button = m_buttonManager->Update();

	//	まだ入力されていなければ処理しない
	if (button == TitleButtonManager::Process::MAINTAIN) return;

	//	入力があれば対応する操作を行う
	switch (button)
	{
	case TitleButtonManager::Process::START:
		AudioManager::GetInstance()->StopBGM();
		ChangeScene<StageSelectScene>(0);
		break;

	case TitleButtonManager::Process::OPTION:
		ChangeScene<OptionScene>();
		break;

	case TitleButtonManager::Process::CREDIT:
		break;

	case TitleButtonManager::Process::EXIT:
		ExitGame();
		break;

	default:
		break;
	}
}

void TitleScene::Render()
{
	//	カメラのビューを更新
	m_camera->CreateView();

	SimpleMath::Matrix view = m_camera->GetView();
	//	画面サイズ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	SimpleMath::Matrix proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		1.f,
		120.f
	);

	m_skySphere->Render(view, proj);

	//	テクスチャ表示
	m_logo->Render();

	m_pleaseLeftClick->Render();
	m_buttonManager->Render();
}

void TitleScene::Finalize()
{
	m_pleaseLeftClick->Finalize();
	m_buttonManager->Finalize();
}
