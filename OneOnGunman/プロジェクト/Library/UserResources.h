/*/
 *	�t�@�C����		�F	UserResources.h
 *	�T�v			�F	�e�V�[���ŋ��ʂ̂��̂ɃA�N�Z�X�ł���悤�ɂ����V���O���g��
 *
 *	�쐬��			�F	�R�{���b
 *	�쐬��			�F	2023/06/05
/*/
#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"

class UserResources
{
public:
	//	�f�X�g���N�^
	~UserResources() = default;

	//	�f�o�C�X���\�[�X
	void SetDeviceResources(DX::DeviceResources* device) { m_deviceResources = device; }
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }

	//	�^�C�}�[
	void SetStepTimer(DX::StepTimer* timer) { m_stepTimer = timer; }
	DX::StepTimer* GetStepTimer() const { return m_stepTimer; }

	//	���ʃX�e�[�g
	void SetCommonStates(DirectX::CommonStates* states) { m_commonStates = states; }
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }

public:
	//	�C���X�^���X����
	static void CreateInstance();

	//	�C���X�^���X�擾
	static UserResources* const GetInstance();

private:
	//	�R���X�g���N�^
	UserResources();

	//	����A�R�s�[�̃R���X�g���N�^���폜
	void operator=(const UserResources& object) = delete;
	UserResources(const UserResources& object) = delete;

private:
	//	�C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<UserResources> s_instance;

	//	�f�o�C�X���\�[�X
	DX::DeviceResources*   m_deviceResources;

	//	�X�e�b�v�^�C�}�[
	DX::StepTimer*         m_stepTimer;

	//	���ʃX�e�[�g
	DirectX::CommonStates* m_commonStates;
};