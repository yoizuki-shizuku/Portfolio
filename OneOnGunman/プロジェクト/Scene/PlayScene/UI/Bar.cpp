/*/
 *  ファイル名		：	Bar.cpp
 *  概要			：	プレイヤーの各クールタイムを表示するバーUI
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/05
/*/
#include "pch.h"
#include "Bar.h"

using namespace DirectX;

#include "Library/UserResources.h"

Bar::Bar()
{
}

void Bar::Initialize()
{
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	基準の画面の大きさからの比率(1280 * 720の時に1になる)
	float aspect = width / 1280.f;

	//	各UIの作成
	m_barFream = std::make_unique<UserInterface>();
	m_barFream->LoadTexture(L"Resources/Texture/Play/BarFream.png");
	m_barFream->SetAnchor(Anchor::TOP_LEFT);
	m_barFream->SetPosition(SimpleMath::Vector2(20.f, 20.f));
	m_barFream->SetScale(aspect * 0.5f);

	m_shotBar = std::make_unique<UserInterface>();
	m_shotBar->LoadTexture(L"Resources/Texture/Play/ShotBar.png");
	m_shotBar->SetAnchor(Anchor::TOP_LEFT);
	m_shotBar->SetPosition(m_barFream->GetPosition() + (SimpleMath::Vector2(30.f, 20.f) * aspect));
	m_shotBar->SetDirection(Direction::Right);
	m_shotBar->SetScale(aspect * 0.5f);
	m_shotBar->SetRatio(0.f);

	m_dodgeBar = std::make_unique<UserInterface>();
	m_dodgeBar->LoadTexture(L"Resources/Texture/Play/DodgeBar.png");
	m_dodgeBar->SetAnchor(Anchor::TOP_LEFT);
	m_dodgeBar->SetPosition(m_barFream->GetPosition() + (SimpleMath::Vector2(27.f, 10.f) * aspect));
	m_dodgeBar->SetDirection(Direction::Right);
	m_dodgeBar->SetScale(aspect * 0.5f);
	m_dodgeBar->SetRatio(0.f);

	m_finishObs = std::make_unique<Observer>();
}

void Bar::Update(float shotRatio, float dodgeRatio)
{
	m_shotBar->SetRatio(shotRatio);
	m_dodgeBar->SetRatio(dodgeRatio);
}

void Bar::Render()
{
	//	ゲームが終了したら表示しない
	if (m_finishObs->GetNotification()) return;

	m_barFream->Render();
	m_shotBar->Render();
	m_dodgeBar->Render();
}

void Bar::RetryGame()
{
	m_shotBar->SetRatio(0.f);
	m_dodgeBar->SetRatio(0.f);
}

