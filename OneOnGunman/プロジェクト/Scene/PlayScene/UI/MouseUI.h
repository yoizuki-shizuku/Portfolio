/*/
 *  �t�@�C����		�F	MouseUI.h
 *  �T�v			�F	�}�E�X�{�^���̑���w���vUI
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/12/10
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/Observer/Observer.h"

class MouseUI
{
public:
	MouseUI();
	~MouseUI() = default;

	void Initialize();
	void Update();
	void Render(float shotCoolRatio, float dodgeCoolRatio);

	void RetryGame();

	Observer* GetObserver() { return m_observer.get(); }

private:
	//	�}�E�X�{�^���̔w�i
	std::unique_ptr<UserInterface> m_mouseBack;
	//	�}�E�X�̃{�^��
	std::unique_ptr<UserInterface> m_leftButton;
	std::unique_ptr<UserInterface> m_rightButton;
	//	�A�C�R��
	std::unique_ptr<UserInterface> m_shotIcon;
	std::unique_ptr<UserInterface> m_dodgeIcon;
	//	�Q�[�W
	std::unique_ptr<UserInterface> m_geuge;

	//	Update�����s����āA�\�����n�߂�
	bool m_isDisplay;

	//	�N���A�܂��̓~�X�̒ʒm���󂯎��
	std::unique_ptr<Observer> m_observer;

};