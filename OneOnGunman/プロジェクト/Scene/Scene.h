/*/
 *  ファイル名		：	Scene.h
 *  概要			：	シーンクラスで共有の関数を実装
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/09
/*/
#pragma once

#include <utility>
#include "Library/Transition.h"

class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

	virtual void Initialize() = 0;
	virtual void Update()     = 0;
	virtual void Render()     = 0;
	virtual void Finalize()   = 0;

	/// <summary>
	/// Update前にしておく共通処理
	/// </summary>
	virtual void CommonUpdate() final;
	/// <summary>
	/// ゲームの終了フラグを取得
	/// </summary>
	/// <returns>true : 終了</returns>
	virtual bool GetExitGame() const final { return m_exitGame; }
	//	シーンの変更タイミングを取得
	virtual bool GetChangeScene() const final { return m_isChange; }
	//	変更先のシーンクラスのポインタを取得
	virtual Scene* GetNextScene() final { return m_nextScene; }

	//	ゲームを終了する
	virtual void ExitGame() final { m_exitGame = true; }

	//	シーンを変更する
	template <class T, class... Args>
	inline void ChangeScene(Args&&... arg)
	{
		//	Sceneを継承したクラスかどうか調べる
		static_assert(std::is_base_of_v<Scene, T>);

		//	遷移実行中は複数回実行されても無効になるようにする
		if (m_isFadeOut || m_isFadeIn || m_isChange) return;

		//	必要な情報をセットし、遷移を開始する
		m_nextScene = new T(std::forward<Args>(arg)...);
		Transition::GetInstance()->StartFadeOut();
		m_isFadeOut = true;
	}

	//	ポストプロセス用のピクセルシェーダーを渡す
	virtual ID3D11PixelShader* GetPixelShader() const { return nullptr; }
	//	ポストプロセス用の定数バッファーを渡す
	virtual ID3D11Buffer* GetConstBuffer() const { return nullptr; }

private:
	//	ゲームの終了フラグ
	bool m_exitGame;

	//	シーンの変更タイミング
	bool m_isChange;

	//	シーン変更のフェードインを実行中
	bool m_isFadeIn;

	//	シーン変更のフェードアウトを実行中
	bool m_isFadeOut;

	//	変更先のシーンクラスのポインタ
	Scene* m_nextScene;

};