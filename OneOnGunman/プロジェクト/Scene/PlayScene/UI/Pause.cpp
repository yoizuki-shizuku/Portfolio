/*/
 *  ファイル名		：	Pause.cpp
 *  概要			：	ポーズ画面
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/12/15
/*/
#include "pch.h"
#include "Pause.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"

Pause::Pause() :
	m_process(Process::MAINTAIN),
	m_letterBoxTimer(0.f)
{
}

void Pause::Initialize()
{
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_pause = std::make_unique<UserInterface>();
	m_pause->LoadTexture(L"Resources/Texture/Play/Pause.png");
	m_pause->SetAnchor(Anchor::MIDDLE_CENTER);
	m_pause->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.2f));
	m_pause->SetScale(SimpleMath::Vector2::One * (width / 1280));

	m_letterBox_1 = std::make_unique<UserInterface>();
	m_letterBox_1->LoadTexture(L"Resources/Texture/Play/LetterBox.png");
	m_letterBox_1->SetScale(SimpleMath::Vector2::One * (width / 1280.f));
	m_letterBox_1->SetPosition(SimpleMath::Vector2::Zero);
	m_letterBox_1->SetAnchor(Anchor::TOP_LEFT);
	m_letterBox_1->SetDirection(Direction::Down);
	m_letterBox_1->SetRatio(0.f);

	m_letterBox_2 = std::make_unique<UserInterface>();
	m_letterBox_2->LoadTexture(L"Resources/Texture/Play/LetterBox.png");
	m_letterBox_2->SetScale(SimpleMath::Vector2::One * (width / 1280.f));
	m_letterBox_2->SetPosition(SimpleMath::Vector2(0.f, height));
	m_letterBox_2->SetAnchor(Anchor::BOTTOM_LEFT);
	m_letterBox_2->SetDirection(Direction::Up);
	m_letterBox_2->SetRatio(0.f);

	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i] = std::make_unique<UserInterface>();
		m_texture[i]->SetAnchor(Anchor::MIDDLE_CENTER);
		m_texture[i]->SetScale(SimpleMath::Vector2(width / 1280 * 0.5f));

		m_animation[i] = std::make_unique<Animator>(m_texture[i].get());
		m_animation[i]->LoadFile("Resources/UIAnimation/Play/ButtonChoise.json");

		m_button[i] = std::make_unique<Button>(m_texture[i].get());
	}
	m_texture[(int)Process::CONTINUE]->LoadTexture(L"Resources/Texture/Play/Continue.png");
	m_texture[(int)Process::RESTART]->LoadTexture(L"Resources/Texture/Play/Restart.png");
	m_texture[(int)Process::SELECT]->LoadTexture(L"Resources/Texture/Play/Select.png");

	m_texture[(int)Process::CONTINUE]->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.4f));
	m_texture[(int)Process::RESTART]->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.6f));
	m_texture[(int)Process::SELECT]->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.8f));
}

Pause::Process Pause::Update()
{
	//	ポーズ状態の切り替え
	auto kbTracker = Input::GetInstace()->GetKeyboardTracker();
	if (kbTracker->IsKeyPressed(Keyboard::Escape))
	{
		if (m_process == Process::MAINTAIN) m_process = Process::PAUSE;
		else								m_process = Process::MAINTAIN;
	}

	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	何かしら処理しているとき以外にレターボックスを動かす
	if (m_process == Process::PAUSE || m_process == Process::MAINTAIN)
	{
		//	レターボックス表示用のタイマーを更新
		m_letterBoxTimer += elapsedTime * ((int)m_process * -2 - 3);
		m_letterBoxTimer = Useful::Clamp(m_letterBoxTimer, 0.f, 0.5f);

		//	レターボックスの表示
		float ratio = Useful::Ratio01(m_letterBoxTimer, 0.5f);
		float easing = Easing::Lerp(0.f, 1.f, ratio, Easing::OutCirc);
		m_letterBox_1->SetRatio(easing);
		m_letterBox_2->SetRatio(easing);
	}

	//	ポーズ中でない時は処理しない
	if (m_process == Process::MAINTAIN) return Process::MAINTAIN;

	//	更新
	for (int i = 0; i < SIZE; i++)
	{
		m_animation[i]->Update(elapsedTime);
		m_button[i]->Update();
	}

	//	マウスが重なっていたらアニメーションさせる
	for (int i = 0; i < SIZE; i++)
	{
		//	重なっているかどうか
		if (!m_button[i]->StayMouse())
		{
			m_animation[i]->PlayReverseAnimation();
			continue;
		}

		m_animation[i]->PlayAnimation();

		//	クリックされたら対応した動作を行う
		auto msTracker = Input::GetInstace()->GetMouseTracker();
		if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
		{
			m_process = (Process)i;
			break;
		}
	}

	//	ポーズ解除
	if (m_process == Process::CONTINUE)
	{
		m_process = Process::MAINTAIN;
	}

	return m_process;
}

void Pause::Render()
{
	m_letterBox_1->Render();
	m_letterBox_2->Render();

	//	ポーズ中のみ描画
	if (m_process == Process::MAINTAIN) return;

	m_pause->Render();
	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i]->Render();
	}
}

void Pause::Finalize()
{
}

void Pause::RetryGame()
{
	m_process = Process::MAINTAIN;
	m_letterBoxTimer = 0.f;

	m_letterBox_1->SetRatio(0.f);
	m_letterBox_2->SetRatio(0.f);
}
