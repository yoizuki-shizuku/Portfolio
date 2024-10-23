/*/
 *  ファイル名		：	SceneManager.cpp
 *  概要			：	シーンクラスを管理する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/09
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
	//	スレッドが実行されていたら終了まで待機
	if (m_loadingThread.joinable()) m_loadingThread.join();
}

void SceneManager::Initialize()
{
	//	トランジションの作成と設定
	Transition::CreateInstance();
	m_transition = Transition::GetInstance();
	m_transition->Initialize();

	//	ローディングの作成と設定
	m_loading = std::make_unique<Loading>();
	m_loading->Initialize();

	//	セーブデータクラスの作成とファイル読み込み
	SaveData::CreateInstance();
	m_saveData = SaveData::GetInstance();
	m_saveData->LoadSaveData();

	//	オーディオマネージャーの作成
	AudioManager::CreateInstance();
	m_audioManager = AudioManager::GetInstance();
	m_audioManager->Initialize();

	//	初めに生成するシーン設定（ここの生成クラスを変えれば、ゲーム開始時のシーンを変更可能）
	m_nextScene = new TitleScene(); // TODO:変え忘れ注意

	//	シーンの生成
	CreateScene();
}

void SceneManager::Update()
{
	m_transition->Update();
	m_audioManager->Update();

	//	ロード中ならロード画面を表示する
	if (m_isLoading)
	{
		m_loading->Update();
		return;
	}

	//	シーンを切り替えるかのフラグを取得
	bool changeSceneFlag = false;

	//	nullでないならUpdateを処理
	if (m_currentScene)
	{
		m_currentScene->CommonUpdate();
		m_currentScene->Update();
		changeSceneFlag = m_currentScene->GetChangeScene();
	}

	//	シーンを切り替えないならこの先は処理しない
	if (!changeSceneFlag) return;

	//	シーンの消去と生成
	DeleteScene();
	CreateScene();
}

void SceneManager::Render()
{
	//	nullでないかつロード中でなければRenderを処理
	if (m_currentScene && !m_isLoading)	m_currentScene->Render();

	//	ロード中ならロード画面を表示
	if (m_isLoading) m_loading->Render();

	m_transition->Render();
}

void SceneManager::Finalize()
{
	//	スレッドが動いていても終了させる
	m_finishThread = true;

	DeleteScene();

	m_transition->Finalize();
	m_loading->Finalize();
	m_saveData->WriteSaveData();
	m_audioManager->Finalize();
}

bool SceneManager::GetExitGame()
{
	//	nullチェック込み（nullなら終了）
	return m_currentScene ? m_currentScene->GetExitGame() : true;
}

ID3D11PixelShader * SceneManager::GetPixelShader()
{
	//	ロード中は画面処理をしない
	if (m_isLoading) return nullptr;

	//	nullチェック
	if (!m_currentScene) return nullptr;

	return m_currentScene->GetPixelShader();
}

ID3D11Buffer* SceneManager::GetConstBuffer()
{
	//	ロード中は画面処理をしない
	if (m_isLoading) return nullptr;

	//	nullチェック
	if (!m_currentScene) return nullptr;

	return m_currentScene->GetConstBuffer();
}

void SceneManager::CreateScene()
{
	//	シーンが既に作成されているのなら処理しない
	if (m_currentScene) return;

	//	変更先のシーンを生成
	m_currentScene.reset(m_nextScene);

	//	ポインタを指している必要がなくなるのでnullptrにしておく
	m_nextScene = nullptr;

	//	スレッドが実行されていたら終了まで待機
	if (m_loadingThread.joinable()) m_loadingThread.join();

	//	ロード開始
	m_isLoading = true;

	//	初期化とリソースのロードを別スレッドで行う
	m_loadingThread = std::thread{ [=]
		{
			//	排他制御
			std::lock_guard<std::mutex> lock(m_sceneMutex);

			//	ロード画面へフェードイン
			m_transition->StartFadeIn();
			//	フェード中は待機
			while (m_transition->ProcessCheckFadeIn())
			{
				//	ソフトが終了した際は中断
				if (m_finishThread) return;
			}

			//	シーンの準備
			m_currentScene->Initialize();

			//	フェードアウト
			m_transition->StartFadeOut();
			//	フェード中は待機
			while (m_transition->ProcessCheckFadeOut())
			{
				//	ソフトが終了した際は中断
				if (m_finishThread) return;
			}

			//	生成したシーンへフェードイン
			m_transition->StartFadeIn();
			//	ロード終了
			m_isLoading = false;
		}
	};
}

void SceneManager::DeleteScene()
{
	//	シーンが既に消去されていたら処理しない
	if (!m_currentScene) return;

	//	スレッドが実行されていたら終了まで待機
	if (m_loadingThread.joinable()) m_loadingThread.join();

	//	変更先のシーンを取得
	m_nextScene = m_currentScene->GetNextScene();

	//	後処理
	m_currentScene->Finalize();

	//	消去
	m_currentScene.reset();
}
