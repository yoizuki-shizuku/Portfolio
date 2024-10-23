/*/
 *  �t�@�C����		�F	PleasePressAnyKey.cpp
 *  �T�v			�F	�n�߂ɃL�[���������܂�UI��\�����A���̃L�[�̉��������m����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/08
/*/
#include "pch.h"
#include "PleaseLeftClick.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/Audio/AudioManager.h"

PleaseLeftClick::PleaseLeftClick() :
	m_isLeftClick(false)
{
}

void PleaseLeftClick::Initialize()
{
	//	��ʂ̑傫���擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	�e�N�X�`���쐬
	m_texture = std::make_unique<UserInterface>();
	m_texture->LoadTexture(L"Resources/Texture/Title/PleaseLeftClick.png");
	m_texture->SetScale(SimpleMath::Vector2::One * 0.8f * (width / 1280.f));
	m_texture->SetPosition(SimpleMath::Vector2(width / 2.f, height / 1.38f));
	m_texture->SetAnchor(Anchor::MIDDLE_CENTER);
	m_texture->SetAlpha(0.f);

	//	�A�j���[�V�����쐬
	m_fadeIn = std::make_unique<Animator>(m_texture.get());
	m_fadeIn->LoadFile("Resources/UIAnimation/Title/LeftClickFadeIn.json");
	m_fadeIn->StartAnimation();
}

bool PleaseLeftClick::Update()
{
	//	��A�̃A�j���[�V�������I��������A���̉�ʂ�
	if (m_fadeIn->GetFinishAnimation() && m_isLeftClick) return true;

	//	�o�ߎ��Ԍv��
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	�A�j���[�V�����̍X�V
	m_fadeIn->Update(elapsedTime);

	//	PleaseLeftClick�̕\�����ł���܂Ői�߂Ȃ��悤�ɂ���
	if (m_fadeIn->GetIsAnimation()) return false;

	//	�N���b�N���m
	auto msTracker = Input::GetInstace()->GetMouseTracker();
	if (msTracker->leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
		m_fadeIn->StartReverseAnimation();
		m_isLeftClick = true;
	}

	return false;
}

void PleaseLeftClick::Render()
{
	m_texture->Render();
}

void PleaseLeftClick::Finalize()
{
	m_fadeIn->Finalize();
}
