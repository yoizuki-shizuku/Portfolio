/*/
 *  �t�@�C����		�F	ShotCount.h
 *  �T�v			�F	�ˌ��񐔂�\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/19
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Numeral.h"

class ShotCount
{
public:
	ShotCount();
	~ShotCount() = default;

	void Initialize(int shotCount);
	void Update();
	void Render();
	void Finalize();

private:
	//	�uOneShot�v�̉摜
	std::unique_ptr<UserInterface> m_texture;

	//	�Z�Ɓ~
	std::unique_ptr<UserInterface> m_circle;
	std::unique_ptr<UserInterface> m_cross;

	//	�ˌ���
	int m_shotCount;

};