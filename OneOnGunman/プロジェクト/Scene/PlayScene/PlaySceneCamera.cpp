/*/
 *  �t�@�C����		�F	PlaySceneCamera.cpp
 *  �T�v			�F	PlayScene��p�̃J����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/26
/*/
#include "pch.h"
#include "PlaySceneCamera.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

//	���j���o�̎���
#define EVENT_TIME	(3.f)

PlaySceneCamera::PlaySceneCamera() :
	m_eventTimer(0.f)
{
}

void PlaySceneCamera::Initialize()
{
	CreateProjection();

	m_up = SimpleMath::Vector3::Up;

	//	�����ʒu
	m_rotation = SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PIDIV2, 0.f, 0.f);

	//	�I�u�U�[�o�[�𐶐�
	m_claerObs = std::make_unique<Observer>();
	m_failureObs = std::make_unique<Observer>();
}

void PlaySceneCamera::Update()
{
	//	�G�̌��j�̒ʒm���󂯎�����猂�j�J�����ɂ���
	if (m_claerObs->GetNotification())
	{
		WinUpdate();
	}
	//	�v���C���[�̎��s�ʒm���󂯎�����牉�o�̃J�����֐؂�ւ���
	else if (m_failureObs->GetNotification())
	{
		FailureUpdate();
	}
	//	�ʏ펞
	else
	{
		DefaultUpdate();
	}

	//	�r���[�s����v�Z
	m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

void PlaySceneCamera::Finalize()
{
}

void PlaySceneCamera::CreateProjection()
{
	//	��ʃT�C�Y�擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f,
		150.f
	);
}

void PlaySceneCamera::DefaultUpdate()
{
	//	�����_��ݒ�
	m_target = SimpleMath::Vector3::Lerp(m_enemyPosition, m_playerPosition, 0.5f);

	//	�G�ƃv���C���[�̋������v�Z
	float distance = SimpleMath::Vector3::Distance(m_playerPosition, m_enemyPosition);

	//	�^�[�Q�b�g�ɑ΂���J�����̊�{�̈ʒu
	SimpleMath::Vector3 eye(0.0f, 2.f, 3.0f + (distance * 0.5f));

	//	�^�[�Q�b�g�̈ʒu����v���C���[���ǂ̊p�x�ɂ��邩�v�Z�iY�͎g��Ȃ��j
	float rad = Useful::GetAngle(m_playerPosition.x, m_playerPosition.z, m_enemyPosition.x, m_enemyPosition.z);
	rad += XM_PI / 2.f;

	//	�N�H�[�^�j�I���ɕϊ�
	SimpleMath::Quaternion radToQuat = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	���X�ɉ�]�����Ă���
	m_rotation = SimpleMath::Quaternion::Lerp(m_rotation, radToQuat, 0.05f);

	//	��]�s��ɕϊ�
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_rotation);

	//	�p�x��K�p
	eye = SimpleMath::Vector3::Transform(eye, rotate.Invert());

	//�J�����̕����x�N�g�����琳�ʕ����x�N�g�����擾����iY�͕s�v�Ȃ̂�0�j
	m_front = SimpleMath::Vector3(-eye.x, 0.0f, -eye.z);
	//���ʕ����x�N�g���𐳋K��
	m_front.Normalize();

	//	�J�����̈ʒu���^�[�Q�b�g�ɑ΂���p�x�ƈʒu����v�Z
	m_eye = eye + m_target;
}

void PlaySceneCamera::WinUpdate()
{
	//	���o���ԏI��
	if (m_eventTimer >= EVENT_TIME) return;

	//	�^�C�}�[�X�V
	m_eventTimer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	2�_�Ԃ̒��S���W
	SimpleMath::Vector3 center = SimpleMath::Vector3::Lerp(m_playerPosition, m_enemyPosition, 0.5f);
	m_target = center;

	//	�G�ƃv���C���[�̋������v�Z
	float distance = SimpleMath::Vector3::Distance(m_playerPosition, m_enemyPosition);
	//	�^�[�Q�b�g�ɑ΂���J�����̊�{�̈ʒu
	SimpleMath::Vector3 eye(1.0f, 3.f, 5.0f + (distance * 0.5f));

	//	�^�[�Q�b�g�̈ʒu����v���C���[���ǂ̊p�x�ɂ��邩�v�Z�iY�͎g��Ȃ��j
	float rad = Useful::GetAngle(m_playerPosition.x, m_playerPosition.z, m_enemyPosition.x, m_enemyPosition.z);
	rad += XM_PI / 2.f;

	//	���݂̊p�x�ɑ������̊p�x���v�Z
	float ratio = Useful::Ratio01(m_eventTimer, EVENT_TIME);
	float offset = Easing::OutInQuart(0.f, XM_2PI, ratio);
	float angle = rad + offset;

	//	��]�s��ɕϊ�
	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(angle, 0.f, 0.f);
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(q);

	m_eye = m_target + SimpleMath::Vector3::Transform(eye, rotate.Invert());
}

void PlaySceneCamera::FailureUpdate()
{
	//	�v���C���[���^�[�Q�b�g�ɂ���
	m_target = m_playerPosition;
	m_target.y = m_playerPosition.y + 0.3f;

	//	�^�[�Q�b�g�ɑ΂���J�����̊�{�̈ʒu
	SimpleMath::Vector3 eye(0.0f, 1.5f, 3.f);

	//	�Ō�Ɍ����Ă�����������ɉ�]������
	SimpleMath::Quaternion offset = SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PI, 0.f, 0.f);
	SimpleMath::Quaternion standard = m_rotation;
	standard = offset * standard;

	//	��]�s��ɕϊ�
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(standard);

	//	�K�p
	m_eye = m_target + SimpleMath::Vector3::Transform(eye, rotate.Invert());
}
