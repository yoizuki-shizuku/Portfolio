/*/
 *  ファイル名		：	Collector.cpp
 *  概要			：	ステージ2の敵
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/23
/*/
#include "pch.h"
#include "Collector.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "../Bullets/NormalBullet.h"
#include "../Bullets/TorpedoBullet.h"
#include "../Bullets/HomingBullet.h"
#include <random>

//	魚雷弾の発射間隔
#define TORPEDO_INTERVAL		(1.6f)
//	魚雷弾の連射時の発射間隔
#define TORPEDO_RAPIDFIRE		(0.2f)
//	魚雷弾の発射位置差分
#define TORPEDO_SHOT_POS		(SimpleMath::Vector3(1.f, 0.f, 0.f))
//	魚雷弾の最大速度
#define TORPEDO_SPEED			(20.f)
//	魚雷弾の発射数
#define TORPEDO_COUNT			(4)

//	追尾弾の発射間隔
#define HOMING_INTERVAL			(2.f)
//	追尾弾の速度
#define HOMING_SPEED			(10.f)

//	連射中の通常弾
#define RAPIDFIAR_INTERVAL		(0.08f)
//	連射終了後の通常弾
#define NORMAL_INTERVAL			(1.f)
//	通常弾のスピード
#define NORMAL_SPEED			(15.f)
//	通常弾の発射数
#define NORMAL_COUNT			(50)

//	モデルの位置
#define OFFSET_POS			(SimpleMath::Vector3(0.f, 1.2f, 0.f))

Collector::Collector() :
	Enemy(),
	m_player(nullptr),
	m_shotTimer(0.f),
	m_canShotTime(TORPEDO_INTERVAL),
	m_torpedoCount(0),
	m_normalCount(0)
{
}

void Collector::Initialize(Player* player, EnemyBulletManager* enemyBulletManager)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	モデル作成
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Robot_Collector.cmo", *fx);

	//	当たり判定を作成
	SetCollider(new Sphere(0.6f));

	//	ポインタを保持しておく
	m_player = player;
	m_enemyBulletManger = enemyBulletManager;

	//	初期化
	m_shotTimer = 0.f;
	SetOffset(OFFSET_POS);
	SetPosition(SimpleMath::Vector3(0.f, -0.3f, 0.f));

	//	ビヘイビアツリー作成
	CreateBehaviour();

	//	影作成
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(GetPosition());
}

void Collector::Update(float elapsedTime)
{
	//	自身が撃破されたら処理を止める
	if (GetObserver()->GetNotification()) return;

	//	タイマーの更新
	m_shotTimer += elapsedTime;

	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();
	//	角度に変換
	float rad = atan2f(toPlayerVec.x, toPlayerVec.z);

	//	クォータニオンに変換
	SimpleMath::Quaternion look = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	徐々に回転
	m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, look, 0.05f);

	//	行動パターン制御
	m_tree->Invoke();

	//	影を移動
	m_shadow->Update(GetPosition());
}

void Collector::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateFromQuaternion(m_quaternion);
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	m_model->Draw(context, *states, world, view, proj);

	m_shadow->Render(view, proj);
}

void Collector::Finalize()
{
	m_shadow->Finalize();
}

void Collector::RetryGame()
{
	m_shotTimer = 0.f;
	m_canShotTime = TORPEDO_INTERVAL;
	m_torpedoCount = 0;
	m_normalCount = 0;
	m_quaternion = SimpleMath::Quaternion::Identity;
	m_tree->ReleaseRunning();
}

