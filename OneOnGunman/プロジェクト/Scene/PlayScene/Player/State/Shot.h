/*/
 *  �t�@�C����		�F	Shot.h
 *  �T�v			�F	�V���b�g���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#pragma once

#include "State.h"

class Player;

class Shot final : public State
{
public:
	Shot(Player* player);
	~Shot() = default;

	void Initialize()              override;
	void Update(float elapsedTime) override;
	void Finalize()                override;

private:
	//	�X�N���[�����W�����[���h���W�ɕϊ�
	DirectX::SimpleMath::Vector3 CalcScreenToWorld(
		int screen_x, int screen_y,
		float fZ,
		int screen_w, int screen_h,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj
	);

	//	XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
	DirectX::SimpleMath::Vector3 CalcScreenToXZ(
		int screen_x, int screen_y,
		int screen_w, int screen_h,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj
	);

private:
	//	�v���C���[�̃N���X
	Player* m_player;
	//	�J�ڂ��Ă���̎��Ԃ��v��
	float m_timer;

};