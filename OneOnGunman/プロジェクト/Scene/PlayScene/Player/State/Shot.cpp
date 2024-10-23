/*/
 *  �t�@�C����		�F	Shot.cpp
 *  �T�v			�F	�V���b�g���
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#include "pch.h"
#include "Shot.h"

using namespace DirectX;

#include "../Player.h"
#include "Library/UserResources.h"

//	�V���b�g��Ԃɓ����Ă��瑼�̏�Ԃɐ؂�ւ��܂ł̎���
#define CHANGE_TIME		(0.6f)

Shot::Shot(Player* player) :
	State(),
	m_player(player),
	m_timer(0.f)
{
	//	�A�j���[�V�����̐ݒ�
	SetAnimStartTime(0.3);
	SetAnimEndTime(1.0);
	SetAnimLoop(false);
}

void Shot::Initialize()
{
	//	�}�E�X�̈ʒu���甭�˕������v�Z

	//	�}�E�X���W�����[���h�ϊ�
	RECT size = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	int width = size.right;
	int height = size.bottom;

	auto mouse = Mouse::Get().GetState();
	int x = mouse.x;
	int y = mouse.y;

	SimpleMath::Matrix view, proj;
	m_player->GetViewProj(&view, &proj);

	SimpleMath::Vector3 stwPos = CalcScreenToXZ(x, y, width, height, view, proj);

	//	�����x�N�g�������߂�
	SimpleMath::Vector3 toVec = stwPos - m_player->GetPosition();
	toVec.Normalize();
	//	�p�x�ɕϊ�
	float rad = atan2f(toVec.x, toVec.z) + XM_PI;

	//	�N�H�[�^�j�I���ɕϊ�
	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	������K�p
	m_player->SetAngle(q);

	//	�e�𔭎�
	m_player->ShotBullet();

	//	�^�C�}�[�����Z�b�g
	m_timer = 0.f;

	//	�A�j���[�V�������͂��߂���ɂ���
	RestartAnimetion();

	//	�N�[���^�C�������Z�b�g
	m_player->ResetShotCoolTime();
}

void Shot::Update(float elapsedTime)
{
	//	�v���C���[���������Ă��Ȃ���΂����Ԃɂ���
	if (!m_player->GetIsAlive())
	{
		m_player->ChangeState(m_player->GetDie());
		return;
	}

	//	���Ԃ��v��
	m_timer += elapsedTime;

	//	���x���[����
	m_player->SetVelocity(SimpleMath::Vector3::Zero);

	//	���̎��Ԃ����ĂΏ�Ԃ�J�ڂ���
	if (m_timer < CHANGE_TIME) return;

	m_player->ChangeState(m_player->GetIdle());
}

void Shot::Finalize()
{
}

DirectX::SimpleMath::Vector3 Shot::CalcScreenToWorld(
	int screen_x, int screen_y, float fZ,
	int screen_w, int screen_h,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	�e�s��̋t�s����Z�o
	SimpleMath::Matrix invView, invProj, viewport, invViewport;
	invView = view.Invert();
	invProj = proj.Invert();
	viewport = SimpleMath::Matrix::Identity;
	viewport._11 = screen_w / 2.f;
	viewport._22 = -screen_h / 2.f;
	viewport._41 = screen_w / 2.f;
	viewport._42 = screen_h / 2.f;
	invViewport = viewport.Invert();

	SimpleMath::Matrix temp = invViewport * invProj * invView;

	return SimpleMath::Vector3::Transform(
		SimpleMath::Vector3(static_cast<float>(screen_x), static_cast<float>(screen_y), fZ), temp
	);
}

DirectX::SimpleMath::Vector3 Shot::CalcScreenToXZ(
	int screen_x, int screen_y,
	int screen_w, int screen_h,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	SimpleMath::Vector3 nearPos, farPos, ray;
	nearPos = CalcScreenToWorld(screen_x, screen_y, 0.f, screen_w, screen_h, view, proj);
	farPos = CalcScreenToWorld(screen_x, screen_y, 1.f, screen_w, screen_h, view, proj);
	ray = farPos - nearPos;
	ray.Normalize();

	//	���Ƃ̌������N���Ă���ꍇ�͌�_��
	//	�N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0)
	{
		float Lray = ray.Dot(SimpleMath::Vector3::Up);
		float lp0 = -nearPos.Dot(SimpleMath::Vector3::Up);
		return nearPos + (lp0 / Lray) * ray;
	}
	else
	{
		return farPos;
	}
}