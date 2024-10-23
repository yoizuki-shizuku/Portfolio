/*/
 *  ファイル名		：	PlaySceneCamera.cpp
 *  概要			：	PlayScene専用のカメラ
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/26
/*/
#include "pch.h"
#include "PlaySceneCamera.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"
#include "Library/Easing.h"

//	撃破演出の時間
#define EVENT_TIME	(3.f)

PlaySceneCamera::PlaySceneCamera() :
	m_eventTimer(0.f)
{
}

void PlaySceneCamera::Initialize()
{
	CreateProjection();

	m_up = SimpleMath::Vector3::Up;

	//	初期位置
	m_rotation = SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PIDIV2, 0.f, 0.f);

	//	オブザーバーを生成
	m_claerObs = std::make_unique<Observer>();
	m_failureObs = std::make_unique<Observer>();
}

void PlaySceneCamera::Update()
{
	//	敵の撃破の通知を受け取ったら撃破カメラにする
	if (m_claerObs->GetNotification())
	{
		WinUpdate();
	}
	//	プレイヤーの失敗通知を受け取ったら演出のカメラへ切り替える
	else if (m_failureObs->GetNotification())
	{
		FailureUpdate();
	}
	//	通常時
	else
	{
		DefaultUpdate();
	}

	//	ビュー行列を計算
	m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

void PlaySceneCamera::Finalize()
{
}

void PlaySceneCamera::CreateProjection()
{
	//	画面サイズ取得
	RECT rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f,
		150.f
	);
}

void PlaySceneCamera::DefaultUpdate()
{
	//	注視点を設定
	m_target = SimpleMath::Vector3::Lerp(m_enemyPosition, m_playerPosition, 0.5f);

	//	敵とプレイヤーの距離を計算
	float distance = SimpleMath::Vector3::Distance(m_playerPosition, m_enemyPosition);

	//	ターゲットに対するカメラの基本の位置
	SimpleMath::Vector3 eye(0.0f, 2.f, 3.0f + (distance * 0.5f));

	//	ターゲットの位置からプレイヤーがどの角度にいるか計算（Yは使わない）
	float rad = Useful::GetAngle(m_playerPosition.x, m_playerPosition.z, m_enemyPosition.x, m_enemyPosition.z);
	rad += XM_PI / 2.f;

	//	クォータニオンに変換
	SimpleMath::Quaternion radToQuat = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	徐々に回転させていく
	m_rotation = SimpleMath::Quaternion::Lerp(m_rotation, radToQuat, 0.05f);

	//	回転行列に変換
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_rotation);

	//	角度を適用
	eye = SimpleMath::Vector3::Transform(eye, rotate.Invert());

	//カメラの方向ベクトルから正面方向ベクトルを取得する（Yは不要なので0）
	m_front = SimpleMath::Vector3(-eye.x, 0.0f, -eye.z);
	//正面方向ベクトルを正規化
	m_front.Normalize();

	//	カメラの位置をターゲットに対する角度と位置から計算
	m_eye = eye + m_target;
}

void PlaySceneCamera::WinUpdate()
{
	//	演出時間終了
	if (m_eventTimer >= EVENT_TIME) return;

	//	タイマー更新
	m_eventTimer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	2点間の中心座標
	SimpleMath::Vector3 center = SimpleMath::Vector3::Lerp(m_playerPosition, m_enemyPosition, 0.5f);
	m_target = center;

	//	敵とプレイヤーの距離を計算
	float distance = SimpleMath::Vector3::Distance(m_playerPosition, m_enemyPosition);
	//	ターゲットに対するカメラの基本の位置
	SimpleMath::Vector3 eye(1.0f, 3.f, 5.0f + (distance * 0.5f));

	//	ターゲットの位置からプレイヤーがどの角度にいるか計算（Yは使わない）
	float rad = Useful::GetAngle(m_playerPosition.x, m_playerPosition.z, m_enemyPosition.x, m_enemyPosition.z);
	rad += XM_PI / 2.f;

	//	現在の角度に足す分の角度を計算
	float ratio = Useful::Ratio01(m_eventTimer, EVENT_TIME);
	float offset = Easing::OutInQuart(0.f, XM_2PI, ratio);
	float angle = rad + offset;

	//	回転行列に変換
	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(angle, 0.f, 0.f);
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(q);

	m_eye = m_target + SimpleMath::Vector3::Transform(eye, rotate.Invert());
}

void PlaySceneCamera::FailureUpdate()
{
	//	プレイヤーをターゲットにする
	m_target = m_playerPosition;
	m_target.y = m_playerPosition.y + 0.3f;

	//	ターゲットに対するカメラの基本の位置
	SimpleMath::Vector3 eye(0.0f, 1.5f, 3.f);

	//	最後に向いていた方向を基準に回転させる
	SimpleMath::Quaternion offset = SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PI, 0.f, 0.f);
	SimpleMath::Quaternion standard = m_rotation;
	standard = offset * standard;

	//	回転行列に変換
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(standard);

	//	適用
	m_eye = m_target + SimpleMath::Vector3::Transform(eye, rotate.Invert());
}
