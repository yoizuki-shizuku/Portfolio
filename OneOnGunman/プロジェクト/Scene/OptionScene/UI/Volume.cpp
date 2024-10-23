/*/
 *  ファイル名		：	Volume.cpp
 *  概要			：	Optionで音の大きさを変更する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/02/07
/*/
#include "pch.h"
#include "Volume.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Scene/Audio/AudioManager.h"

Volume::Volume() :
	m_volume(0.f)
{
}

void Volume::Initialize(float volume, float offsetY)
{
	m_volume = volume;

	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);

	m_Plus = std::make_unique<UserInterface>();
	m_Plus->LoadTexture(L"Resources/Texture/Option/Plus.png");
	m_Plus->SetPosition(SimpleMath::Vector2(width * 0.8f, offsetY));
	m_Plus->SetAnchor(Anchor::MIDDLE_CENTER);
	m_Plus->SetScale(width / 1280.f * 0.6f);
	m_PlusButton = std::make_unique<Button>(m_Plus.get());

	m_Minus = std::make_unique<UserInterface>();
	m_Minus->LoadTexture(L"Resources/Texture/Option/Minus.png");
	m_Minus->SetPosition(SimpleMath::Vector2(width * 0.35f, offsetY));
	m_Minus->SetAnchor(Anchor::MIDDLE_CENTER);
	m_Minus->SetScale(width / 1280.f * 0.6f);
	m_MinusButton = std::make_unique<Button>(m_Minus.get());

	m_volumeGauge = std::make_unique<UserInterface>();
	m_volumeGauge->LoadTexture(L"Resources/Texture/Option/VolumeGauge.png");
	m_volumeGauge->SetPosition(SimpleMath::Vector2(width * 0.58f, offsetY));
	m_volumeGauge->SetAnchor(Anchor::MIDDLE_CENTER);
	m_volumeGauge->SetScale(width / 1280.f * 0.6f);
	m_volumeGauge->SetDirection(Direction::Right);
	m_volumeGauge->SetRatio(m_volume);

	m_volumeFrame = std::make_unique<UserInterface>();
	m_volumeFrame->LoadTexture(L"Resources/Texture/Option/VolumeFrame.png");
	m_volumeFrame->SetPosition(SimpleMath::Vector2(width * 0.58f, offsetY));
	m_volumeFrame->SetAnchor(Anchor::MIDDLE_CENTER);
	m_volumeFrame->SetScale(width / 1280.f * 0.6f);
}

void Volume::Update()
{
	m_PlusButton->Update();
	m_MinusButton->Update();

	AudioManager* audio = AudioManager::GetInstance();


	if (m_volume < 1.f)
	{
		m_Plus->SetAlpha(1.f);

		if (m_PlusButton->OnMouse()) audio->PlaySE(CRI_SE_MOVECURSOR);

		if (m_PlusButton->IsPresed())
		{
			m_volume += 0.1f;
			audio->PlaySE(CRI_SE_DECIDE);
		}
	}
	else
	{
		m_Plus->SetAlpha(0.5f);
	}


	if (m_volume > 0.f)
	{
		m_Minus->SetAlpha(1.f);

		if (m_MinusButton->OnMouse()) audio->PlaySE(CRI_SE_MOVECURSOR);

		if (m_MinusButton->IsPresed())
		{
			m_volume -= 0.1f;
			audio->PlaySE(CRI_SE_DECIDE);
		}
	}
	else
	{
		m_Minus->SetAlpha(0.5f);
	}

	m_volume = Useful::Clamp(m_volume, 0.f, 1.f);
	m_volumeGauge->SetRatio(m_volume);
}

void Volume::Render()
{
	m_Plus->Render();
	m_Minus->Render();
	m_volumeFrame->Render();
	m_volumeGauge->Render();
}
