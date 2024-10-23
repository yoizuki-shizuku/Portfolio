/*/
 *  ファイル名		：	ShotCount.cpp
 *  概要			：	射撃回数を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/19
/*/
#include "pch.h"
#include "ShotCount.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

ShotCount::ShotCount() :
	m_shotCount(0)
{

}

void ShotCount::Initialize(int shotCount)
{
	m_shotCount = Useful::Clamp(shotCount, 0, 99);

	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_texture = std::make_unique<UserInterface>();
	m_texture->LoadTexture(L"Resources/Texture/Result/OneShot.png");
	m_texture->SetAnchor(Anchor::MIDDLE_CENTER);
	m_texture->SetPosition(SimpleMath::Vector2(width * 0.3f, height * 0.63f));
	m_texture->SetScale(SimpleMath::Vector2::One * 0.8f);
	
	m_circle = std::make_unique<UserInterface>();
	m_circle->LoadTexture(L"Resources/Texture/StageSelect/Circle.png");
	m_circle->SetAnchor(Anchor::MIDDLE_CENTER);
	m_circle->SetPosition(SimpleMath::Vector2(width * 0.75f, height * 0.62f));
	m_circle->SetScale(SimpleMath::Vector2(width / 1280 * 0.8f));

	m_cross = std::make_unique<UserInterface>();
	m_cross->LoadTexture(L"Resources/Texture/StageSelect/Cross.png");
	m_cross->SetAnchor(Anchor::MIDDLE_CENTER);
	m_cross->SetPosition(SimpleMath::Vector2(width * 0.75f, height * 0.61f));
	m_cross->SetScale(SimpleMath::Vector2(width / 1280 * 0.8f));
}

void ShotCount::Update()
{
}

void ShotCount::Render()
{
	m_texture->Render();

	//	2発以上撃っていたら「×」を表示する
	if (m_shotCount > 1)	m_cross->Render();
	else					m_circle->Render();
}

void ShotCount::Finalize()
{

}