/*/
 *  �t�@�C����		�F	Idle.h
 *  �T�v			�F	�ҋ@���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/21
/*/
#pragma once

#include "State.h"

class Player;

class Idle final : public State
{
public:
	Idle(Player* player);
	~Idle() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	�v���C���[�̃N���X
	Player* m_player;

};