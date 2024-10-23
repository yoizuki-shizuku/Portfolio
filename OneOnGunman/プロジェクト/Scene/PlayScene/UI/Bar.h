/*/
 *  �t�@�C����		�F	Bar.h
 *  �T�v			�F	�v���C���[�̊e�N�[���^�C����\������o�[UI
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/05
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/Observer/Observer.h"

class Bar
{
public:
	Bar();
	~Bar() = default;

	void Initialize();
	void Update(float shotRatio, float dodgeRatio);
	void Render();

	//	���g���C���̃t���O������
	void RetryGame();

	Observer* GetFinishObserver() const { return m_finishObs.get(); }

private:
	//	�t���[���g
	std::unique_ptr<UserInterface> m_barFream;
	//	�V���b�g�̃o�[
	std::unique_ptr<UserInterface> m_shotBar;
	//	����̃o�[
	std::unique_ptr<UserInterface> m_dodgeBar;
	
	//	�Q�[���I���̃C�x���g�����m
	std::unique_ptr<Observer> m_finishObs;


};