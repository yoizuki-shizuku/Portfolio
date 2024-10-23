/*/
 *  �t�@�C����		�F	TitleScene.cpp
 *  �T�v			�F	�^�C�g���V�[���ɏ���������̂��W�߂��N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/12
/*/
#include "pch.h"
#include "TitleScene.h"
#include "../StageSelectScene/StageSelectScene.h"
#include "../OptionScene/OptionScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "../Audio/AudioManager.h"

TitleScene::TitleScene() :
	Scene()
{
}

void TitleScene::Initialize()
{
	//	�J�����̐����Ə����ݒ�
	m_camera = std::make_unique<Camera>();
	m_camera->SetEye(SimpleMath::Vector3(0.f, -8.f, -10.f));
	m_camera->SetTarget(SimpleMath::Vector3::Zero);
	m_camera->Initialize();

	m_skySphere = std::make_unique<SkySphere>();
	m_skySphere->Initialize();

	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	�^�C�g�����S
	m_logo = std::make_unique<GlitchEffect>();
	m_logo->LoadTexture(L"Resources/Texture/Title/Logo.png");
	m_logo->SetAnchor(Anchor::MIDDLE_CENTER);
	m_logo->SetPosition(SimpleMath::Vector2(width / 2.f, height / 3.6f));
	m_logo->SetScale(SimpleMath::Vector2::One * (static_cast<float>(rect.right) / 1280.f));

	//	�N���X�̍쐬�Ə�����
	m_pleaseLeftClick = std::make_unique<PleaseLeftClick>();
	m_pleaseLeftClick->Initialize();
	m_buttonManager = std::make_unique<TitleButtonManager>();
	m_buttonManager->Initialize();

	//	BGM�̍Đ�
	AudioManager::GetInstance()->StartBGM(CRI_BGM_�V�j���g���|���X);
}

void TitleScene::Update()
{
	m_logo->Update();

	//	���N���b�N�����܂ő��̑�����󂯂��Ȃ�
	if (!m_pleaseLeftClick->Update()) return;

	//	UI�̃{�^���̓��͂����m
	int button = m_buttonManager->Update();

	//	�܂����͂���Ă��Ȃ���Ώ������Ȃ�
	if (button == TitleButtonManager::Process::MAINTAIN) return;

	//	���͂�����ΑΉ����鑀����s��
	switch (button)
	{
	case TitleButtonManager::Process::START:
		AudioManager::GetInstance()->StopBGM();
		ChangeScene<StageSelectScene>(0);
		break;

	case TitleButtonManager::Process::OPTION:
		ChangeScene<OptionScene>();
		break;

	case TitleButtonManager::Process::CREDIT:
		break;

	case TitleButtonManager::Process::EXIT:
		ExitGame();
		break;

	default:
		break;
	}
}

void TitleScene::Render()
{
	//	�J�����̃r���[���X�V
	m_camera->CreateView();

	SimpleMath::Matrix view = m_camera->GetView();
	//	��ʃT�C�Y�擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	SimpleMath::Matrix proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		1.f,
		120.f
	);

	m_skySphere->Render(view, proj);

	//	�e�N�X�`���\��
	m_logo->Render();

	m_pleaseLeftClick->Render();
	m_buttonManager->Render();
}

void TitleScene::Finalize()
{
	m_pleaseLeftClick->Finalize();
	m_buttonManager->Finalize();
}