void Collector::CreateBehaviour()
{
	m_tree = std::make_unique<TreeManager>();

	//	ルートノード
	Selector* root = new Selector;
	m_tree->Register(root);

		//	攻撃
		Sequence* attack = new Sequence;
		root->AddChild(attack);

			//	弾を撃てるかどうか
			attack->AddChild(new Condition([&] {return ShotBulletCondition(); }));

			//	弾の種類を選択
			Selector* shotKind = new Selector;
			attack->AddChild(shotKind);

				//	魚雷弾
				Condition* torpedo = new Condition([&] {return TorpedoBulletCondition(); });
				shotKind->AddChild(torpedo);
					torpedo->AddChild(new Action([&] { return ShotTorpedoBullet(); }));

				//	追尾弾
				Condition* homing = new Condition([&] {return HomingBulletCondition(); });
				shotKind->AddChild(homing);
					homing->AddChild(new Action([&] {return ShotHomingBullet(); }));

				//	通常弾連射
				shotKind->AddChild(new Action([&] {return RapidFireNormalBullet(); }));

		//	バリア
		Condition* barrier = new Condition([&] {return BarrierCondition(); });
		root->AddChild(barrier);
			barrier->AddChild(new Action([&] {return PutUpBarrier(); }));
}

NodeJudge Collector::ShotBulletCondition()
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

NodeJudge Collector::TorpedoBulletCondition()
{
	//	チャージ割合が40%以下なら魚雷弾を撃つ
	if (m_player->GetShotRatio() <= 0.4)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}
}

NodeStatus Collector::ShotTorpedoBullet()
{
	//	5発の魚雷弾を位置やタイミングをずらして放つ
	
	//	プレイヤーへの方向ベクトル
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	上のベクトルを基準に回転させるクォータニオンを生成
	SimpleMath::Quaternion rotate = SimpleMath::Quaternion::CreateFromAxisAngle(toPlayerVec, XM_PI / 4 * m_torpedoCount);

	//	自機の向いている方向へ発射位置差分を回転
	SimpleMath::Vector3 offset = SimpleMath::Vector3::Transform(TORPEDO_SHOT_POS, m_quaternion);

	//	発射位置をさらにずらす
	offset = SimpleMath::Vector3::Transform(offset, rotate);

	//	発射位置
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS + offset;

	//	弾の進む方向
	SimpleMath::Vector3 velocity = m_player->GetCenter() - bulletPos;
	velocity.Normalize();
	velocity *= TORPEDO_SPEED;

	//	弾の生成と制御の委譲
	std::unique_ptr<TorpedoBullet> bullet = std::make_unique<TorpedoBullet>(bulletPos,velocity);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	クールタイム設定
	m_shotTimer = 0.f;

	//	5回目
	if (m_torpedoCount >= TORPEDO_COUNT)
	{
		m_torpedoCount = 0;
		m_canShotTime = TORPEDO_INTERVAL;

		return NodeStatus::SUCCESS;
	}
	else
	{
		//	発射回数を更新
		m_torpedoCount++;
		m_canShotTime = TORPEDO_RAPIDFIRE;

		return NodeStatus::RUNNING;
	}
}

NodeJudge Collector::HomingBulletCondition()
{
	//	チャージ割合が80%以下なら魚雷弾を撃つ
	if (m_player->GetShotRatio() <= 0.8)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}
}

NodeStatus Collector::ShotHomingBullet()
{
	//	弾を生成
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<HomingBullet> bullet = std::make_unique<HomingBullet>(bulletPos, m_player, HOMING_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	クールタイム設定
	m_shotTimer = 0.f;
	m_canShotTime = HOMING_INTERVAL;

	return NodeStatus::SUCCESS;
}

NodeStatus Collector::RapidFireNormalBullet()
{
	//	通常弾を連射する 
	
	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	弾を生成
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, toPlayerVec * NORMAL_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	クールタイム設定
	m_shotTimer = 0.f;

	//	連射終了
	if (m_normalCount >= NORMAL_COUNT)
	{
		m_normalCount = 0;
		m_canShotTime = NORMAL_INTERVAL;

		return NodeStatus::SUCCESS;
	}
	//	連射中
	else
	{
		//	発射回数を更新
		m_normalCount++;
		m_canShotTime = RAPIDFIAR_INTERVAL;

		return NodeStatus::RUNNING;
	}
}

NodeJudge Collector::BarrierCondition()
{
	return NodeJudge();
}

NodeStatus Collector::PutUpBarrier()
{
	return NodeStatus();
}
