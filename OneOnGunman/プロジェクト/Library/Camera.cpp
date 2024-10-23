/*/
 *  �t�@�C����		�F	Camera.cpp
 *  �T�v			�F	�J�����̋@�\���������N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/12
/*/
#include "pch.h"
#include "Camera.h"

#include "UserResources.h"

using namespace DirectX;

Camera::Camera()
{
}

void Camera::Initialize()
{
	m_up = SimpleMath::Vector3::Up;
	CreateView();
	CreateProjection();
}

void Camera::Update()
{
}

void Camera::CreateView()
{
	m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

void Camera::CreateProjection()
{
	//	��ʃT�C�Y�擾
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		1.f,
		20.f
	);
}
