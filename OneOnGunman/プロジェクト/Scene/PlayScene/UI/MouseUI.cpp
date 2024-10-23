/*/
 *  �t�@�C����		�F	MouseUI.cpp
 *  �T�v			�F	�}�E�X�{�^���̑���w���vUI
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/12/10
/*/
#include "pch.h"
#include "MouseUI.h"

using namespace DirectX;

#include "Library/UserResources.h"

MouseUI::MouseUI() :
	m_isDisplay(false)
{
}

void MouseUI::Initialize()
{
	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	��̉�ʂ̑傫������̔䗦(1280 * 720�̎���1�ɂȂ�)
	float aspect = width / 1280.f;

	m_mouseBack = std::make_unique<UserInterface>();
	m_mouseBack->LoadTexture(L"Resources/Texture/Play/Mouse_Back.png");
	m_mouseBack->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_mouseBack->SetPosition(SimpleMath::Vector2(width - 50.f, height - 50.f));
	m_mouseBack->SetScale(aspect * 0.4f);

	m_leftButton = std::make_unique<UserInterface>();
	m_leftButton->LoadTexture(L"Resources/Texture/Play/LeftMouse.png");
	m_leftButton->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_leftButton->SetPosition(m_mouseBack->GetPosition() + SimpleMath::Vector2(-120.f, -15.f) * aspect);
	m_leftButton->SetScale(aspect * 0.4f);

	m_rightButton = std::make_unique<UserInterface>();
	m_rightButton->LoadTexture(L"Resources/Texture/Play/RightMouse.png");
	m_rightButton->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_rightButton->SetPosition(m_mouseBack->GetPosition() + SimpleMath::Vector2(-35.f, -15.f) * aspect);
	m_rightButton->SetScale(aspect * 0.4f);

	m_shotIcon = std::make_unique<UserInterface>();
	m_shotIcon->LoadTexture(L"Resources/Texture/Play/ShotIcon.png");
	m_shotIcon->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_shotIcon->SetPosition(m_mouseBack->GetPosition() + SimpleMath::Vector2(-110.f, -100.f) * aspect);
	m_shotIcon->SetScale(aspect * 0.25f);

	m_dodgeIcon = std::make_unique<UserInterface>();
	m_dodgeIcon->LoadTexture(L"Resources/Texture/Play/DodgeIcon.png");
	m_dodgeIcon->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_dodgeIcon->SetPosition(m_mouseBack->GetPosition() + SimpleMath::Vector2(-30.f, -100.f) * aspect);
	m_dodgeIcon->SetScale(aspect * 0.25f);

	m_geuge = std::make_unique<UserInterface>();
	m_geuge->LoadTexture(L"Resources/Texture/Play/IconGeuge.png");
	m_geuge->SetAnchor(Anchor::BOTTOM_RIGHT);
	m_geuge->SetDirection(Direction::Up);
	m_geuge->SetScale(aspect * 0.25f);
	m_geuge->SetAlpha(0.5f);

	m_observer = std::make_unique<Observer>();
}

void MouseUI::Update()
{
	auto mouse = Mouse::Get().GetState();

	//	���N���b�N
	if (mouse.leftButton) m_leftButton->SetColor(SimpleMath::Vector3(1.f, 0.f, 0.f));
	else				  m_leftButton->SetColor(SimpleMath::Vector3(1.f, 1.f, 1.f));

	//	�E�N���b�N
	if (mouse.rightButton) m_rightButton->SetColor(SimpleMath::Vector3(1.f, 0.f, 0.f));
	else				   m_rightButton->SetColor(SimpleMath::Vector3(1.f, 1.f, 1.f));

	//	�\�����n�߂�
	m_isDisplay = true;
}

void MouseUI::Render(float shotCoolRatio, float dodgeCoolRatio)
{
	//	�Q�[�����܂��n�܂��Ă��Ȃ��̂ŕ\�����Ȃ�
	if (!m_isDisplay) return;

	//	�Q�[�����I�������̂ŕ\�����Ȃ�
	if (m_observer->GetNotification()) return;

	m_mouseBack->Render();
	m_leftButton->Render();
	m_rightButton->Render();

	//	�V���b�g�A�C�R��
	m_geuge->SetPosition(m_shotIcon->GetPosition() - SimpleMath::Vector2(5.f, 5.f));
	m_geuge->SetRatio(shotCoolRatio);
	m_geuge->Render();

	//	����A�C�R��
	m_geuge->SetPosition(m_dodgeIcon->GetPosition() - SimpleMath::Vector2(5.f, 5.f));
	m_geuge->SetRatio(dodgeCoolRatio);
	m_geuge->Render();

	m_shotIcon->Render();
	m_dodgeIcon->Render();
}

void MouseUI::RetryGame()
{
	//	�\�������Z�b�g
	m_isDisplay = false;
}
