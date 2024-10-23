/*/
 *  ファイル名		：	BeginEvent.cpp
 *  概要			：	ゲーム開始前の演出
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/01
/*/
#include "pch.h"
#include "BeginEvent.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

BeginEvent::BeginEvent() :
	m_letterBoxTimer(0.f)
{
}

void BeginEvent::Initialize()
{
	//	画面の大きさ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	テクスチャとアニメーションの設定
	m_ready = std::make_unique<UserInterface>();
	m_ready->LoadTexture(L"Resources/Texture/Play/Ready.png");
	m_ready->SetPosition(SimpleMath::Vector2(width / 2.f, height / 2.f));
	m_ready->SetAnchor(Anchor::MIDDLE_CENTER);
	m_ready->SetAlpha(1.f);

	m_readyAnim = std::make_unique<Animator>(m_ready.get());
	m_readyAnim->LoadFile("Resources/UIAnimation/Play/Ready.json");
	m_readyAnim->StartAnimation();

	m_go = std::make_unique<UserInterface>();
	m_go->LoadTexture(L"Resources/Texture/Play/Go!!.png");
	m_go->SetPosition(SimpleMath::Vector2(width / 2.f, height / 2.f));
	m_go->SetAnchor(Anchor::MIDDLE_CENTER);
	m_go->SetAlpha(0.f);

	m_goAnim = std::make_unique<Animator>(m_go.get());
	m_goAnim->LoadFile("Resources/UIAnimation/Play/Go.json");
	m_goAnim->StartAnimation();

	m_letterBox_1 = std::make_unique<UserInterface>();
	m_letterBox_1->LoadTexture(L"Resources/Texture/Play/LetterBox.png");
	m_letterBox_1->SetScale(SimpleMath::Vector2::One * (width / 1280.f));
	m_letterBox_1->SetPosition(SimpleMath::Vector2::Zero);
	m_letterBox_1->SetAnchor(Anchor::TOP_LEFT);
	m_letterBox_1->SetDirection(Direction::Down);

	m_letterBox_2 = std::make_unique<UserInterface>();
	m_letterBox_2->LoadTexture(L"Resources/Texture/Play/LetterBox.png");
	m_letterBox_2->SetScale(SimpleMath::Vector2::One * (width / 1280.f));
	m_letterBox_2->SetPosition(SimpleMath::Vector2(0.f, height));
	m_letterBox_2->SetAnchor(Anchor::BOTTOM_LEFT);
	m_letterBox_2->SetDirection(Direction::Up);
}

bool BeginEvent::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_readyAnim->Update(elapsedTime);
	m_goAnim->Update(elapsedTime);

	//	readyの表示が消えたらレターボックスをはける
	if (m_readyAnim->GetFinishAnimation())
	{
		m_letterBoxTimer += elapsedTime;
		float ratio = Useful::Ratio01(m_letterBoxTimer, 0.5f);
		float easing = Easing::Lerp(1.f, 0.f, ratio, Easing::OutCirc);
		m_letterBox_1->SetRatio(easing);
		m_letterBox_2->SetRatio(easing);
	}

	//	アニメーションが再生終了したらプレイシーンの処理を開始する
	return m_goAnim->GetFinishAnimation();
}

void BeginEvent::Render()
{
	//	アニメーションが再生終了したら表示しない
	if (m_goAnim->GetFinishAnimation()) return;

	m_go->Render();
	m_ready->Render();

	m_letterBox_1->Render();
	m_letterBox_2->Render();
}

void BeginEvent::Finalize()
{
}

void BeginEvent::RetryGame()
{
	m_letterBoxTimer = 0.f;
	m_letterBox_1->SetRatio(1.f);
	m_letterBox_2->SetRatio(1.f);

	m_ready->SetAlpha(1.f);
	m_go->SetAlpha(0.f);
	m_readyAnim->StartAnimation();
	m_goAnim->StartAnimation();
}
