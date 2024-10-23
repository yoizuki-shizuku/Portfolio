/*/
 *  ファイル名		：	OptionScene.cpp
 *  概要			：	各種オプションを行うシーン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/02/05
/*/
#include "pch.h"
#include "OptionScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "../TitleScene/TitleScene.h"
#include "ReadData.h"
#include "Scene/SaveData/SaveData.h"
#include "Scene/Audio/AudioManager.h"

OptionScene::OptionScene() :
	Scene(),
	m_timer(0.f)
{
}

void OptionScene::Initialize()
{
	//	カメラの生成と初期設定
	m_camera = std::make_unique<Camera>();
	m_camera->SetEye(SimpleMath::Vector3(0.f, 1.f, -5.f));
	m_camera->SetTarget(SimpleMath::Vector3(0.f, 1.f, 0.f));
	m_camera->Initialize();

	m_plane = std::make_unique<InfinitePlane>();

	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_option = std::make_unique<UserInterface>();
	m_option->LoadTexture(L"Resources/Texture/Title/Option.png");
	m_option->SetAnchor(Anchor::MIDDLE_CENTER);
	m_option->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.15f));
	m_option->SetScale(width / 1280.f);

	m_title = std::make_unique<UserInterface>();
	m_title->LoadTexture(L"Resources/Texture/StageSelect/Title.png");
	m_title->SetAnchor(Anchor::MIDDLE_CENTER);
	m_title->SetPosition(SimpleMath::Vector2(width * 0.1f, height * 0.1f));
	m_title->SetScale(0.5f);
	m_titleButton = std::make_unique<Button>(m_title.get());
	m_titleAnimation = std::make_unique<Animator>(m_title.get());
	m_titleAnimation->LoadFile("Resources/UIAnimation/StageSelect/ButtonSelect.json");

	//	BGM
	float heightBgm = height * 0.35f;
	m_bgm = std::make_unique<UserInterface>();
	m_bgm->LoadTexture(L"Resources/Texture/Option/BGM.png");
	m_bgm->SetPosition(SimpleMath::Vector2(width * 0.2f, heightBgm));
	m_bgm->SetAnchor(Anchor::MIDDLE_CENTER);
	m_bgm->SetScale(width / 1280.f * 0.6f);
	m_bgmVolume = std::make_unique<Volume>();
	m_bgmVolume->Initialize(SaveData::GetInstance()->GetVolumeBGM(), heightBgm);

	//	SE
	float heightSE = height * 0.5f;
	m_se = std::make_unique<UserInterface>();
	m_se->LoadTexture(L"Resources/Texture/Option/SE.png");
	m_se->SetPosition(SimpleMath::Vector2(width * 0.2f, heightSE));
	m_se->SetAnchor(Anchor::MIDDLE_CENTER);
	m_se->SetScale(width / 1280.f * 0.6f);
	m_seVolume = std::make_unique<Volume>();
	m_seVolume->Initialize(SaveData::GetInstance()->GetVolumeSE(), heightSE);

	//	セーブデータのリセット
	m_resetData = std::make_unique<UserInterface>();
	m_resetData->LoadTexture(L"Resources/Texture/Option/ResetData.png");
	m_resetData->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.8f));
	m_resetData->SetAnchor(Anchor::MIDDLE_CENTER);
	m_resetData->SetScale(width / 1280.f * 0.4f);
	m_resetDataButton = std::make_unique<Button>(m_resetData.get());

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
}

void OptionScene::Update()
{
	m_plane->Update();

	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	m_titleButton->Update();
	m_titleAnimation->Update(elapsedTime);

	m_bgmVolume->Update();
	m_seVolume->Update();

	AudioManager* audio = AudioManager::GetInstance();
	audio->SetVolumeBGM(m_bgmVolume->GetVolume());
	audio->SetVolumeSE(m_seVolume->GetVolume());

	m_resetDataButton->Update();
	if (m_resetDataButton->OnMouse())
	{
		audio->PlaySE(CRI_SE_MOVECURSOR);
	}
	if (m_resetDataButton->IsPresed())
	{
		audio->PlaySE(CRI_SE_DECIDE);
		SaveData::GetInstance()->ResetData();
	}


	if (m_titleButton->StayMouse()) m_titleAnimation->PlayAnimation();
	else m_titleAnimation->ResetAnimation();

	if (m_titleButton->OnMouse()) audio->PlaySE(CRI_SE_MOVECURSOR);

	//	シーン移動
	if (m_titleButton->IsPresed())
	{
		audio->PlaySE(CRI_SE_DECIDE);
		ChangeScene<TitleScene>();
	}
}

void OptionScene::Render()
{
	//	カメラのビューを更新
	m_camera->CreateView();
	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	m_plane->Render(view, proj);
	
	m_option->Render();
	m_title->Render();

	m_bgm->Render();
	m_bgmVolume->Render();
	m_se->Render();
	m_seVolume->Render();

	m_resetData->Render();

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	ConstBuffer cbuff;
	cbuff.Time = SimpleMath::Vector4(m_timer, 0.f, 0.f, 0.f);
	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
}

void OptionScene::Finalize()
{
	SaveData::GetInstance()->SetVolumeBGM(m_bgmVolume->GetVolume());
	SaveData::GetInstance()->SetVolumeSE(m_seVolume->GetVolume());
}
