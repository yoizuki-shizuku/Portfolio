/*/
 *  ファイル名		：	PlaySceneCamera.h
 *  概要			：	PlayScene専用のカメラ
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/26
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

	//	リトライ時のフラグ初期化
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
	//	射影行列の作成
	void CreateProjection();
	//	通常時のカメラ挙動
	void DefaultUpdate();
	//	撃破時のカメラ挙動
	void WinUpdate();
	//	死亡時のカメラ挙動
	void FailureUpdate();

private:
	//	カメラの位置
	DirectX::SimpleMath::Vector3 m_eye;
	//	注視点の位置
	DirectX::SimpleMath::Vector3 m_target;
	//	カメラの上方向ベクトル
	DirectX::SimpleMath::Vector3 m_up;

	//	カメラの正面ベクトル（水平方向のみ）
	DirectX::SimpleMath::Vector3 m_front;

	//	ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//	射影行列
	DirectX::SimpleMath::Matrix m_proj;

	//	プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPosition;
	//	敵の位置
	DirectX::SimpleMath::Vector3 m_enemyPosition;

	//	カメラの回転角度
	DirectX::SimpleMath::Quaternion m_rotation;

	//	エネミーが倒されたのかどうかの通知を受け取る
	std::unique_ptr<Observer> m_claerObs;
	//	プレイヤーが倒されたかどうかの通知を受け取る
	std::unique_ptr<Observer> m_failureObs;
	//	撃破演出用のタイマー
	float m_eventTimer;

};