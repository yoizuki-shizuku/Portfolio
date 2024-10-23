/*/
 *  �t�@�C����		�F	Camera.h
 *  �T�v			�F	�J�����̋@�\���������N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/12
/*/
#pragma once

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Initialize();
	void Update();
	
	//	�r���[�s��̍쐬
	void CreateView();


public:	//	�Q�b�^�[�ƃZ�b�^�[
	void SetEye(const DirectX::SimpleMath::Vector3& eye) { m_eye = eye; }
	void SetTarget(const DirectX::SimpleMath::Vector3& target) { m_target = target; }

	DirectX::SimpleMath::Matrix GetView() const { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() const { return m_proj; }

private:
	//	�ˉe�s��̍쐬
	void CreateProjection();

private:
	//	�J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eye;
	//	�����̈ʒu
	DirectX::SimpleMath::Vector3 m_target;
	//	�J�����̏�����x�N�g��
	DirectX::SimpleMath::Vector3 m_up;

	//	�r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	//	�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

};