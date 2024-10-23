/*/
 *  �t�@�C����		�F	Run.h
 *  �T�v			�F	�ړ����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#pragma once

#include "State.h"

class Player;

class Run final : public State
{
public:
	Run(Player* player);
	~Run() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	�v���C���[�N���X
	Player* m_player;

};