/*/
 *  �t�@�C����		�F	ClearTime.h
 *  �T�v			�F	�N���A�^�C����\��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/19
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Numeral.h"

class ClearTime
{
public:
	ClearTime();
	~ClearTime() = default;

	void Initialize(float clearTime);
	void Update();
	void Render();
	void Finalize();

private:
	//	�摜
	std::unique_ptr<UserInterface> m_texture;
	//	����
	std::unique_ptr<Numeral> m_hundred;
	std::unique_ptr<Numeral> m_ten;
	std::unique_ptr<Numeral> m_one;

	//	�N���A�^�C��
	float m_clearTime;
	//	���o�p�ɕ\������^�C��
	int m_displayTime;
	//	���o�p�̃^�C�}�[
	float m_directionTimer;

};