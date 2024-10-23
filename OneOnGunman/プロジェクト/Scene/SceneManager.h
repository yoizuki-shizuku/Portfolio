/*/
 *  �t�@�C����		�F	SceneManager.h
 *  �T�v			�F	�V�[���N���X���Ǘ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/09
/*/
#pragma once

#include "Scene.h"
#include "Library/Transition.h"
#include "Library/Loading.h"
#include "SaveData/SaveData.h"
#include "Audio/AudioManager.h"

#include <thread>
#include <mutex>

class SceneManager
{
public:
	//	�R���X�g���N�^
	SceneManager();
	//	�f�X�g���N�^
	~SceneManager();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	/// <summary>
	/// �Q�[���̏I���t���O���擾
	/// </summary>
	/// <returns>true : �I��</returns>
	bool GetExitGame();

	//	�|�X�g�v���Z�X�p�̃s�N�Z���V�F�[�_�[��n��
	ID3D11PixelShader* GetPixelShader();
	//	�|�X�g�v���Z�X�p�̒萔�o�b�t�@�[��n��
	ID3D11Buffer* GetConstBuffer();

private:
	//	�V�[���N���X���쐬����
	void CreateScene();
	//	�V�[���N���X���폜����
	void DeleteScene();

private:
	//	���s���Ă���V�[���̃|�C���^
	std::unique_ptr<Scene> m_currentScene;
	//	�V�[���|�C���^�̔r������
	std::mutex m_sceneMutex;
	//	�ύX��̃V�[����ێ�����|�C���^
	Scene* m_nextScene;

	//	�g�����W�V�����N���X�̃|�C���^
	Transition* m_transition;
	//	�Z�[�u�f�[�^�N���X�̃|�C���^
	SaveData* m_saveData;
	//	�I�[�f�B�I�}�l�[�W���̃|�C���^
	AudioManager* m_audioManager;

	//	���[�h��ʂ�\������
	std::unique_ptr<Loading> m_loading;
	//	���[�h��ʂ�\�����邽�߂̃X���b�h
	std::thread m_loadingThread;
	//	���[�h�����ǂ���
	std::atomic<bool> m_isLoading;
	//	�X���b�h�̏I��
	std::atomic<bool> m_finishThread;

};