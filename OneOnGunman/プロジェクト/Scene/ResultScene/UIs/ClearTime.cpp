/*/
 *  ファイル名		：	ClearTime.cpp
 *  概要			：	クリアタイムを表示
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/19
/*/
#include "pch.h"
#include "ClearTime.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

ClearTime::ClearTime() :
	m_clearTime(0.f),
	m_displayTime(0),
	m_directionTimer(0.f)
{

}

void ClearTime::Initialize(float clearTime)
{
	m_clearTime = Useful::Clamp(clearTime, 0.f, 999.f);

	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_texture = std::make_unique<UserInterface>();

	m_hundred = std::make_unique<Numeral>();
	m_ten     = std::make_unique<Numeral>();
	m_one     = std::make_unique<Numeral>();

	m_texture->LoadTexture(L"Resources/Texture/Result/ClearTime.png");
	m_texture->SetAnchor(Anchor::MIDDLE_CENTER);
	m_texture->SetPosition(SimpleMath::Vector2(width * 0.3f, height * 0.36f));
	m_texture->SetScale(SimpleMath::Vector2::One * 0.8f);

	m_hundred->LoadTexture(L"Resources/Texture/Numeral.png");
	m_hundred->SetSize(SimpleMath::Vector2(24.f, 97.f));
	m_hundred->SetPosition(SimpleMath::Vector2(width * 0.7f, height * 0.34f));

	m_ten->LoadTexture(L"Resources/Texture/Numeral.png");
	m_ten->SetSize(SimpleMath::Vector2(24.f, 97.f));
	m_ten->SetPosition(SimpleMath::Vector2(width * 0.7f + 60.f, height * 0.34f));

	m_one->LoadTexture(L"Resources/Texture/Numeral.png");
	m_one->SetSize(SimpleMath::Vector2(24.f, 97.f));
	m_one->SetPosition(SimpleMath::Vector2(width * 0.7f + 120.f, height * 0.34f));
}

void ClearTime::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_directionTimer += elapsedTime;

	m_directionTimer = Useful::Clamp(m_directionTimer, 0.f, 2.f);
	float ratio = Useful::Ratio01(m_directionTimer, 2.f);

	m_displayTime = static_cast<int>(Easing::Lerp(0.f, m_clearTime, ratio));

	int one = 0, ten = 0, hun = 0;

	one = m_displayTime % 10;
	ten = m_displayTime / 10 % 10;
	hun = m_displayTime / 100;

	m_one->SetNumber(one);
	m_ten->SetNumber(ten);
	m_hundred->SetNumber(hun);
}

void ClearTime::Render()
{
	m_texture->Render();

	m_hundred->Render();
	m_ten->Render();
	m_one->Render();
}

void ClearTime::Finalize()
{

}