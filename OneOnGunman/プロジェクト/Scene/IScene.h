/*/
 *  ファイル名		：	IScene.h
 *  概要			：	シーンクラスのインターフェース
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/09
/*/
#pragma once

class Scene;

__interface IScene
{
	void Initialize();
	void Update();
	void Render();
	void Finalize();

	//	共通のUpdate
	void CommonUpdate();
	//	シーンを変更
	void ChangeScene(std::function<Scene* ()> func);
	//	ゲームを終了
	void ExitGame();
	/// <summary>
	/// ゲームの終了フラグを取得
	/// </summary>
	/// <returns>true : 終了</returns>
	const bool GetExitGame();
	//	シーンの変更タイミングを取得
	const bool GetChangeScene();
	//	変更先のシーンを取得
	const std::function<Scene* ()> GetNextScene();

};
