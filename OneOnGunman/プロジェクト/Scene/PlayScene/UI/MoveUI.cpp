/*/
 *  �t�@�C����		�F	MoveUI.cpp
 *  �T�v			�F	WASD�ňړ��ł��邱�ƂƃL�[���͂ł��Ă��邩����\������UI
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/12/08
/*/
#include "pch.h"
#include "MoveUI.h"

using namespace DirectX;

#include "Library/UserResources.h"

MoveUI::MoveUI() :
	m_isDisplay(false)
{
}

void MoveUI::Initialize()
{
	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	��̉�ʂ̑傫������̔䗦(1280 * 720�̎���1�ɂȂ�)
	float aspect = width / 1280.f;

	//	�e��UI�̐����Ɛݒ�
	m_back = std::make_unique<UserInterface>();
	m_back->LoadTexture(L"Resources/Texture/Play/Move_Back.png");
	m_back->SetAnchor(Anchor::BOTTOM_LEFT);
	m_back->SetPosition(SimpleMath::Vector2(30.f, height - 30.f));
	m_back->SetScale(SimpleMath::Vector2(width / 1280 * 0.5f));

	m_key_W = std::make_unique<UserInterface>();
	m_key_W->LoadTexture(L"Resources/Texture/Play/Move_W.png");
	m_key_W->SetAnchor(Anchor::BOTTOM_LEFT);
	m_key_W->SetPosition(m_back->GetPosition() + SimpleMath::Vector2(85.f, -130.f) * aspect);
	m_key_W->SetScale(SimpleMath::Vector2(width / 1280 * 0.4f));

	m_key_A = std::make_unique<UserInterface>();
	m_key_A->LoadTexture(L"Resources/Texture/Play/Move_A.png");
	m_key_A->SetAnchor(Anchor::BOTTOM_LEFT);
	m_key_A->SetPosition(m_back->GetPosition() + SimpleMath::Vector2(25.f, -70.f) * aspect);
	m_key_A->SetScale(SimpleMath::Vector2(width / 1280 * 0.4f));

	m_key_S = std::make_unique<UserInterface>();
	m_key_S->LoadTexture(L"Resources/Texture/Play/Move_S.png");
	m_key_S->SetAnchor(Anchor::BOTTOM_LEFT);
	m_key_S->SetPosition(m_back->GetPosition() + SimpleMath::Vector2(87.f, -10.f) * aspect);
	m_key_S->SetScale(SimpleMath::Vector2(width / 1280 * 0.4f));

	m_key_D = std::make_unique<UserInterface>();
	m_key_D->LoadTexture(L"Resources/Texture/Play/Move_D.png");
	m_key_D->SetAnchor(Anchor::BOTTOM_LEFT);
	m_key_D->SetPosition(m_back->GetPosition() + SimpleMath::Vector2(150.f, -70.f) * aspect);
	m_key_D->SetScale(SimpleMath::Vector2(width / 1280 * 0.4f));

	m_observer = std::make_unique<Observer>();
}

void MoveUI::Update()
{
	auto kb = Keyboard::Get().GetState();

	//	W�L�[
	if (kb.W) m_key_W->SetColor(SimpleMath::Vector3(1.f, 0.f, 0.f));
	else	  m_key_W->SetColor(SimpleMath::Vector3(1.f, 1.f, 1.f));

	//	A�L�[
	if (kb.A) m_key_A->SetColor(SimpleMath::Vector3(1.f, 0.f, 0.f));
	else	  m_key_A->SetColor(SimpleMath::Vector3(1.f, 1.f, 1.f));

	//	S�L�[
	if (kb.S) m_key_S->SetColor(SimpleMath::Vector3(1.f, 0.f, 0.f));
	else	  m_key_S->SetColor(SimpleMath::Vector3(1.f, 1.f, 1.f));

	//	D�L�[
	if (kb.D) m_key_D->SetColor(SimpleMath::Vector3(1.f, 0.f, 0.f));
	else	  m_key_D->SetColor(SimpleMath::Vector3(1.f, 1.f, 1.f));

	//	�\�����n�߂�
	m_isDisplay = true;
}

void MoveUI::Render()
{
	//	�Q�[�����܂��n�܂��Ă��Ȃ��̂ŕ\�����Ȃ�
	if (!m_isDisplay) return;

	//	�Q�[�����I�������̂ŕ\�����Ȃ�
	if (m_observer->GetNotification()) return;

	m_back->Render();
	m_key_W->Render();
	m_key_A->Render();
	m_key_S->Render();
	m_key_D->Render();
}

void MoveUI::RetryGame()
{
	//	�\�������Z�b�g
	m_isDisplay = false;
}
