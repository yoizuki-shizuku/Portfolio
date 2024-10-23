/*/
 *  ファイル名		：	PleasePressAnyKey.cpp
 *  概要			：	始めにキーが押されるまでUIを表示し、そのキーの押下を検知する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/08
/*/
#include "pch.h"
#include "PleaseLeftClick.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/Audio/AudioManager.h"

PleaseLeftClick::PleaseLeftClick() :
	m_isLeftClick(false)
{
}

void PleaseLeftClick::Initialize()
{
	//	画面の大きさ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	テクスチャ作成
	m_texture = std::make_unique<UserInterface>();
	m_texture->LoadTexture(L"Resources/Texture/Title/PleaseLeftClick.png");
	m_texture->SetScale(SimpleMath::Vector2::One * 0.8f * (width / 1280.f));
	m_texture->SetPosition(SimpleMath::Vector2(width / 2.f, height / 1.38f));
	m_texture->SetAnchor(Anchor::MIDDLE_CENTER);
	m_texture->SetAlpha(0.f);

	//	アニメーション作成
	m_fadeIn = std::make_unique<Animator>(m_texture.get());
	m_fadeIn->LoadFile("Resources/UIAnimation/Title/LeftClickFadeIn.json");
	m_fadeIn->StartAnimation();
}

bool PleaseLeftClick::Update()
{
	//	一連のアニメーションが終了したら、次の画面へ
	if (m_fadeIn->GetFinishAnimation() && m_isLeftClick) return true;

	//	経過時間計測
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	アニメーションの更新
	m_fadeIn->Update(elapsedTime);

	//	PleaseLeftClickの表示ができるまで進めないようにする
	if (m_fadeIn->GetIsAnimation()) return false;

	//	クリック検知
	auto msTracker = Input::GetInstace()->GetMouseTracker();
	if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
		m_fadeIn->StartReverseAnimation();
		m_isLeftClick = true;
	}

	return false;
}

void PleaseLeftClick::Render()
{
	m_texture->Render();
}

void PleaseLeftClick::Finalize()
{
	m_fadeIn->Finalize();
}
