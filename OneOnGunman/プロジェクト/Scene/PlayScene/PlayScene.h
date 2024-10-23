/*/
 *  ファイル名		：	PlayScene.h
 *  概要			：	プレイシーンに処理するものを集めたクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/12
/*/
#pragma once

#include "../Scene.h"

#include "../Observer/Subject.h"
#include "PlaySceneCamera.h"
#include "StageObject/SkySphere.h"
#include "StageObject/Floor.h"

#include "UI/BeginEvent.h"
#include "UI/FinishEvent.h"
#include "UI/Bar.h"
#include "UI/MoveUI.h"
#include "UI/MouseUI.h"
#include "UI/Pause.h"

#include "Player/Player.h"
#include "Player/PlayerBullet.h"

#include "Enemy/Enemy.h"
#include "Enemy/EnemyBulletManager.h"

class PlayScene final : public Scene
{
public:
	struct ConstBuffer
	{
		float ratio;
		float timer;
		DirectX::SimpleMath::Vector2 none;
	};

public:
	PlayScene(int stageIndex);
	~PlayScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	void CreateShader();
	//	リトライ時のフラグ初期化
	void RetryGame();

private:
	//	カメラ
	std::unique_ptr<PlaySceneCamera> m_camera;
	//	天球
	std::unique_ptr<SkySphere> m_skySphere;
	//	床
	std::unique_ptr<Floor> m_floor;

	//	開始前演出用UI
	std::unique_ptr<BeginEvent> m_ready;
	//	終了時演出用UI
	std::unique_ptr<FinishEvent> m_finish;
	//	プレイヤーの各種クールタイムを表示するバー
	std::unique_ptr<Bar> m_bar;
	//	プレイヤーの操作(移動)を表示する
	std::unique_ptr<MoveUI> m_moveUI;
	//	プレイヤーの操作(アクション)を表示する
	std::unique_ptr<MouseUI> m_mouseUI;
	//	ポーズ画面
	std::unique_ptr<Pause> m_pause;

	//	プレイヤー
	std::unique_ptr<Player> m_player;
	//	プレイヤーの弾
	std::unique_ptr<PlayerBullet> m_playerBullet;

	//	敵
	std::unique_ptr<Enemy> m_enemy;
	//	敵の弾管理クラス
	std::unique_ptr<EnemyBulletManager> m_bulletManager;

	//	敵を倒した際に通知を送る
	std::unique_ptr<Subject> m_knockDownEnemy;
	//	敵を倒したフラグ
	bool m_downEnemyFlag;
	//	プレイヤーがやられた際に通知を送る
	std::unique_ptr<Subject> m_knockDownPlayer;
	//	プレイヤーがやられたフラグ
	bool m_downPlayerFlag;
	//	ゲームをリトライする
	bool m_isRetry;

	//	ステージ番号
	int m_stageIndex;
	//	時間の進行速度
	float m_timeSpeed;
	//	ゲーム内での経過時間
	float m_inGameTimer;


	//	後処理エフェクト
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	後処理エフェクト用のタイマー
	float m_effectTimer;

};