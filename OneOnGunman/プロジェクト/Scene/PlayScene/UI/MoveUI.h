/*/
 *  �t�@�C����		�F	MoveUI.h
 *  �T�v			�F	WASD�ňړ��ł��邱�ƂƃL�[���͂ł��Ă��邩����\������UI
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/12/08
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/Observer/Observer.h"

class MoveUI
{
public:
	MoveUI();
	~MoveUI() = default;

	void Initialize();
	void Update();
	void Render();

	void RetryGame();

	Observer* GetObserver() { return m_observer.get(); }

private:
	//	UI�̌�둤
	std::unique_ptr<UserInterface> m_back;
	//	WASD�e��
	std::unique_ptr<UserInterface> m_key_W;
	std::unique_ptr<UserInterface> m_key_A;
	std::unique_ptr<UserInterface> m_key_S;
	std::unique_ptr<UserInterface> m_key_D;

	//	Update�����s����āA�\�����n�߂�
	bool m_isDisplay;

	//	�N���A�܂��̓~�X�̒ʒm���󂯎��
	std::unique_ptr<Observer> m_observer;

};