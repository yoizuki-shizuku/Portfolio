/*/
 *  �t�@�C����		�F	SceneManager.cpp
 *  �T�v			�F	�V�[���N���X���Ǘ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/09
/*/
#include "pch.h"
#include "SceneManager.h"

#include "TitleScene/TitleScene.h"
#include "PlayScene/PlayScene.h"
#include "StageSelectScene/StageSelectScene.h"
#include "ResultScene/ResultScene.h"
#include "OptionScene/OptionScene.h"

SceneManager::SceneManager() :
	m_currentScene(nullptr),
	m_nextScene(nullptr),
	m_transition(nullptr),
	m_saveData(nullptr),
	m_isLoading(false),
	m_finishThread(false)
{
}

SceneManager::~SceneManager()
{
	//	�X���b�h�����s����Ă�����I���܂őҋ@
	if (m_loadingThread.joinable()) m_loadingThread.join();
}

void SceneManager::Initialize()
{
	//	�g�����W�V�����̍쐬�Ɛݒ�
	Transition::CreateInstance();
	m_transition = Transition::GetInstance();
	m_transition->Initialize();

	//	���[�f�B���O�̍쐬�Ɛݒ�
	m_loading = std::make_unique<Loading>();
	m_loading->Initialize();

	//	�Z�[�u�f�[�^�N���X�̍쐬�ƃt�@�C���ǂݍ���
	SaveData::CreateInstance();
	m_saveData = SaveData::GetInstance();
	m_saveData->LoadSaveData();

	//	�I�[�f�B�I�}�l�[�W���[�̍쐬
	AudioManager::CreateInstance();
	m_audioManager = AudioManager::GetInstance();
	m_audioManager->Initialize();

	//	���߂ɐ�������V�[���ݒ�i�����̐����N���X��ς���΁A�Q�[���J�n���̃V�[����ύX�\�j
	m_nextScene = new TitleScene(); // TODO:�ς��Y�꒍��

	//	�V�[���̐���
	CreateScene();
}

void SceneManager::Update()
{
	m_transition->Update();
	m_audioManager->Update();

	//	���[�h���Ȃ烍�[�h��ʂ�\������
	if (m_isLoading)
	{
		m_loading->Update();
		return;
	}

	//	�V�[����؂�ւ��邩�̃t���O���擾
	bool changeSceneFlag = false;

	//	null�łȂ��Ȃ�Update������
	if (m_currentScene)
	{
		m_currentScene->CommonUpdate();
		m_currentScene->Update();
		changeSceneFlag = m_currentScene->GetChangeScene();
	}

	//	�V�[����؂�ւ��Ȃ��Ȃ炱�̐�͏������Ȃ�
	if (!changeSceneFlag) return;

	//	�V�[���̏����Ɛ���
	DeleteScene();
	CreateScene();
}

void SceneManager::Render()
{
	//	null�łȂ������[�h���łȂ����Render������
	if (m_currentScene && !m_isLoading)	m_currentScene->Render();

	//	���[�h���Ȃ烍�[�h��ʂ�\��
	if (m_isLoading) m_loading->Render();

	m_transition->Render();
}

void SceneManager::Finalize()
{
	//	�X���b�h�������Ă��Ă��I��������
	m_finishThread = true;

	DeleteScene();

	m_transition->Finalize();
	m_loading->Finalize();
	m_saveData->WriteSaveData();
	m_audioManager->Finalize();
}

bool SceneManager::GetExitGame()
{
	//	null�`�F�b�N���݁inull�Ȃ�I���j
	return m_currentScene ? m_currentScene->GetExitGame() : true;
}

ID3D11PixelShader * SceneManager::GetPixelShader()
{
	//	���[�h���͉�ʏ��������Ȃ�
	if (m_isLoading) return nullptr;

	//	null�`�F�b�N
	if (!m_currentScene) return nullptr;

	return m_currentScene->GetPixelShader();
}

ID3D11Buffer* SceneManager::GetConstBuffer()
{
	//	���[�h���͉�ʏ��������Ȃ�
	if (m_isLoading) return nullptr;

	//	null�`�F�b�N
	if (!m_currentScene) return nullptr;

	return m_currentScene->GetConstBuffer();
}

void SceneManager::CreateScene()
{
	//	�V�[�������ɍ쐬����Ă���̂Ȃ珈�����Ȃ�
	if (m_currentScene) return;

	//	�ύX��̃V�[���𐶐�
	m_currentScene.reset(m_nextScene);

	//	�|�C���^���w���Ă���K�v���Ȃ��Ȃ�̂�nullptr�ɂ��Ă���
	m_nextScene = nullptr;

	//	�X���b�h�����s����Ă�����I���܂őҋ@
	if (m_loadingThread.joinable()) m_loadingThread.join();

	//	���[�h�J�n
	m_isLoading = true;

	//	�������ƃ��\�[�X�̃��[�h��ʃX���b�h�ōs��
	m_loadingThread = std::thread{ [=]
		{
			//	�r������
			std::lock_guard<std::mutex> lock(m_sceneMutex);

			//	���[�h��ʂփt�F�[�h�C��
			m_transition->StartFadeIn();
			//	�t�F�[�h���͑ҋ@
			while (m_transition->ProcessCheckFadeIn())
			{
				//	�\�t�g���I�������ۂ͒��f
				if (m_finishThread) return;
			}

			//	�V�[���̏���
			m_currentScene->Initialize();

			//	�t�F�[�h�A�E�g
			m_transition->StartFadeOut();
			//	�t�F�[�h���͑ҋ@
			while (m_transition->ProcessCheckFadeOut())
			{
				//	�\�t�g���I�������ۂ͒��f
				if (m_finishThread) return;
			}

			//	���������V�[���փt�F�[�h�C��
			m_transition->StartFadeIn();
			//	���[�h�I��
			m_isLoading = false;
		}
	};
}

void SceneManager::DeleteScene()
{
	//	�V�[�������ɏ�������Ă����珈�����Ȃ�
	if (!m_currentScene) return;

	//	�X���b�h�����s����Ă�����I���܂őҋ@
	if (m_loadingThread.joinable()) m_loadingThread.join();

	//	�ύX��̃V�[�����擾
	m_nextScene = m_currentScene->GetNextScene();

	//	�㏈��
	m_currentScene->Finalize();

	//	����
	m_currentScene.reset();
}
