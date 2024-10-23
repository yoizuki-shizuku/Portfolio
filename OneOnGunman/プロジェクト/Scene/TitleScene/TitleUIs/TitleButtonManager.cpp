/*/
 *  ファイル名		：	TitleButtonManager.cpp
 *  概要			：	タイトル画面でのボタンを管理する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/08
/*/
#include "pch.h"
#include "TitleButtonManager.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/Audio/AudioManager.h"

//	表示タイミング
#define DISPLAY_OFFSET		(0.2f)

TitleButtonManager::TitleButtonManager() :
	m_timer(0.f)
{
}

void TitleButtonManager::Initialize()
{
	//	画面の大きさ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	テクスチャの生成と初期化、ファイル読み込み
	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i] = std::make_unique<UserInterface>();
		m_texture[i]->SetAnchor(Anchor::MIDDLE_LEFT);
		m_texture[i]->SetPosition(SimpleMath::Vector2(width + 70.f, (height - 320.f) + 80.f * i));
	}
	m_texture[START]->LoadTexture(L"Resources/Texture/Title/Start.png");
	m_texture[OPTION]->LoadTexture(L"Resources/Texture/Title/Option.png");
	m_texture[CREDIT]->LoadTexture(L"Resources/Texture/Title/Credit.png");
	m_texture[EXIT]->LoadTexture(L"Resources/Texture/Title/Exit.png");

	//	インフォメーション画像の生成と読み込み
	for (int i = 0; i < SIZE; i++)
	{
		m_info[i] = std::make_unique<UserInterface>();
		m_info[i]->SetAnchor(Anchor::MIDDLE_LEFT);
		m_info[i]->SetPosition(SimpleMath::Vector2(20.f, height - 70.f));
		m_info[i]->SetAlpha(0.f);
	}
	m_info[START]->LoadTexture(L"Resources/Texture/Title/GameStartTelop.png");
	m_info[OPTION]->LoadTexture(L"Resources/Texture/Title/OptionTelop.png");
	m_info[CREDIT]->LoadTexture(L"Resources/Texture/Title/CreditTelop.png");
	m_info[EXIT]->LoadTexture(L"Resources/Texture/Title/ExitTelop.png");
	
	//	アニメーションの生成とファイル読み込み
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_fadeinAnimator[i]->LoadFile("Resources/UIAnimation/Title/TitleButtonIn.json");

		m_choiceAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_choiceAnimator[i]->LoadFile("Resources/UIAnimation/Title/ChoiceButton.json");
	}
	 
	//	ボタンの生成
	for (int i = 0; i < SIZE; i++)
	{
		m_buttons[i] = std::make_unique<Button>(m_texture[i].get());
	}
	 
	m_timer = 0.f;
}

int TitleButtonManager::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	//	アニメーションとボタンの更新
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i]->Update(elapsedTime);
		m_choiceAnimator[i]->Update(elapsedTime);
		m_buttons[i]->Update();
	}

	//	タイミングをずらして画面に表示する
	m_fadeinAnimator[START]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET) m_fadeinAnimator[OPTION]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET * 2) m_fadeinAnimator[CREDIT]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET * 3) m_fadeinAnimator[EXIT]->PlayAnimation();

	//	全てのアニメーションが終了するまで処理しない
	if (!m_fadeinAnimator[EXIT]->GetFinishAnimation()) return MAINTAIN;

	//	マウスが重なっていたら説明を表示し、アニメーションさせる
	for (int i = 0; i < SIZE; i++)
	{
		//	重なっているかどうか
		if (m_buttons[i]->OnMouse())
		{
			AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);
		}

		if (m_buttons[i]->StayMouse())
		{
			m_info[i]->SetAlpha(1.f);
			m_choiceAnimator[i]->PlayAnimation();
		}
		else
		{
			m_info[i]->SetAlpha(0.f);
			m_choiceAnimator[i]->PlayReverseAnimation();
		}

		//	クリックされたかどうか
		if (m_buttons[i]->IsPresed())
		{
			AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
			//	ボタンに対応したキーを送る
			return i;
		}
	}

	return MAINTAIN;
}

void TitleButtonManager::Render()
{
	for (const auto& it : m_texture)
	{
		it->Render();
	}

	for (const auto& it : m_info)
	{
		it->Render();
	}
}

void TitleButtonManager::Finalize()
{
}
