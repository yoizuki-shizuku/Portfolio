/*/
 *  ファイル名		：	Guardian.cpp
 *  概要			：	ステージ1の敵
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/24
/*/
#include "pch.h"
#include "Guardian.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "../Bullets/NormalBullet.h"
#include <random>

//	弾の発射間隔
#define SHOT_INTERVAL_DEF		(2.5f)
//	弾のスピード
#define BULLET_SPEED			(10.f)
//	モデルの位置
#define OFFSET_POS	(SimpleMath::Vector3(0.f, 1.f, 0.f))
//	始めの移動可能までの時間
#define CAN_MOVE_TIME_DEF		(2.5f)
//	移動可能範囲
#define MOVE_RANGE_MIN			(-4.f)
#define MOVE_RANGE_MAX			(4.f)

Guardian::Guardian() :
	Enemy(),
	m_player(nullptr),
	m_elapsedTime(0.f),
	m_moveTimer(0.f),
	m_shotTimer(0.f),
	m_canMoveTime(CAN_MOVE_TIME_DEF),
	m_canShotTime(SHOT_INTERVAL_DEF),
	m_moveCount(0)
{
}

void Guardian::Initialize(Player* player, EnemyBulletManager* enemyBulletManager)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	モデル作成
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Robot_Guardian.cmo", *fx);

	//	当たり判定を作成
	SetCollider(new Sphere(0.6f));

	//	ポインタを保持しておく
	m_player = player;
	m_enemyBulletManger = enemyBulletManager;

	//	初期化
	m_nextPos = SimpleMath::Vector3::Zero;
	SetOffset(OFFSET_POS);
	SetPosition(SimpleMath::Vector3::Zero);

	//	ビヘイビアツリー作成
	CreateBehaviour();

	//	影作成
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(GetPosition());
}

void Guardian::Update(float elapsedTime)
{
	//	自身が撃破されたら処理を止める
	if (GetObserver()->GetNotification()) return;

	//	タイマーの更新
	m_moveTimer += elapsedTime;
	m_shotTimer += elapsedTime;


	//	受け渡し用
	m_elapsedTime = elapsedTime;

	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();
	//	角度に変換
	float rad = atan2f(toPlayerVec.x, toPlayerVec.z);

	//	クォータニオンに変換
	SimpleMath::Quaternion look = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	徐々に回転
	m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, look, 0.1f);

	//	行動パターン制御
	m_tree->Invoke();

	//	影を移動
	m_shadow->Update(GetPosition());
}

void Guardian::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateFromQuaternion(m_quaternion);
	world *= SimpleMath::Matrix::CreateTranslation(GetPosition());

	m_model->Draw(context, *states, world, view, proj);

	m_shadow->Render(view, proj);
}

void Guardian::Finalize()
{
	m_shadow->Finalize();
}

void Guardian::RetryGame()
{
	m_moveTimer = 0.f;
	m_shotTimer = 0.f;
	m_canMoveTime = CAN_MOVE_TIME_DEF;
	m_canShotTime = SHOT_INTERVAL_DEF;
	m_nextPos = SimpleMath::Vector3::Zero;
	SetPosition(SimpleMath::Vector3::Zero);
	m_quaternion = SimpleMath::Quaternion::Identity;
	m_shadow->Update(GetPosition());
	m_tree->ReleaseRunning();
}

void Guardian::CreateBehaviour()
{
	m_tree = std::make_unique<TreeManager>();

	//	ルートノード
	Selector* root = new Selector;
		
		//	目的地の設定
		Sequence* destination = new Sequence;
			destination->AddChild(new Condition([&] { return DestinationCondition(); }));
			destination->AddChild(new Action([&] { return Destination(); }));
			destination->AddChild(new Action([&] { return SettingMoveTimer(); }));
		root->AddChild(destination);

		//	移動と攻撃
		Sequence* attack = new Sequence;
			attack->AddChild(new Action([&] { return Move(); }));
			attack->AddChild(new Condition([&] { return ShotBulletCondition(); }));
			//	攻撃の種類を選択
			Selector* shotKind = new Selector;
				//	通常弾
				Condition* normal = new Condition([&] { return NormalBulletCondition(); });
					normal->AddChild(new Action([&] { return ShotNormalBullet(); }));
				shotKind->AddChild(normal);
				//	3Way弾
				Condition* threeWay = new Condition([&] { return ThreeWayCondition(); });
					threeWay->AddChild(new Action([&] { return ShotThreeWay(); }));
				shotKind->AddChild(threeWay);
				//	ランダム弾
				shotKind->AddChild(new Action([&]{ return ShotRandomBullet(); }));
			attack->AddChild(shotKind);
			attack->AddChild(new Action([&] { return SettingShotTimer(); }));
		root->AddChild(attack);

	m_tree->Register(root);
}

NodeJudge Guardian::DestinationCondition()
{
	float dis = SimpleMath::Vector3::Distance(GetPosition(), m_nextPos);

	//	移動先に到達しているかどうか
	if (abs(dis) >= 0.1f) return NodeJudge::FAILURE;

	//	指定の時間が経過しているかどうか
	if (m_moveTimer <= m_canMoveTime) return NodeJudge::FAILURE;

	return NodeJudge::SUCCESS;
}

