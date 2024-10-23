/*/
 *  ファイル名		：	Camera.cpp
 *  概要			：	カメラの機能を持ったクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/12
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
	//	画面サイズ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		1.f,
		20.f
	);
}
