/*/
 *  ファイル名		：	PlayScene.cpp
 *  概要			：	プレイシーンに処理するものを集めたクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/12
/*/
#include "pch.h"
#include "PlayScene.h"
#include "Scene/StageSelectScene/StageSelectScene.h"
#include "Scene/ResultScene/ResultScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"
#include "ReadData.h"
#include "Scene/SaveData/SaveData.h"
#include "../Audio/AudioManager.h"

#include "Enemy/Enemies/Guardian.h"
#include "Enemy/Enemies/Collector.h"
#include "Enemy/Enemies/Invader.h"

//	スロー時の時間倍率
#define SLOW_TIME_SPEED		(0.3f)

PlayScene::PlayScene(int stageIndex) :
	Scene(),
	m_downEnemyFlag(false),
	m_downPlayerFlag(false),
	m_isRetry(false),
	m_stageIndex(stageIndex),
	m_timeSpeed(1.f),
	m_inGameTimer(0.f),
	m_effectTimer(0.f)
{
}

void PlayScene::Initialize()
{
	//	生成
	m_camera        = std::make_unique<PlaySceneCamera>();
	m_skySphere     = std::make_unique<SkySphere>();
	m_floor         = std::make_unique<Floor>();

	m_ready         = std::make_unique<BeginEvent>();
	m_finish        = std::make_unique<FinishEvent>();
	m_bar           = std::make_unique<Bar>();
	m_moveUI        = std::make_unique<MoveUI>();
	m_mouseUI       = std::make_unique<MouseUI>();
	m_pause         = std::make_unique<Pause>();

	m_player        = std::make_unique<Player>();
	m_playerBullet  = std::make_unique<PlayerBullet>();

	m_bulletManager = std::make_unique<EnemyBulletManager>();

	//	stageIndexに応じて敵を変更
	     if (m_stageIndex == 0) m_enemy = std::make_unique<Guardian>();
	else if (m_stageIndex == 1) m_enemy = std::make_unique<Collector>();
	else if (m_stageIndex == 2) m_enemy = std::make_unique<Invader>();

	//	初期化
	m_camera->Initialize();
	m_skySphere->Initialize();
	m_floor->Initialize();

	m_ready->Initialize();
	m_finish->Initialize();
	m_bar->Initialize();
	m_moveUI->Initialize();
	m_mouseUI->Initialize();
	m_pause->Initialize();

	m_player->Initialize(m_playerBullet.get(), &m_timeSpeed);
	m_playerBullet->Initialize();
	m_player->Update(0.016f);	//	アニメーションを約1/60秒分進めておく(表示が崩れたので対策)

	m_enemy->Initialize(m_player.get(), m_bulletManager.get());
	m_bulletManager->Initialize();

	//	敵の撃破イベントの初期化、設定
	m_knockDownEnemy = std::make_unique<Subject>();
	m_knockDownEnemy->Attach(m_camera->GetClearObserver());
	m_knockDownEnemy->Attach(m_finish->GetClearObserver());
	m_knockDownEnemy->Attach(m_bar->GetFinishObserver());
	m_knockDownEnemy->Attach(m_enemy->GetObserver());
	m_knockDownEnemy->Attach(m_moveUI->GetObserver());
	m_knockDownEnemy->Attach(m_mouseUI->GetObserver());

	//	プレイヤーの失敗イベントの初期化、設定
	m_knockDownPlayer = std::make_unique<Subject>();
	m_knockDownPlayer->Attach(m_camera->GetFailureObserver());
	m_knockDownPlayer->Attach(m_finish->GetFailureObserver());
	m_knockDownPlayer->Attach(m_bar->GetFinishObserver());
	m_knockDownPlayer->Attach(m_player->GetFailureObserver());
	m_knockDownPlayer->Attach(m_moveUI->GetObserver());
	m_knockDownPlayer->Attach(m_mouseUI->GetObserver());

	//	プレイ回数の更新
	SaveData::GetInstance()->AddTryCount(m_stageIndex);

	CreateShader();

	//	BGMの再生
	AudioManager::GetInstance()->StartBGM(CRI_BGM_CONFRONTATION);
}

