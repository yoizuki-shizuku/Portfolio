/*/
 *  ファイル名		：	Invader.cpp
 *  概要			：	ステージ3の敵
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/23
/*/
#include "pch.h"
#include "Invader.h"

using namespace DirectX;

#include "Library/Easing.h"

#include "Library/UserResources.h"
#include "../Bullets/NormalBullet.h"
#include "../Bullets/WaveBullet.h"
#include <random>

//	ウェーブ弾の発射間隔
#define WAVE_INTERVAL		(1.5f)
//	ウェーブ弾のスピード
#define WAVE_SPEED			(5.f)

//	モデルの位置
#define OFFSET_POS	(SimpleMath::Vector3(0.f, 1.2f, 0.f))
//	移動速度
#define MOVE_SPEED		(3.f)
//	移動可能範囲
#define MOVE_RANGE_MIN		(-7.f)
#define MOVE_RANGE_MAX		(7.f)

Invader::Invader() :
	Enemy(),
	m_player(nullptr),
	m_shotTimer(0.f),
	m_canShotTime(WAVE_INTERVAL),
	m_elapsedTime(0.f)
{
}

void Invader::Initialize(Player* player, EnemyBulletManager* enemyBulletManager)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	モデル作成
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Robot_Invader.cmo", *fx);

	//	当たり判定を作成
	SetCollider(new Sphere(0.5f));

	//	ポインタを保持しておく
	m_player = player;
	m_enemyBulletManger = enemyBulletManager;

	//	初期化
	m_shotTimer = 0.f;
	m_angle = SimpleMath::Quaternion::Identity;
	m_velocity = SimpleMath::Vector3::Zero;
	SetOffset(OFFSET_POS);
	SetPosition(SimpleMath::Vector3::Zero);

	//	ビヘイビアツリー作成
	CreateBehaviour();

	//	影作成
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(GetPosition());
}

void Invader::Update(float elapsedTime)
{
	//	自身が撃破されたら処理を止める
	if (GetObserver()->GetNotification()) return;

	//	タイマーの更新
	m_shotTimer += elapsedTime;

	//	フレーム間の経過時間を保持
	m_elapsedTime = elapsedTime;

	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();
	//	角度に変換
	float rad = atan2f(toPlayerVec.x, toPlayerVec.z);

	//	クォータニオンに変換
	SimpleMath::Quaternion look = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	徐々に回転
	m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, look, 0.08f);

	//	行動パターン制御
	m_tree->Invoke();

	//	影を移動
	m_shadow->Update(GetPosition());
}

void Invader::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateFromQuaternion(m_quaternion);
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	m_model->Draw(context, *states, world, view, proj);

	m_shadow->Render(view, proj);
}

void Invader::Finalize()
{
	m_shadow->Finalize();
}

void Invader::RetryGame()
{
	m_shotTimer = 0.f;
	m_angle = SimpleMath::Quaternion::Identity;
	m_velocity = SimpleMath::Vector3::Zero;
	SetPosition(SimpleMath::Vector3::Zero);
	m_quaternion = SimpleMath::Quaternion::Identity;
	m_shadow->Update(GetPosition());
	m_tree->ReleaseRunning();
}

void Invader::CreateBehaviour()
{
	m_tree = std::make_unique<TreeManager>();

	//	ルートノード
	Sequence* root = new Sequence;
	m_tree->Register(root);
	
		//	移動
		Selector* move = new Selector;
		root->AddChild(move);
			
			//	移動可能範囲内かどうか
			Condition* checkRange = new Condition([&] { return CheckMoveRange(); });
			move->AddChild(checkRange);
				checkRange->AddChild(new Action([&] { return NormalMove(); }));

			//	範囲内でない場合
			move->AddChild(new Action([&] { return CorrectRange(); }));

		//	弾を撃てるかどうか
		root->AddChild(new Condition([&] { return ShotBulletCondition(); }));

		//	弾の種類を選択
		Selector* shotKind = new Selector;
		root->AddChild(shotKind);
			
			//	ウェーブ弾
			Condition* wave = new Condition([&] { return WaveBulletCondition(); });
			shotKind->AddChild(wave);
				wave->AddChild(new Action([&] { return ShotWaveBullet(); }));

}

NodeJudge Invader::CheckMoveRange()
{
	//	移動可能範囲内に位置しているかどうか判定
	SimpleMath::Vector3 position = GetPosition();

	bool inRangeX = position.x < MOVE_RANGE_MAX && position.x > MOVE_RANGE_MIN;
	bool inRangeZ = position.z < MOVE_RANGE_MAX && position.z > MOVE_RANGE_MIN;

	if (inRangeX && inRangeZ)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}
}

NodeStatus Invader::NormalMove()
{
	//	進行方向をランダムに変更
	std::random_device seed;
	std::mt19937	   rand{ seed() };
	std::uniform_real_distribution<> distY(-0.02f, 0.02f);
	float angle = static_cast<float>(distY(rand));

	//	進行方向を更新
	m_angle = SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PI * angle, 0.f, 0.f) * m_angle;

	//	徘徊行動用の円上にターゲットを配置
	SimpleMath::Vector3 target = GetPosition() + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward * 1.f, m_angle);

	//	進行方向を計算
 	m_velocity = target - GetPosition();
	m_velocity.Normalize();

	//	移動
	SetPosition(GetPosition() + m_velocity * m_elapsedTime * MOVE_SPEED);

	return NodeStatus::SUCCESS;
}

NodeStatus Invader::CorrectRange()
{
	//	中心へ向かうベクトル
	SimpleMath::Vector3 toCenter = -GetPosition();
	toCenter.Normalize();

	//	現在の角度との差を求める
	SimpleMath::Quaternion difference = SimpleMath::Quaternion::LookRotation(toCenter, SimpleMath::Vector3::Up);

	//	角度を更新
	m_angle = SimpleMath::Quaternion::Lerp(m_angle, difference, m_elapsedTime);

	//	徘徊行動用の円上にターゲットを配置
	SimpleMath::Vector3 target = GetPosition() + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward * 1.f, m_angle);

	//	進行方向を計算
	m_velocity = target - GetPosition();
	m_velocity.y = 0.f;
	m_velocity.Normalize();

	//	移動
	SetPosition(GetPosition() + m_velocity * m_elapsedTime * MOVE_SPEED);

	return NodeStatus::SUCCESS;
}

NodeJudge Invader::ShotBulletCondition()
{
	//	指定の時間に達しているかどうか
	if (m_shotTimer >= m_canShotTime)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}
}

NodeJudge Invader::WaveBulletCondition()
{
	//	チャージ割合が40%以下なら通常弾を撃つ
	if (m_player->GetShotRatio() <= 0.4)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}
}

NodeStatus Invader::ShotWaveBullet()
{
	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	弾を生成
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<WaveBullet> bullet = std::make_unique<WaveBullet>(bulletPos, toPlayerVec * WAVE_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	クールタイム設定
	m_shotTimer = 0.f;
	m_canShotTime = WAVE_INTERVAL;

	return NodeStatus::SUCCESS;
}
