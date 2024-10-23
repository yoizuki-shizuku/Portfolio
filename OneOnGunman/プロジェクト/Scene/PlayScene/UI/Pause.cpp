/*/
 *  �t�@�C����		�F	Pause.cpp
 *  �T�v			�F	�|�[�Y���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/12/15
/*/
#include "pch.h"
#include "Pause.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"

Pause::Pause() :
	m_process(Process::MAINTAIN),
	m_letterBoxTimer(0.f)
{
}

void Pause::Initialize()
{
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	m_pause = std::make_unique<UserInterface>();
	m_pause->LoadTexture(L"Resources/Texture/Play/Pause.png");
	m_pause->SetAnchor(Anchor::MIDDLE_CENTER);
	m_pause->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.2f));
	m_pause->SetScale(SimpleMath::Vector2::One * (width / 1280));

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

	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i] = std::make_unique<UserInterface>();
		m_texture[i]->SetAnchor(Anchor::MIDDLE_CENTER);
		m_texture[i]->SetScale(SimpleMath::Vector2(width / 1280 * 0.5f));

		m_animation[i] = std::make_unique<Animator>(m_texture[i].get());
		m_animation[i]->LoadFile("Resources/UIAnimation/Play/ButtonChoise.json");

		m_button[i] = std::make_unique<Button>(m_texture[i].get());
	}
	m_texture[(int)Process::CONTINUE]->LoadTexture(L"Resources/Texture/Play/Continue.png");
	m_texture[(int)Process::RESTART]->LoadTexture(L"Resources/Texture/Play/Restart.png");
	m_texture[(int)Process::SELECT]->LoadTexture(L"Resources/Texture/Play/Select.png");

	m_texture[(int)Process::CONTINUE]->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.4f));
	m_texture[(int)Process::RESTART]->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.6f));
	m_texture[(int)Process::SELECT]->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.8f));
}

Pause::Process Pause::Update()
{
	//	�|�[�Y��Ԃ̐؂�ւ�
	auto kbTracker = Input::GetInstace()->GetKeyboardTracker();
	if (kbTracker->IsKeyPressed(Keyboard::Escape))
	{
		if (m_process == Process::MAINTAIN) m_process = Process::PAUSE;
		else								m_process = Process::MAINTAIN;
	}

	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	�������珈�����Ă���Ƃ��ȊO�Ƀ��^�[�{�b�N�X�𓮂���
	if (m_process == Process::PAUSE || m_process == Process::MAINTAIN)
	{
		//	���^�[�{�b�N�X�\���p�̃^�C�}�[���X�V
		m_letterBoxTimer += elapsedTime * ((int)m_process * -2 - 3);
		m_letterBoxTimer = Useful::Clamp(m_letterBoxTimer, 0.f, 0.5f);

		//	���^�[�{�b�N�X�̕\��
		float ratio = Useful::Ratio01(m_letterBoxTimer, 0.5f);
		float easing = Easing::Lerp(0.f, 1.f, ratio, Easing::OutCirc);
		m_letterBox_1->SetRatio(easing);
		m_letterBox_2->SetRatio(easing);
	}

	//	�|�[�Y���łȂ����͏������Ȃ�
	if (m_process == Process::MAINTAIN) return Process::MAINTAIN;

	//	�X�V
	for (int i = 0; i < SIZE; i++)
	{
		m_animation[i]->Update(elapsedTime);
		m_button[i]->Update();
	}

	//	�}�E�X���d�Ȃ��Ă�����A�j���[�V����������
	for (int i = 0; i < SIZE; i++)
	{
		//	�d�Ȃ��Ă��邩�ǂ���
		if (!m_button[i]->StayMouse())
		{
			m_animation[i]->PlayReverseAnimation();
			continue;
		}

		m_animation[i]->PlayAnimation();

		//	�N���b�N���ꂽ��Ή�����������s��
		auto msTracker = Input::GetInstace()->GetMouseTracker();
		if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
		{
			m_process = (Process)i;
			break;
		}
	}

	//	�|�[�Y����
	if (m_process == Process::CONTINUE)
	{
		m_process = Process::MAINTAIN;
	}

	return m_process;
}

void Pause::Render()
{
	m_letterBox_1->Render();
	m_letterBox_2->Render();

	//	�|�[�Y���̂ݕ`��
	if (m_process == Process::MAINTAIN) return;

	m_pause->Render();
	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i]->Render();
	}
}

void Pause::Finalize()
{
}

void Pause::RetryGame()
{
	m_process = Process::MAINTAIN;
	m_letterBoxTimer = 0.f;

	m_letterBox_1->SetRatio(0.f);
	m_letterBox_2->SetRatio(0.f);
}
