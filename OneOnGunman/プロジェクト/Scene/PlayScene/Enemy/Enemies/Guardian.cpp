/*/
 *  �t�@�C����		�F	Guardian.cpp
 *  �T�v			�F	�X�e�[�W1�̓G
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/24
/*/
#include "pch.h"
#include "Guardian.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"
#include "../Bullets/NormalBullet.h"
#include <random>

//	�e�̔��ˊԊu
#define SHOT_INTERVAL_DEF		(2.5f)
//	�e�̃X�s�[�h
#define BULLET_SPEED			(10.f)
//	���f���̈ʒu
#define OFFSET_POS	(SimpleMath::Vector3(0.f, 1.f, 0.f))
//	�n�߂̈ړ��\�܂ł̎���
#define CAN_MOVE_TIME_DEF		(2.5f)
//	�ړ��\�͈�
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

	//	���f���쐬
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Robot_Guardian.cmo", *fx);

	//	�����蔻����쐬
	SetCollider(new Sphere(0.6f));

	//	�|�C���^��ێ����Ă���
	m_player = player;
	m_enemyBulletManger = enemyBulletManager;

	//	������
	m_nextPos = SimpleMath::Vector3::Zero;
	SetOffset(OFFSET_POS);
	SetPosition(SimpleMath::Vector3::Zero);

	//	�r�w�C�r�A�c���[�쐬
	CreateBehaviour();

	//	�e�쐬
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(GetPosition());
}

void Guardian::Update(float elapsedTime)
{
	//	���g�����j���ꂽ�珈�����~�߂�
	if (GetObserver()->GetNotification()) return;

	//	�^�C�}�[�̍X�V
	m_moveTimer += elapsedTime;
	m_shotTimer += elapsedTime;


	//	�󂯓n���p
	m_elapsedTime = elapsedTime;

	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();
	//	�p�x�ɕϊ�
	float rad = atan2f(toPlayerVec.x, toPlayerVec.z);

	//	�N�H�[�^�j�I���ɕϊ�
	SimpleMath::Quaternion look = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	���X�ɉ�]
	m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, look, 0.1f);

	//	�s���p�^�[������
	m_tree->Invoke();

	//	�e���ړ�
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

	//	���[�g�m�[�h
	Selector* root = new Selector;
		
		//	�ړI�n�̐ݒ�
		Sequence* destination = new Sequence;
			destination->AddChild(new Condition([&] { return DestinationCondition(); }));
			destination->AddChild(new Action([&] { return Destination(); }));
			destination->AddChild(new Action([&] { return SettingMoveTimer(); }));
		root->AddChild(destination);

		//	�ړ��ƍU��
		Sequence* attack = new Sequence;
			attack->AddChild(new Action([&] { return Move(); }));
			attack->AddChild(new Condition([&] { return ShotBulletCondition(); }));
			//	�U���̎�ނ�I��
			Selector* shotKind = new Selector;
				//	�ʏ�e
				Condition* normal = new Condition([&] { return NormalBulletCondition(); });
					normal->AddChild(new Action([&] { return ShotNormalBullet(); }));
				shotKind->AddChild(normal);
				//	3Way�e
				Condition* threeWay = new Condition([&] { return ThreeWayCondition(); });
					threeWay->AddChild(new Action([&] { return ShotThreeWay(); }));
				shotKind->AddChild(threeWay);
				//	�����_���e
				shotKind->AddChild(new Action([&]{ return ShotRandomBullet(); }));
			attack->AddChild(shotKind);
			attack->AddChild(new Action([&] { return SettingShotTimer(); }));
		root->AddChild(attack);

	m_tree->Register(root);
}

NodeJudge Guardian::DestinationCondition()
{
	float dis = SimpleMath::Vector3::Distance(GetPosition(), m_nextPos);

	//	�ړ���ɓ��B���Ă��邩�ǂ���
	if (abs(dis) >= 0.1f) return NodeJudge::FAILURE;

	//	�w��̎��Ԃ��o�߂��Ă��邩�ǂ���
	if (m_moveTimer <= m_canMoveTime) return NodeJudge::FAILURE;

	return NodeJudge::SUCCESS;
}

NodeStatus Guardian::Destination()
{
	//	���̈ړ��������
	std::random_device seed;
	std::mt19937	   rand{ seed() };

	std::uniform_real_distribution<> distY(0.f, XM_2PI);

	//	�ړ��悪���܂�܂ōĒ��I
	while (true)
	{
		//	�p�x�������_���Ɍ���
		float angle = static_cast<float>(distY(rand));

		//	�ړ�����𐶐�
		float nextX = GetPosition().x + 3.f * sin(angle);
		float nextZ = GetPosition().z + 3.f * cos(angle);

		//	�͈͓��Ɏ��܂�Ό���
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
	//	�^�C�}�[�����Z�b�g
	m_moveTimer = 0.f;

	//	�`���[�W�̊����ňړ��\�܂ł̎��Ԃ�ύX
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
	//	�i�s����
	SimpleMath::Vector3 toVec = m_nextPos - GetPosition();

	//	�ړ����鋗��
	SimpleMath::Vector3 velocity = toVec * 4.f * m_elapsedTime;

	//	�K�p
	SetPosition(GetPosition() + velocity);

	return NodeStatus::SUCCESS;
}

NodeJudge Guardian::ShotBulletCondition()
{
	//	�w��̎��ԂɒB���Ă��邩�ǂ���
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
	//	�`���[�W������50%�ȉ��Ȃ�ʏ�e������
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
	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	�e�𐶐�
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, toPlayerVec * BULLET_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	return NodeJudge::SUCCESS;
}

NodeJudge Guardian::ThreeWayCondition()
{
	//	�`���[�W������90%�ȉ��Ȃ�ʏ�e������
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
	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	�e�𐶐�
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
	//	�����_���Ȋp�x�𐶐�
	std::random_device seed;
	std::mt19937	   rand{ seed() };

	std::uniform_real_distribution<> distY(-XM_PIDIV4, XM_PIDIV4);
	float angle = static_cast<float>(distY(rand));

	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	�p�x���v���C���[�ւ̕����x�N�g���ɑ���
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateRotationY(angle);
	SimpleMath::Vector3 shotVec = SimpleMath::Vector3::Transform(toPlayerVec, rotate);

	//	�e�𐶐�
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, shotVec * BULLET_SPEED * 0.6f);
	m_enemyBulletManger->AddBullet(std::move(bullet));


	return NodeStatus::SUCCESS;
}

NodeStatus Guardian::SettingShotTimer()
{
	//	�^�C�}�[�����Z�b�g
	m_shotTimer = 0.f;

	//	�`���[�W�̊����ňړ��\�܂ł̎��Ԃ�ύX
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
