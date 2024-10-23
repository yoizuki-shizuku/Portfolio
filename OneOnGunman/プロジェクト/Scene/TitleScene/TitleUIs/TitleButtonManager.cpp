/*/
 *  �t�@�C����		�F	TitleButtonManager.cpp
 *  �T�v			�F	�^�C�g����ʂł̃{�^�����Ǘ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/08
/*/
#include "pch.h"
#include "TitleButtonManager.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/Audio/AudioManager.h"

//	�\���^�C�~���O
#define DISPLAY_OFFSET		(0.2f)

TitleButtonManager::TitleButtonManager() :
	m_timer(0.f)
{
}

void TitleButtonManager::Initialize()
{
	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	�e�N�X�`���̐����Ə������A�t�@�C���ǂݍ���
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

	//	�C���t�H���[�V�����摜�̐����Ɠǂݍ���
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
	
	//	�A�j���[�V�����̐����ƃt�@�C���ǂݍ���
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_fadeinAnimator[i]->LoadFile("Resources/UIAnimation/Title/TitleButtonIn.json");

		m_choiceAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_choiceAnimator[i]->LoadFile("Resources/UIAnimation/Title/ChoiceButton.json");
	}
	 
	//	�{�^���̐���
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

	//	�A�j���[�V�����ƃ{�^���̍X�V
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i]->Update(elapsedTime);
		m_choiceAnimator[i]->Update(elapsedTime);
		m_buttons[i]->Update();
	}

	//	�^�C�~���O�����炵�ĉ�ʂɕ\������
	m_fadeinAnimator[START]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET) m_fadeinAnimator[OPTION]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET * 2) m_fadeinAnimator[CREDIT]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET * 3) m_fadeinAnimator[EXIT]->PlayAnimation();

	//	�S�ẴA�j���[�V�������I������܂ŏ������Ȃ�
	if (!m_fadeinAnimator[EXIT]->GetFinishAnimation()) return MAINTAIN;

	//	�}�E�X���d�Ȃ��Ă����������\�����A�A�j���[�V����������
	for (int i = 0; i < SIZE; i++)
	{
		//	�d�Ȃ��Ă��邩�ǂ���
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

		//	�N���b�N���ꂽ���ǂ���
		if (m_buttons[i]->IsPresed())
		{
			AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
			//	�{�^���ɑΉ������L�[�𑗂�
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
