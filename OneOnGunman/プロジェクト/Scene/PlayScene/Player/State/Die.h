/*/
 *  �t�@�C����		�F	Die.h
 *  �T�v			�F	������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/06
/*/
#pragma once

#include "State.h"

class Player;

class Die : public State
{
public:
	Die(Player* player);
	~Die() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	�v���C���[�̃N���X
	Player* m_player;

};