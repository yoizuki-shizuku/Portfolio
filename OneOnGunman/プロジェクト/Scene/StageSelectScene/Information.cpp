/*/
 *  ファイル名		：	Information.cpp
 *  概要			：	ステージの情報を表示
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/22
/*/
#include "pch.h"
#include "Information.h"

using namespace DirectX;

#include "Library/UserResources.h"

Information::Information()
{
}

void Information::Initialize()
{
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_stage = std::make_unique<UserInterface>();
	m_stage->LoadTexture(L"Resources/Texture/StageSelect/Stage.png");
	m_stage->SetAnchor(Anchor::MIDDLE_CENTER);
	m_stage->SetPosition(SimpleMath::Vector2(width * 0.73f, height * 0.32f));
	m_stage->SetScale(SimpleMath::Vector2((width / 1280) * 0.5f));

	m_stageNumber = std::make_unique<Numeral>();
	m_stageNumber->LoadTexture(L"Resources/Texture/Numeral.png");
	m_stageNumber->SetPosition(SimpleMath::Vector2(width * 0.8f, height * 0.309f));
	m_stageNumber->SetSize(SimpleMath::Vector2(15.f, 58.2f));

	m_fream = std::make_unique<GlitchEffect>();
	m_fream->LoadTexture(L"Resources/Texture/StageSelect/Fream.png");
	m_fream->SetAnchor(Anchor::MIDDLE_CENTER);
	m_fream->SetPosition(SimpleMath::Vector2(width * 0.75f, height * 0.52f));
	m_fream->SetScale(SimpleMath::Vector2((width / 1280) * 0.9f));

	m_time = std::make_unique<UserInterface>();
	m_time->LoadTexture(L"Resources/Texture/StageSelect/Time.png");
	m_time->SetAnchor(Anchor::MIDDLE_CENTER);
	m_time->SetPosition(SimpleMath::Vector2(width * 0.64f, height * 0.44f));
	m_time->SetScale(SimpleMath::Vector2((width / 1280) * 0.5f));

	for (int i = 0; i < 3; i++)
	{
		m_timeNumber[i] = std::make_unique<Numeral>();
		m_timeNumber[i]->LoadTexture(L"Resources/Texture/Numeral.png");
		m_timeNumber[i]->SetSize(SimpleMath::Vector2(15.f, 58.2f));
		m_timeNumber[i]->SetPosition(SimpleMath::Vector2((width * 0.79f) + (width * 0.04f * i), height * 0.43f));
	}

	m_oneShot = std::make_unique<UserInterface>();
	m_oneShot->LoadTexture(L"Resources/Texture/StageSelect/OneShot.png");
	m_oneShot->SetAnchor(Anchor::MIDDLE_CENTER);
	m_oneShot->SetPosition(SimpleMath::Vector2(width * 0.68f, height * 0.57f));
	m_oneShot->SetScale(SimpleMath::Vector2(width / 1280) * 0.5f);

	m_circle = std::make_unique<UserInterface>();
	m_circle->LoadTexture(L"Resources/Texture/StageSelect/Circle.png");
	m_circle->SetAnchor(Anchor::MIDDLE_CENTER);
	m_circle->SetPosition(SimpleMath::Vector2(width * 0.84f, height * 0.56f));
	m_circle->SetScale(SimpleMath::Vector2(width / 1280) * 0.5f);

	m_cross = std::make_unique<UserInterface>();
	m_cross->LoadTexture(L"Resources/Texture/StageSelect/Cross.png");
	m_cross->SetAnchor(Anchor::MIDDLE_CENTER);
	m_cross->SetPosition(SimpleMath::Vector2(width * 0.84f, height * 0.56f));
	m_cross->SetScale(SimpleMath::Vector2(width / 1280) * 0.5f);

	m_play = std::make_unique<UserInterface>();
	m_play->LoadTexture(L"Resources/Texture/StageSelect/Play.png");
	m_play->SetAnchor(Anchor::MIDDLE_CENTER);
	m_play->SetPosition(SimpleMath::Vector2(width * 0.65f, height * 0.69f));
	m_play->SetScale(SimpleMath::Vector2(width / 1280) * 0.5f);

	for (int i = 0; i < 3; i++)
	{
		m_playNumber[i] = std::make_unique<Numeral>();
		m_playNumber[i]->LoadTexture(L"Resources/Texture/Numeral.png");
		m_playNumber[i]->SetSize(SimpleMath::Vector2(15.f, 58.2f));
		m_playNumber[i]->SetPosition(SimpleMath::Vector2((width * 0.79f) + (width * 0.04f * i), height * 0.69f));
	}

	m_stageData = SaveData::GetInstance()->GetStageData();
}

void Information::Update(int stageIndex)
{
	m_stageNumber->SetNumber(stageIndex + 1);
	m_fream->Update();

	//	クリアタイムの表示
	int claerTime = m_stageData[stageIndex].time;

	int one = claerTime % 10;
	int ten = claerTime / 10 % 10;
	int hun = claerTime / 100;

	m_timeNumber[0]->SetNumber(hun);
	m_timeNumber[1]->SetNumber(ten);
	m_timeNumber[2]->SetNumber(one);

	//	OneShotの表示
	bool oneShot = m_stageData[stageIndex].oneShot;

	if (oneShot)
	{
		m_circle->SetAlpha(1.f);
		m_cross->SetAlpha(0.f);
	}
	else
	{
		m_circle->SetAlpha(0.f);
		m_cross->SetAlpha(1.f);
	}

	//	プレイ回数の表示
	int tryCount = m_stageData[stageIndex].tryCount;

	one = tryCount % 10;
	ten = tryCount / 10 % 10;
	hun = tryCount / 100;

	m_playNumber[0]->SetNumber(hun);
	m_playNumber[1]->SetNumber(ten);
	m_playNumber[2]->SetNumber(one);

}

void Information::Render()
{
	m_stage->Render();
	m_stageNumber->Render();
	m_fream->Render();
	m_time->Render();
	for (int i = 0; i < 3; i++)
	{
		m_timeNumber[i]->Render();
		m_playNumber[i]->Render();
	}
	m_oneShot->Render();
	m_circle->Render();
	m_cross->Render();
	m_play->Render();
}

void Information::Finalize()
{
}