NodeStatus Guardian::Destination()
{
	//	次の移動先を決定
	std::random_device seed;
	std::mt19937	   rand{ seed() };

	std::uniform_real_distribution<> distY(0.f, XM_2PI);

	//	移動先が決まるまで再抽選
	while (true)
	{
		//	角度をランダムに決定
		float angle = static_cast<float>(distY(rand));

		//	移動先候補を生成
		float nextX = GetPosition().x + 3.f * sin(angle);
		float nextZ = GetPosition().z + 3.f * cos(angle);

		//	範囲内に収まれば決定
		bool inRangeX = nextX < MOVE_RANGE_MAX && nextX > MOVE_RANGE_MIN;
		bool inRangeZ = nextZ < MOVE_RANGE_MAX && nextZ > MOVE_RANGE_MIN;

		if (inRangeX && inRangeZ)
		{
			m_nextPos.x = nextX;
			m_nextPos.z = nextZ;
			break;
		}
	}

	return NodeStatus::SUCCESS;
}

NodeStatus Guardian::SettingMoveTimer()
{
	//	タイマーをリセット
	m_moveTimer = 0.f;

	//	チャージの割合で移動可能までの時間を変更
	if (m_player->GetShotRatio() < 0.5f)
	{
		m_canMoveTime = CAN_MOVE_TIME_DEF;
	}
	else if (m_player->GetShotRatio() < 0.8f)
	{
		m_canMoveTime = CAN_MOVE_TIME_DEF / 2;
	}
	else
	{
		m_canMoveTime = CAN_MOVE_TIME_DEF / 10 + CAN_MOVE_TIME_DEF * (m_moveCount == 5);
		m_moveCount = Useful::Loop(m_moveCount + 1, 0, 5);
	}

	return NodeStatus::SUCCESS;
}

NodeStatus Guardian::Move()
{
	//	進行方向
	SimpleMath::Vector3 toVec = m_nextPos - GetPosition();

	//	移動する距離
	SimpleMath::Vector3 velocity = toVec * 4.f * m_elapsedTime;

	//	適用
	SetPosition(GetPosition() + velocity);

	return NodeStatus::SUCCESS;
}

NodeJudge Guardian::ShotBulletCondition()
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

NodeJudge Guardian::NormalBulletCondition()
{
	//	チャージ割合が50%以下なら通常弾を撃つ
	if (m_player->GetShotRatio() <= 0.5)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}
}

NodeJudge Guardian::ShotNormalBullet()
{
	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	弾を生成
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, toPlayerVec * BULLET_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	return NodeJudge::SUCCESS;
}

NodeJudge Guardian::ThreeWayCondition()
{
	//	チャージ割合が90%以下なら通常弾を撃つ
	if (m_player->GetShotRatio() <= 0.8)
	{
		return NodeJudge::SUCCESS;
	}
	else
	{
		return NodeJudge::FAILURE;
	}

}

NodeStatus Guardian::ShotThreeWay()
{
	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	弾を生成
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, toPlayerVec * BULLET_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateRotationY(XM_PIDIV4 / 4);
	SimpleMath::Vector3 shotOffset = SimpleMath::Vector3::Transform(toPlayerVec, rotate);
	std::unique_ptr<NormalBullet> bullet2 = std::make_unique<NormalBullet>(bulletPos, shotOffset * BULLET_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet2));

	rotate = SimpleMath::Matrix::CreateRotationY(-XM_PIDIV4 / 4);
	shotOffset = SimpleMath::Vector3::Transform(toPlayerVec, rotate);
	std::unique_ptr<NormalBullet> bullet3 = std::make_unique<NormalBullet>(bulletPos, shotOffset * BULLET_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet3));

	return NodeJudge::SUCCESS;
}

NodeStatus Guardian::ShotRandomBullet()
{
	//	ランダムな角度を生成
	std::random_device seed;
	std::mt19937	   rand{ seed() };

	std::uniform_real_distribution<> distY(-XM_PIDIV4, XM_PIDIV4);
	float angle = static_cast<float>(distY(rand));

	//	プレイヤーへの方向ベクトルを求める
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	角度をプレイヤーへの方向ベクトルに足す
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateRotationY(angle);
	SimpleMath::Vector3 shotVec = SimpleMath::Vector3::Transform(toPlayerVec, rotate);

	//	弾を生成
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, shotVec * BULLET_SPEED * 0.6f);
	m_enemyBulletManger->AddBullet(std::move(bullet));


	return NodeStatus::SUCCESS;
}

NodeStatus Guardian::SettingShotTimer()
{
	//	タイマーをリセット
	m_shotTimer = 0.f;

	//	チャージの割合で移動可能までの時間を変更
	if (m_player->GetShotRatio() < 0.8f)
	{
		m_canShotTime = SHOT_INTERVAL_DEF;
	}
	else
	{
		m_canShotTime = SHOT_INTERVAL_DEF / 8.f;
	}

	return NodeStatus::SUCCESS;
}