void PlayScene::Update()
{
	m_camera->SetPlayerPosition(m_player->GetPosition());
	m_camera->SetEnemyPosition(m_enemy->GetPosition());
	m_camera->Update();

	//	Readyのアニメーションが終了したら処理を始める
	if (!m_ready->Update()) return;

	//	経過時間を取得
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	進行速度を適用
	float gameTime = elapsedTime * m_timeSpeed;

	//	ポーズ中かどうか
	Pause::Process isPause = Pause::Process::MAINTAIN;

	//	ミスまたは成功演出時以外で行う処理
	if (!m_downEnemyFlag && !m_downPlayerFlag)
	{
		//	時間を計測
		m_inGameTimer += gameTime;

		//	フラグの更新
		isPause = m_pause->Update();
	}

	//	演出が終了したらステージセレクトへ戻る
	FinishEvent::Process result = m_finish->Update();
	Transition* transition = Transition::GetInstance();

	//	フェードアウトが終わったらフラグを初期化、ゲームを再開
	if (transition->FinishFadeOut() && m_isRetry)
	{
		RetryGame();
		transition->StartFadeIn();
		m_isRetry = false;
		return;
	}

	//	ゲームをはじめからやり直す
	if (result == FinishEvent::Process::RETRY || isPause == Pause::Process::RESTART && !m_isRetry)
	{
		transition->StartFadeOut();
		m_isRetry = true;
	}
	//	ステージセレクトへ戻る
	else if (result == FinishEvent::Process::SELECT || isPause == Pause::Process::SELECT)
	{
		ChangeScene<StageSelectScene>(m_stageIndex);
	}
	//	クリア
	else if (result == FinishEvent::Process::CLEAR)
	{
		ChangeScene<ResultScene>(m_inGameTimer, m_player->GetShotNum(), m_stageIndex);
	}

	//	ポーズ状態なら処理を中断
	if (isPause != Pause::Process::MAINTAIN) return;

	m_player->Update(gameTime);
	m_player->SetFront(m_camera->GetFront());
	m_bar->Update(m_player->GetShotRatio(), m_player->GetDodgeRatio());
	m_moveUI->Update();
	m_mouseUI->Update();

	m_enemy->Update(gameTime);

	m_playerBullet->Update(gameTime);
	m_bulletManager->Update(gameTime);

	//	敵に弾が当たったら撃破イベントを通知
	if (m_enemy->GetCollider()->CheckHit(m_playerBullet->GetCollider()) && !m_downPlayerFlag)
	{
		m_knockDownEnemy->Notify();
		m_timeSpeed = SLOW_TIME_SPEED;
		m_downEnemyFlag = true;
	}

	//	プレイヤーがやられたらイベントを通知
	if (!m_downEnemyFlag)
	{
		if (m_bulletManager->CheckHitBullet(m_player->GetCollider(), m_player->GetJustDodge()))
		{
			m_knockDownPlayer->Notify();
			m_timeSpeed = SLOW_TIME_SPEED;
			m_downPlayerFlag = true;
		}
	}

	//	プレイヤーがやられたら画面をモノクロにしていく
	if (m_downPlayerFlag)
	{
		m_effectTimer += elapsedTime;
		m_timeSpeed = SLOW_TIME_SPEED;
	}

	//	クリア後は時間の進みを固定
	if (m_downEnemyFlag) m_timeSpeed = SLOW_TIME_SPEED;
}

void PlayScene::Render()
{
	//	viewとprojectionを取得
	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	//	描画
	m_skySphere->Render(view, proj);
	m_floor->Render(view, proj);

	m_player->Render(view, proj);
	m_enemy->Render(view, proj);
	m_playerBullet->Render(view, proj);
	m_bulletManager->Render(view, proj);

	m_bar->Render();
	m_moveUI->Render();
	m_mouseUI->Render(m_player->GetShotRatio(), m_player->GetDodgeRatio());
	m_ready->Render();
	m_finish->Render();
	m_pause->Render();

	//	画面効果
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	float ratio = Useful::Ratio01(m_effectTimer, 2.f);

	ConstBuffer cbuff;
	cbuff.ratio = ratio;
	cbuff.timer = m_effectTimer;
	//	受け渡し用バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
}

void PlayScene::Finalize()
{
	m_skySphere->Finalize();
	m_camera->Finalize();
	m_floor->Finalize();
	m_ready->Finalize();
	m_player->Finalize();
	m_playerBullet->Finalize();
	m_enemy->Finalize();
	m_bulletManager->Finalize();

	AudioManager::GetInstance()->StopBGM();
}

void PlayScene::CreateShader()
{
	//  シェーダー読み込み
	std::vector<uint8_t> psBlob;
	psBlob = DX::ReadData(L"Resources/CSO/PostProcess/Monochrome.cso");
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_postProcess.ReleaseAndGetAddressOf())
	);

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

void PlayScene::RetryGame()
{
	m_downEnemyFlag = false;
	m_downPlayerFlag = false;
	m_timeSpeed = 1.f;
	m_inGameTimer = 0.f;
	m_effectTimer = 0.f;
	m_knockDownEnemy->Reset();
	m_knockDownPlayer->Reset();

	m_camera->RetryGame();
	m_ready->RetryGame();
	m_finish->RetryGame();
	m_bar->RetryGame();
	m_moveUI->RetryGame();
	m_mouseUI->RetryGame();
	m_pause->RetryGame();
	m_player->RetryGame();
	m_playerBullet->RetryGame();
	m_enemy->RetryGame();
	m_bulletManager->RetryGame();

	SaveData::GetInstance()->AddTryCount(m_stageIndex);
}
