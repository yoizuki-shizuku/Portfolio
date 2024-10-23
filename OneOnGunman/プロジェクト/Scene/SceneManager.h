/*/
 *  ファイル名		：	SceneManager.h
 *  概要			：	シーンクラスを管理する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/09
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
	//	コンストラクタ
	SceneManager();
	//	デストラクタ
	~SceneManager();

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	/// <summary>
	/// ゲームの終了フラグを取得
	/// </summary>
	/// <returns>true : 終了</returns>
	bool GetExitGame();

	//	ポストプロセス用のピクセルシェーダーを渡す
	ID3D11PixelShader* GetPixelShader();
	//	ポストプロセス用の定数バッファーを渡す
	ID3D11Buffer* GetConstBuffer();

private:
	//	シーンクラスを作成する
	void CreateScene();
	//	シーンクラスを削除する
	void DeleteScene();

private:
	//	実行しているシーンのポインタ
	std::unique_ptr<Scene> m_currentScene;
	//	シーンポインタの排他制御
	std::mutex m_sceneMutex;
	//	変更先のシーンを保持するポインタ
	Scene* m_nextScene;

	//	トランジションクラスのポインタ
	Transition* m_transition;
	//	セーブデータクラスのポインタ
	SaveData* m_saveData;
	//	オーディオマネージャのポインタ
	AudioManager* m_audioManager;

	//	ロード画面を表示する
	std::unique_ptr<Loading> m_loading;
	//	ロード画面を表示するためのスレッド
	std::thread m_loadingThread;
	//	ロード中かどうか
	std::atomic<bool> m_isLoading;
	//	スレッドの終了
	std::atomic<bool> m_finishThread;

};