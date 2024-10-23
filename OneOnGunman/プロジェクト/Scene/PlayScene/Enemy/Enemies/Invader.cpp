/*/
 *  �t�@�C����		�F	Invader.cpp
 *  �T�v			�F	�X�e�[�W3�̓G
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/23
/*/
#include "pch.h"
#include "Invader.h"

using namespace DirectX;

#include "Library/Easing.h"

#include "Library/UserResources.h"
#include "../Bullets/NormalBullet.h"
#include "../Bullets/WaveBullet.h"
#include <random>

//	�E�F�[�u�e�̔��ˊԊu
#define WAVE_INTERVAL		(1.5f)
//	�E�F�[�u�e�̃X�s�[�h
#define WAVE_SPEED			(5.f)

//	���f���̈ʒu
#define OFFSET_POS	(SimpleMath::Vector3(0.f, 1.2f, 0.f))
//	�ړ����x
#define MOVE_SPEED		(3.f)
//	�ړ��\�͈�
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

	//	���f���쐬
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Robot_Invader.cmo", *fx);

	//	�����蔻����쐬
	SetCollider(new Sphere(0.5f));

	//	�|�C���^��ێ����Ă���
	m_player = player;
	m_enemyBulletManger = enemyBulletManager;

	//	������
	m_shotTimer = 0.f;
	m_angle = SimpleMath::Quaternion::Identity;
	m_velocity = SimpleMath::Vector3::Zero;
	SetOffset(OFFSET_POS);
	SetPosition(SimpleMath::Vector3::Zero);

	//	�r�w�C�r�A�c���[�쐬
	CreateBehaviour();

	//	�e�쐬
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(GetPosition());
}

void Invader::Update(float elapsedTime)
{
	//	���g�����j���ꂽ�珈�����~�߂�
	if (GetObserver()->GetNotification()) return;

	//	�^�C�}�[�̍X�V
	m_shotTimer += elapsedTime;

	//	�t���[���Ԃ̌o�ߎ��Ԃ�ێ�
	m_elapsedTime = elapsedTime;

	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();
	//	�p�x�ɕϊ�
	float rad = atan2f(toPlayerVec.x, toPlayerVec.z);

	//	�N�H�[�^�j�I���ɕϊ�
	SimpleMath::Quaternion look = SimpleMath::Quaternion::CreateFromYawPitchRoll(rad, 0.f, 0.f);

	//	���X�ɉ�]
	m_quaternion = SimpleMath::Quaternion::Lerp(m_quaternion, look, 0.08f);

	//	�s���p�^�[������
	m_tree->Invoke();

	//	�e���ړ�
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

	//	���[�g�m�[�h
	Sequence* root = new Sequence;
	m_tree->Register(root);
	
		//	�ړ�
		Selector* move = new Selector;
		root->AddChild(move);
			
			//	�ړ��\�͈͓����ǂ���
			Condition* checkRange = new Condition([&] { return CheckMoveRange(); });
			move->AddChild(checkRange);
				checkRange->AddChild(new Action([&] { return NormalMove(); }));

			//	�͈͓��łȂ��ꍇ
			move->AddChild(new Action([&] { return CorrectRange(); }));

		//	�e�����Ă邩�ǂ���
		root->AddChild(new Condition([&] { return ShotBulletCondition(); }));

		//	�e�̎�ނ�I��
		Selector* shotKind = new Selector;
		root->AddChild(shotKind);
			
			//	�E�F�[�u�e
			Condition* wave = new Condition([&] { return WaveBulletCondition(); });
			shotKind->AddChild(wave);
				wave->AddChild(new Action([&] { return ShotWaveBullet(); }));

}

NodeJudge Invader::CheckMoveRange()
{
	//	�ړ��\�͈͓��Ɉʒu���Ă��邩�ǂ�������
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
	//	�i�s�����������_���ɕύX
	std::random_device seed;
	std::mt19937	   rand{ seed() };
	std::uniform_real_distribution<> distY(-0.02f, 0.02f);
	float angle = static_cast<float>(distY(rand));

	//	�i�s�������X�V
	m_angle = SimpleMath::Quaternion::CreateFromYawPitchRoll(XM_PI * angle, 0.f, 0.f) * m_angle;

	//	�p�j�s���p�̉~��Ƀ^�[�Q�b�g��z�u
	SimpleMath::Vector3 target = GetPosition() + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward * 1.f, m_angle);

	//	�i�s�������v�Z
 	m_velocity = target - GetPosition();
	m_velocity.Normalize();

	//	�ړ�
	SetPosition(GetPosition() + m_velocity * m_elapsedTime * MOVE_SPEED);

	return NodeStatus::SUCCESS;
}

NodeStatus Invader::CorrectRange()
{
	//	���S�֌������x�N�g��
	SimpleMath::Vector3 toCenter = -GetPosition();
	toCenter.Normalize();

	//	���݂̊p�x�Ƃ̍������߂�
	SimpleMath::Quaternion difference = SimpleMath::Quaternion::LookRotation(toCenter, SimpleMath::Vector3::Up);

	//	�p�x���X�V
	m_angle = SimpleMath::Quaternion::Lerp(m_angle, difference, m_elapsedTime);

	//	�p�j�s���p�̉~��Ƀ^�[�Q�b�g��z�u
	SimpleMath::Vector3 target = GetPosition() + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward * 1.f, m_angle);

	//	�i�s�������v�Z
	m_velocity = target - GetPosition();
	m_velocity.y = 0.f;
	m_velocity.Normalize();

	//	�ړ�
	SetPosition(GetPosition() + m_velocity * m_elapsedTime * MOVE_SPEED);

	return NodeStatus::SUCCESS;
}

NodeJudge Invader::ShotBulletCondition()
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

NodeJudge Invader::WaveBulletCondition()
{
	//	�`���[�W������40%�ȉ��Ȃ�ʏ�e������
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
	//	�v���C���[�ւ̕����x�N�g�������߂�
	SimpleMath::Vector3 toPlayerVec = m_player->GetPosition() - GetPosition();
	toPlayerVec.Normalize();

	//	�e�𐶐�
	SimpleMath::Vector3 bulletPos = GetPosition() + OFFSET_POS;
	std::unique_ptr<WaveBullet> bullet = std::make_unique<WaveBullet>(bulletPos, toPlayerVec * WAVE_SPEED);
	m_enemyBulletManger->AddBullet(std::move(bullet));

	//	�N�[���^�C���ݒ�
	m_shotTimer = 0.f;
	m_canShotTime = WAVE_INTERVAL;

	return NodeStatus::SUCCESS;
}
