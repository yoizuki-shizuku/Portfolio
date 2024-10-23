/*/
 *  �t�@�C����		�F	Collector.cpp
 *  �T�v			�F	�X�e�[�W2�̓G
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/23
/*/
#include "pch.h"
#include "Collector.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "../Bullets/NormalBullet.h"
#include "../Bullets/TorpedoBullet.h"
#include "../Bullets/HomingBullet.h"
#include <random>

//	�����e�̔��ˊԊu
#define TORPEDO_INTERVAL		(1.6f)
//	�����e�̘A�ˎ��̔��ˊԊu
#define TORPEDO_RAPIDFIRE		(0.2f)
//	�����e�̔��ˈʒu����
#define TORPEDO_SHOT_POS		(SimpleMath::Vector3(1.f, 0.f, 0.f))
//	�����e�̍ő呬�x
#define TORPEDO_SPEED			(20.f)
//	�����e�̔��ː�
#define TORPEDO_COUNT			(4)

//	�ǔ��e�̔��ˊԊu
#define HOMING_INTERVAL			(2.f)
//	�ǔ��e�̑��x
#define HOMING_SPEED			(10.f)

//	�A�˒��̒ʏ�e
#define RAPIDFIAR_INTERVAL		(0.08f)
//	�A�ˏI����̒ʏ�e
#define NORMAL_INTERVAL			(1.f)
//	�ʏ�e�̃X�s�[�h
#define NORMAL_SPEED			(15.f)
//	�ʏ�e�̔��ː�
#define NORMAL_COUNT			(50)

//	���f���̈ʒu
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

	//	���f���쐬
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Robot_Collector.cmo", *fx);

	//	�����蔻����쐬
	SetCollider(new Sphere(0.6f));

	//	�|�C���^��ێ����Ă���
	m_player = player;
	m_enemyBulletManger = enemyBulletManager;

	//	������
	m_shotTimer = 0.f;
	SetOffset(OFFSET_POS);
	SetPosition(SimpleMath::Vector3(0.f, -0.3f, 0.f));

	//	�r�w�C�r�A�c���[�쐬
	CreateBehaviour();

	//	�e�쐬
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(GetPosition());
}

void Collector::Update(float elapsedTime)
{
	//	���g�����j���ꂽ�珈�����~�߂�
	if (GetObserver()->GetNotification()) return;

	//	�^�C�}�[�̍X�V
	m_shotTimer += elapsedTime;

	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();
	//	�p�x�ɕϊ�
	float rad = atan2f(toPlayerVec.x, toPlayerVec.z);

	//	�N�H�[�^�j�I���ɕϊ�
	SimpleMath::Quaternion look = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	���X�ɉ�]
	m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, look, 0.05f);

	//	�s���p�^�[������
	m_tree->Invoke();

	//	�e���ړ�
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

	//	���[�g�m�[�h
	Selector* root = new Selector;
	m_tree->Register(root);

		//	�U��
		Sequence* attack = new Sequence;
		root->AddChild(attack);

			//	�e�����Ă邩�ǂ���
			attack->AddChild(new Condition([&] {return ShotBulletCondition(); }));

			//	�e�̎�ނ�I��
			Selector* shotKind = new Selector;
			attack->AddChild(shotKind);

				//	�����e
				Condition* torpedo = new Condition([&] {return TorpedoBulletCondition(); });
				shotKind->AddChild(torpedo);
					torpedo->AddChild(new Action([&] { return ShotTorpedoBullet(); }));

				//	�ǔ��e
				Condition* homing = new Condition([&] {return HomingBulletCondition(); });
				shotKind->AddChild(homing);
					homing->AddChild(new Action([&] {return ShotHomingBullet(); }));

				//	�ʏ�e�A��
				shotKind->AddChild(new Action([&] {return RapidFireNormalBullet(); }));

		//	�o���A
		Condition* barrier = new Condition([&] {return BarrierCondition(); });
		root->AddChild(barrier);
			barrier->AddChild(new Action([&] {return PutUpBarrier(); }));
}

NodeJudge Collector::ShotBulletCondition()
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

NodeJudge Collector::TorpedoBulletCondition()
{
	//	�`���[�W������40%�ȉ��Ȃ狛���e������
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
	//	5���̋����e���ʒu��^�C�~���O�����炵�ĕ���
	
	//	�v���C���[�ւ̕����x�N�g��
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	��̃x�N�g������ɉ�]������N�H�[�^�j�I���𐶐�
	SimpleMath::Quaternion rotate = SimpleMath::Quaternion::CreateFromAxisAngle(toPlayerVec, XM_PI / 4 * m_torpedoCount);

	//	���@�̌����Ă�������֔��ˈʒu��������]
	SimpleMath::Vector3 offset = SimpleMath::Vector3::Transform(TORPEDO_SHOT_POS, m_quaternion);

	//	���ˈʒu������ɂ��炷
	offset = SimpleMath::Vector3::Transform(offset, rotate);

	//	���ˈʒu
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS + offset;

	//	�e�̐i�ޕ���
	SimpleMath::Vector3 velocity = m_player->GetCenter() - bulletPos;
	velocity.Normalize();
	velocity *= TORPEDO_SPEED;

	//	�e�̐����Ɛ���̈Ϗ�
	std::unique_ptr<TorpedoBullet> bullet = std::make_unique<TorpedoBullet>(bulletPos,velocity);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	�N�[���^�C���ݒ�
	m_shotTimer = 0.f;

	//	5���
	if (m_torpedoCount >= TORPEDO_COUNT)
	{
		m_torpedoCount = 0;
		m_canShotTime = TORPEDO_INTERVAL;

		return NodeStatus::SUCCESS;
	}
	else
	{
		//	���ˉ񐔂��X�V
		m_torpedoCount++;
		m_canShotTime = TORPEDO_RAPIDFIRE;

		return NodeStatus::RUNNING;
	}
}

NodeJudge Collector::HomingBulletCondition()
{
	//	�`���[�W������80%�ȉ��Ȃ狛���e������
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
	//	�e�𐶐�
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<HomingBullet> bullet = std::make_unique<HomingBullet>(bulletPos, m_player, HOMING_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	�N�[���^�C���ݒ�
	m_shotTimer = 0.f;
	m_canShotTime = HOMING_INTERVAL;

	return NodeStatus::SUCCESS;
}

NodeStatus Collector::RapidFireNormalBullet()
{
	//	�ʏ�e��A�˂��� 
	
	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	�e�𐶐�
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<NormalBullet> bullet = std::make_unique<NormalBullet>(bulletPos, toPlayerVec * NORMAL_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	�N�[���^�C���ݒ�
	m_shotTimer = 0.f;

	//	�A�ˏI��
	if (m_normalCount >= NORMAL_COUNT)
	{
		m_normalCount = 0;
		m_canShotTime = NORMAL_INTERVAL;

		return NodeStatus::SUCCESS;
	}
	//	�A�˒�
	else
	{
		//	���ˉ񐔂��X�V
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
