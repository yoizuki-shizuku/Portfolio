/*/
 *  �t�@�C����		�F	PlaySceneCamera.h
 *  �T�v			�F	PlayScene��p�̃J����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/26
/*/
#pragma once

#include "../Observer/Observer.h"

class PlaySceneCamera
{
public:
	PlaySceneCamera();
	~PlaySceneCamera() = default;

	void Initialize();
	void Update();
	void Finalize();

	//	���g���C���̃t���O������
	void RetryGame() { m_eventTimer = 0.f; }

public:
	DirectX::SimpleMath::Matrix GetView()       const { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() const { return m_proj; }
	DirectX::SimpleMath::Vector3 GetFront()     const { return m_front; }
	DirectX::SimpleMath::Vector3 GetEye()       const { return m_eye; }
	Observer* GetClearObserver()				const { return m_claerObs.get(); }
	Observer* GetFailureObserver()				const { return m_failureObs.get(); }

	void SetEye(DirectX::SimpleMath::Vector3 eye)                  { m_eye = eye; }
	void SetTarget(DirectX::SimpleMath::Vector3 target)            { m_target = target; }
	void SetPlayerPosition(DirectX::SimpleMath::Vector3 pPosition) { m_playerPosition = pPosition; }
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 ePosition)  { m_enemyPosition = ePosition; }

private:
	//	�ˉe�s��̍쐬
	void CreateProjection();
	//	�ʏ펞�̃J��������
	void DefaultUpdate();
	//	���j���̃J��������
	void WinUpdate();
	//	���S���̃J��������
	void FailureUpdate();

private:
	//	�J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eye;
	//	�����_�̈ʒu
	DirectX::SimpleMath::Vector3 m_target;
	//	�J�����̏�����x�N�g��
	DirectX::SimpleMath::Vector3 m_up;

	//	�J�����̐��ʃx�N�g���i���������̂݁j
	DirectX::SimpleMath::Vector3 m_front;

	//	�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//	�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	//	�v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_playerPosition;
	//	�G�̈ʒu
	DirectX::SimpleMath::Vector3 m_enemyPosition;

	//	�J�����̉�]�p�x
	DirectX::SimpleMath::Quaternion m_rotation;

	//	�G�l�~�[���|���ꂽ�̂��ǂ����̒ʒm���󂯎��
	std::unique_ptr<Observer> m_claerObs;
	//	�v���C���[���|���ꂽ���ǂ����̒ʒm���󂯎��
	std::unique_ptr<Observer> m_failureObs;
	//	���j���o�p�̃^�C�}�[
	float m_eventTimer;

};