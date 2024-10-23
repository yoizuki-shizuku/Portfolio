/*/
 *  �t�@�C����		�F	FinishEvent.cpp
 *  �T�v			�F	�Q�[���I�����̉��o
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/02
/*/
#include "pch.h"
#include "FinishEvent.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "Library/Easing.h"
#include "Library/Input.h"

FinishEvent::FinishEvent() :
	m_letterBoxTimer(0.f),
	m_pushButton(false)
{
}

void FinishEvent::Initialize()
{
	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	�e�N�X�`���̐ݒ�
	m_clear = std::make_unique<UserInterface>();
	m_clear->LoadTexture(L"Resources/Texture/Play/Clear.png");
	m_clear->SetPosition(SimpleMath::Vector2(width / 2.f, height / 2.f));
	m_clear->SetAnchor(Anchor::MIDDLE_CENTER);
	m_clear->SetAlpha(0.f);

	m_failure = std::make_unique<UserInterface>();
	m_failure->LoadTexture(L"Resources/Texture/Play/Failure.png");
	m_failure->SetPosition(SimpleMath::Vector2(width / 2.f, height / 2.f));
	m_failure->SetAnchor(Anchor::MIDDLE_CENTER);
	m_failure->SetAlpha(0.f);

	m_retry = std::make_unique<UserInterface>();
	m_retry->LoadTexture(L"Resources/Texture/Play/Retry.png");
	m_retry->SetPosition(SimpleMath::Vector2(width * 0.3f, height * 0.7f));
	m_retry->SetScale(SimpleMath::Vector2::One * 0.7f);
	m_retry->SetAnchor(Anchor::MIDDLE_CENTER);
	m_retry->SetAlpha(0.f);

	m_select = std::make_unique<UserInterface>();
	m_select->LoadTexture(L"Resources/Texture/Play/Select.png");
	m_select->SetPosition(SimpleMath::Vector2(width * 0.7f, height * 0.7f));
	m_select->SetScale(SimpleMath::Vector2::One * 0.7f);
	m_select->SetAnchor(Anchor::MIDDLE_CENTER);
	m_select->SetAlpha(0.f);

	//	�A�j���[�V�����̐ݒ�
	m_clearAnim = std::make_unique<Animator>(m_clear.get());
	m_clearAnim->LoadFile("Resources/UIAnimation/Play/Clear.json");
	m_failureAnim = std::make_unique<Animator>(m_failure.get());
	m_failureAnim->LoadFile("Resources/UIAnimation/Play/Failure.json");

	m_FadeIn[0] = std::make_unique<Animator>(m_retry.get());
	m_FadeIn[1] = std::make_unique<Animator>(m_select.get());
	m_choise[0] = std::make_unique<Animator>(m_retry.get());
	m_choise[1] = std::make_unique<Animator>(m_select.get());
	m_button[0] = std::make_unique<Button>(m_retry.get());
	m_button[1] = std::make_unique<Button>(m_select.get());

	for (int i = 0; i < 2; i++)
	{
		m_FadeIn[i]->LoadFile("Resources/UIAnimation/Play/ButtonFadeIn.json");
		m_choise[i]->LoadFile("Resources/UIAnimation/Play/ButtonChoise.json");
	}

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

	//	�I�u�U�[�o�[�̐���
	m_clearObs   = std::make_unique<Observer>();
	m_failureObs = std::make_unique<Observer>();
}

FinishEvent::Process FinishEvent::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	�N���b�N���ꂽ���ǂ���
	auto msTracker = Input::GetInstace()->GetMouseTracker();

	m_clearAnim->Update(elapsedTime);
	m_failureAnim->Update(elapsedTime);

	for (int i = 0; i < 2; i++)
	{
		m_FadeIn[i]->Update(elapsedTime);
		m_choise[i]->Update(elapsedTime);
		m_button[i]->Update();
	}

	//	�e�C�x���g���󂯎������A�j���[�V�������J�n���A
	//	�A�j���[�V�������I�������V�[����؂�ւ���
	if (m_clearObs->GetNotification())
	{
		m_clearAnim->PlayAnimation();
		m_letterBoxTimer += elapsedTime;
	}

	if (m_failureObs->GetNotification())
	{
		m_failureAnim->PlayAnimation();
		m_FadeIn[0]->PlayAnimation();
		m_FadeIn[1]->PlayAnimation();
		m_letterBoxTimer += elapsedTime;

		//	�A�j���[�V�������X�L�b�v
		if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
		{
			m_failureAnim->FinishAnimation();
			m_FadeIn[0]->FinishAnimation();
			m_FadeIn[1]->FinishAnimation();
			m_letterBoxTimer = 0.5f;
		}
	}

	//	���^�[�{�b�N�X�̕\��
	float ratio = Useful::Ratio01(m_letterBoxTimer, 0.5f);
	float easing = Easing::Lerp(0.f, 1.f, ratio, Easing::OutCirc);
	m_letterBox_1->SetRatio(easing);
	m_letterBox_2->SetRatio(easing);

	//	�A�j���[�V�������I��������V�[�����ړ�
	if (m_clearAnim->GetFinishAnimation()) return Process::CLEAR;

	//	���s���ɑ����邩�ǂ�����I��
	if (!m_failureAnim->GetFinishAnimation()) return Process::MAINTAIN;

	for (int i = 0; i < 2; i++)
	{
		//	���łɃ{�^�������������ǂ���
		if (m_pushButton) break;

		//	�d�Ȃ��Ă��邩�ǂ���
		if (m_button[i]->StayMouse())
		{
			m_choise[i]->PlayAnimation();
		}
		else
		{
			m_choise[i]->PlayReverseAnimation();
			continue;
		}

		if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
		{
			//	�{�^���ɑΉ������L�[�𑗂�
			m_pushButton = true;
			return (Process)i;
		}
	}

	return Process::MAINTAIN;
}

void FinishEvent::Render()
{
	m_clear->Render();
	m_failure->Render();
	m_retry->Render();
	m_select->Render();
	m_letterBox_1->Render();
	m_letterBox_2->Render();
}

void FinishEvent::RetryGame()
{
	m_letterBoxTimer = 0.f;
	m_pushButton = false;

	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_failureAnim->ResetAnimation();
	m_failure->SetAlpha(0.f);
	m_failure->SetPosition(SimpleMath::Vector2(width / 2.f, height / 2.f));
	m_retry->SetAlpha(0.f);
	m_select->SetAlpha(0.f);
	m_FadeIn[0]->ResetAnimation();
	m_FadeIn[1]->ResetAnimation();
	m_choise[0]->ResetAnimation();
	m_choise[1]->ResetAnimation();
	m_letterBox_1->SetRatio(0.f);
	m_letterBox_2->SetRatio(0.f);
}
