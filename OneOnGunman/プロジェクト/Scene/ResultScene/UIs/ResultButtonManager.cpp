/*/
 *  �t�@�C����		�F	ResultButtonManager.cpp
 *  �T�v			�F	���U���g��ʂł̃{�^�����Ǘ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/18
/*/
#include "pch.h"
#include "ResultButtonManager.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/Audio/AudioManager.h"

//	�\���^�C�~���O
#define DISPLAY_OFFSET		(0.2f)
// TODO:�ŏI�X�e�[�W�ԍ�
#define LAST_STAGE_INDEX	(2)

ResultButtonManager::ResultButtonManager() :
	m_timer(0.f),
	m_isLastStage(false)
{
}

void ResultButtonManager::Initialize(int stageIndex)
{
	//	��ʂ̑傫���擾
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	�{�^���̉摜�̐����Ə������A�t�@�C���ǂݍ���
	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i] = std::make_unique<UserInterface>();
		m_texture[i]->SetAnchor(Anchor::MIDDLE_CENTER);
		m_texture[i]->SetScale(SimpleMath::Vector2::Zero);
	}
	m_texture[Process::SELECT]->LoadTexture(L"Resources/Texture/Result/Select.png");
	m_texture[Process::RETRY]->LoadTexture(L"Resources/Texture/Result/Retry.png");
	m_texture[Process::NEXT]->LoadTexture(L"Resources/Texture/Result/Next.png");

	m_texture[Process::SELECT]->SetPosition(SimpleMath::Vector2(width * 0.9f, height - 100.f));
	m_texture[Process::RETRY]->SetPosition(SimpleMath::Vector2(width * 0.73f, height - 100.f));
	m_texture[Process::NEXT]->SetPosition(SimpleMath::Vector2(width * 0.6f, height - 100.f));

	//	�ʂňʒu����
	m_texture[Process::RETRY]->SetPosition(m_texture[Process::RETRY]->GetPosition() + SimpleMath::Vector2(15.f, 0.f));

	//	�C���t�H���[�V�����摜�̐����Ə������A�ǂݍ���
	for (int i = 0; i < SIZE; i++)
	{
		m_info[i] = std::make_unique<UserInterface>();
		m_info[i]->SetAnchor(Anchor::MIDDLE_LEFT);
		m_info[i]->SetPosition(SimpleMath::Vector2(20.f, height - 90.f));
		m_info[i]->SetAlpha(0.f);
	}
	m_info[Process::SELECT]->LoadTexture(L"Resources/Texture/Result/SelectTelop.png");
	m_info[Process::RETRY]->LoadTexture(L"Resources/Texture/Result/RetryTelop.png");
	m_info[Process::NEXT]->LoadTexture(L"Resources/Texture/Result/NextTelop.png");

	//	�A�j���[�V�����ƃ{�^���̐���
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_fadeinAnimator[i]->LoadFile("Resources/UIAnimation/Result/ResultButtonIn.json");

		m_choiceAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_choiceAnimator[i]->LoadFile("Resources/UIAnimation/Result/ChoiceButton.json");
	
		m_buttons[i] = std::make_unique<Button>(m_texture[i].get());
	}

	m_timer = 0.f;
	m_isLastStage = stageIndex == LAST_STAGE_INDEX;
}

ResultButtonManager::Process ResultButtonManager::Update()
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
	m_fadeinAnimator[Process::NEXT]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET) m_fadeinAnimator[Process::RETRY]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET * 2) m_fadeinAnimator[Process::SELECT]->PlayAnimation();

	//	�S�ẴA�j���[�V�������I������܂ŏ������Ȃ�
	if (!m_fadeinAnimator[Process::SELECT]->GetFinishAnimation()) return Process::MAINTAIN;

	//	�}�E�X���d�Ȃ��Ă����������\�����A�A�j���[�V����������
	for (int i = 0; i < SIZE; i++)
	{
		//	�ŏI�X�e�[�W�������Ȃ��NEXT��\�����Ȃ�
		if (m_isLastStage && i == Process::NEXT) continue;

		if (m_buttons[i]->OnMouse()) AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);

		//	�d�Ȃ��Ă��邩�ǂ���
		if (m_buttons[i]->StayMouse())
		{
			m_info[i]->SetAlpha(1.f);
			m_choiceAnimator[i]->PlayAnimation();
		}
		else
		{
			m_info[i]->SetAlpha(0.f);
			m_choiceAnimator[i]->PlayReverseAnimation();
			continue;
		}

		//	�N���b�N���ꂽ���ǂ���
		if (m_buttons[i]->IsPresed())
		{
			AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
			//	�{�^���ɑΉ������L�[�𑗂�
			return (Process)i;
		}
	}

	return Process::MAINTAIN;
}

void ResultButtonManager::Render()
{
	for (int i = 0; i < SIZE; i++)
	{
		//	�ŏI�X�e�[�W�������Ȃ��NEXT��\�����Ȃ�
		if (m_isLastStage && i == (int)Process::NEXT) continue;

		m_texture[i]->Render();
		m_info[i]->Render();
	}
}

void ResultButtonManager::Finalize()
{
}
