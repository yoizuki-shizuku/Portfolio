/*/
 *  ファイル名		：	Shot.cpp
 *  概要			：	ショット状態
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#include "pch.h"
#include "Shot.h"

using namespace DirectX;

#include "../Player.h"
#include "Library/UserResources.h"

//	ショット状態に入ってから他の状態に切り替わるまでの時間
#define CHANGE_TIME		(0.6f)

Shot::Shot(Player* player) :
	State(),
	m_player(player),
	m_timer(0.f)
{
	//	アニメーションの設定
	SetAnimStartTime(0.3);
	SetAnimEndTime(1.0);
	SetAnimLoop(false);
}

void Shot::Initialize()
{
	//	マウスの位置から発射方向を計算

	//	マウス座標をワールド変換
	RECT size = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	int width = size.right;
	int height = size.bottom;

	auto mouse = Mouse::Get().GetState();
	int x = mouse.x;
	int y = mouse.y;

	SimpleMath::Matrix view, proj;
	m_player->GetViewProj(&view, &proj);

	SimpleMath::Vector3 stwPos = CalcScreenToXZ(x, y, width, height, view, proj);

	//	方向ベクトルを求める
	SimpleMath::Vector3 toVec = stwPos - m_player->GetPosition();
	toVec.Normalize();
	//	角度に変換
	float rad = atan2f(toVec.x, toVec.z) + XM_PI;

	//	クォータニオンに変換
	SimpleMath::Quaternion q = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	向きを適用
	m_player->SetAngle(q);

	//	弾を発射
	m_player->ShotBullet();

	//	タイマーをリセット
	m_timer = 0.f;

	//	アニメーションをはじめからにする
	RestartAnimetion();

	//	クールタイムをリセット
	m_player->ResetShotCoolTime();
}

void Shot::Update(float elapsedTime)
{
	//	プレイヤーが生存していなければやられ状態にする
	if (!m_player->GetIsAlive())
	{
		m_player->ChangeState(m_player->GetDie());
		return;
	}

	//	時間を計測
	m_timer += elapsedTime;

	//	速度をゼロに
	m_player->SetVelocity(SimpleMath::Vector3::Zero);

	//	一定の時間がたてば状態を遷移する
	if (m_timer < CHANGE_TIME) return;

	m_player->ChangeState(m_player->GetIdle());
}

void Shot::Finalize()
{
}

DirectX::SimpleMath::Vector3 Shot::CalcScreenToWorld(
	int screen_x, int screen_y, float fZ,
	int screen_w, int screen_h,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	各行列の逆行列を算出
	SimpleMath::Matrix invView, invProj, viewport, invViewport;
	invView = view.Invert();
	invProj = proj.Invert();
	viewport = SimpleMath::Matrix::Identity;
	viewport._11 = screen_w / 2.f;
	viewport._22 = -screen_h / 2.f;
	viewport._41 = screen_w / 2.f;
	viewport._42 = screen_h / 2.f;
	invViewport = viewport.Invert();

	SimpleMath::Matrix temp = invViewport * invProj * invView;

	return SimpleMath::Vector3::Transform(
		SimpleMath::Vector3(static_cast<float>(screen_x), static_cast<float>(screen_y), fZ), temp
	);
}

DirectX::SimpleMath::Vector3 Shot::CalcScreenToXZ(
	int screen_x, int screen_y,
	int screen_w, int screen_h,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	SimpleMath::Vector3 nearPos, farPos, ray;
	nearPos = CalcScreenToWorld(screen_x, screen_y, 0.f, screen_w, screen_h, view, proj);
	farPos = CalcScreenToWorld(screen_x, screen_y, 1.f, screen_w, screen_h, view, proj);
	ray = farPos - nearPos;
	ray.Normalize();

	//	床との交差が起きている場合は交点を
	//	起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0)
	{
		float Lray = ray.Dot(SimpleMath::Vector3::Up);
		float lp0 = -nearPos.Dot(SimpleMath::Vector3::Up);
		return nearPos + (lp0 / Lray) * ray;
	}
	else
	{
		return farPos;
	}
}