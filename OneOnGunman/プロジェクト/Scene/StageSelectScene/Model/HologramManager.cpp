/*/
 *  �t�@�C����		�F	HologramManager.cpp
 *  �T�v			�F	�I������Ă���X�e�[�W�ƕ\�����郂�f�����Ǘ�
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/20
/*/
#include "pch.h"
#include "HologramManager.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Easing.h"
#include "Scene/Audio/AudioManager.h"

#define STAGE1_MODEL		(L"Resources/CMO/Robot_Guardian.cmo")
#define STAGE2_MODEL		(L"Resources/CMO/Robot_Collector.cmo")
#define STAGE3_MODEL		(L"Resources/CMO/Robot_Invader.cmo")

#define STAGE1_TEX			(L"Resources/CMO/_Users_it222275_Documents_23__�O��_�A�E��i_OneOnGunman_FBX_Robot__Guardian__Albedo.png.dds")
#define STAGE2_TEX			(L"Resources/CMO/_Users_it222275_Documents_23__�O��_�A�E��i_OneOnGunman_FBX_Robot__Collector__Albedo.png.dds")
#define STAGE3_TEX			(L"Resources/CMO/_Users_it222275_Documents_23__�O��_�A�E��i_OneOnGunman_FBX_Robot__Invader__Albedo.png.dds")

#define ARROW_TEX			(L"Resources/Texture/StageSelect/Arrow.png")
#define ANIMATION			("Resources/UIAnimation/StageSelect/ButtonSelect.json")

HologramManager::HologramManager() :
	m_nowChoice(0),
	m_beforeChoice(0),
	m_animTimer(0.5f),
	m_isAnimation(true)
{

}

void HologramManager::Initialize(int stageIndex)
{
	//	���f���̐���
	for (auto& it : m_holograms)
	{
		it = std::make_unique<Hologram>();
	}

	m_holograms[0]->Initialize(STAGE1_MODEL, STAGE1_TEX);
	m_holograms[1]->Initialize(STAGE2_MODEL, STAGE2_TEX);
	m_holograms[2]->Initialize(STAGE3_MODEL, STAGE3_TEX);

	m_nowChoice = stageIndex;

	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	���̐���
	m_rightArrow = std::make_unique<UserInterface>();
	m_rightArrow->LoadTexture(ARROW_TEX);
	m_rightArrow->SetAnchor(Anchor::MIDDLE_CENTER);
	m_rightArrow->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.5f));
	m_rightArrow->SetScale(SimpleMath::Vector2::One * 0.48f);

	m_rightArrowButton = std::make_unique<Button>(m_rightArrow.get());

	m_rightArrowAnimation = std::make_unique<Animator>(m_rightArrow.get());
	m_rightArrowAnimation->LoadFile(ANIMATION);

	m_leftArrow = std::make_unique<UserInterface>();
	m_leftArrow->LoadTexture(ARROW_TEX);
	m_leftArrow->SetAnchor(Anchor::MIDDLE_CENTER);
	m_leftArrow->SetPosition(SimpleMath::Vector2(width * 0.1f, height * 0.5f));
	m_leftArrow->SetScale(SimpleMath::Vector2::One * 0.5f);
	m_leftArrow->SetRadian(XM_PI);

	m_leftArrowButton = std::make_unique<Button>(m_leftArrow.get());

	m_leftArrowAnimation = std::make_unique<Animator>(m_leftArrow.get());
	m_leftArrowAnimation->LoadFile(ANIMATION);
}

void HologramManager::Update()
{
	//	���f���̍X�V
	for (auto& it : m_holograms)
	{
		it->Update();
	}

	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	if (m_isAnimation)
	{
		m_animTimer += elapsedTime;

		if (m_animTimer < 0.5f)
		{
			float scale = Easing::Lerp(1.f, 0.f, m_animTimer * 2.f, Easing::InBack);
			m_holograms[m_beforeChoice]->SetScale(scale);
		}
		else if (m_animTimer < 1.f)
		{
			m_holograms[m_beforeChoice]->SetScale(0.f);
			float scale = Easing::Lerp(0.f, 1.f, (m_animTimer * 2.f) - 1.f, Easing::OutBack);
			m_holograms[m_nowChoice]->SetScale(scale);
		}
		else
		{
			m_isAnimation = false;
		}

		return;
	}

	m_rightArrowButton->Update();
	m_leftArrowButton->Update();
	m_rightArrowAnimation->Update(elapsedTime);
	m_leftArrowAnimation->Update(elapsedTime);

	//	�E�̖���\��
	if (m_nowChoice < 2)
	{
		m_rightArrow->SetAlpha(1.f);

		if (m_rightArrowButton->OnMouse()) AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);

		//	�}�E�X�J�[�\�����d�Ȃ��Ă�����A�j���[�V����������
		if (m_rightArrowButton->StayMouse()) m_rightArrowAnimation->PlayAnimation();
		else m_rightArrowAnimation->ResetAnimation();
	}
	else
	{
		m_rightArrow->SetAlpha(0.4f);
		m_rightArrowAnimation->ResetAnimation();
	}

	//	���̖���\��
	if (m_nowChoice > 0)
	{
		m_leftArrow->SetAlpha(1.f);

		if (m_leftArrowButton->OnMouse()) AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);

		//	�}�E�X�J�[�\�����d�Ȃ��Ă�����A�j���[�V����������
		if (m_leftArrowButton->StayMouse()) m_leftArrowAnimation->PlayAnimation();
		else m_leftArrowAnimation->ResetAnimation();
	}
	else
	{
		m_leftArrow->SetAlpha(0.4f);
		m_leftArrowAnimation->ResetAnimation();
	}


	//	�E�̃{�^�����N���b�N
	if (m_rightArrowButton->IsPresed() && m_nowChoice < 2)
	{
		AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
		m_beforeChoice = m_nowChoice;
		m_nowChoice++;
		m_isAnimation = true;
		m_animTimer = 0.f;
	}

	//	���̃{�^�����N���b�N
	if (m_leftArrowButton->IsPresed() && m_nowChoice > 0)
	{
		AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
		m_beforeChoice = m_nowChoice;
		m_nowChoice--;
		m_isAnimation = true;
		m_animTimer = 0.f;
	}
}

void HologramManager::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	for (auto& it : m_holograms)
	{
		it->Render(view, proj);
	}

	m_rightArrow->Render();
	m_leftArrow->Render();
}

void HologramManager::Finalize()
{

}