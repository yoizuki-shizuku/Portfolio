/*/
 *  �t�@�C����		�F	Player.cpp
 *  �T�v			�F	�v���C���[�𐧌䂷��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/09
/*/
#include "pch.h"
#include "Player.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Useful.h"

#include "State/Idle.h"
#include "State/Run.h"
#include "State/Shot.h"
#include "State/Dodge.h"
#include "State/Die.h"

#include "Library/Input.h"

//	�����蔻��Ƃ̈ʒu�̍�
#define COLLIDER_OFFSET_POS		(SimpleMath::Vector3(0.f, 0.5f, 0.f))
//	�e���o��ʒu
#define SHOT_OFFSET_POS			(SimpleMath::Vector3(0.f, 0.6f, 0.f))
//	�V���b�g�̃N�[���^�C��
#define SHOT_COOLTIME		(20.f)
//	����̃N�[���^�C��
#define DODGE_COOLTIME		(1.5f)
//	�v���C���[�̈ړ��\�͈�
#define MOVEING_RANGE		(9.f)

Player::Player() :
	m_playerBullet(nullptr),
	m_shotCoolTime(0.f),
	m_dodgeCoolTime(0.f),
	m_currentState(nullptr),
	m_isAlive(true),
	m_shotNum(0)
{

}

void Player::Initialize(PlayerBullet* playerBullet, float* timeSpeed)
{
	//	�����ʒu
	m_position = SimpleMath::Vector3(0.f, 0.f, 5.f);
	m_velocity = SimpleMath::Vector3::Zero;

	//	�R���C�_�[�̏����ݒ�
	m_collider = std::make_unique<Capsule>(0.6f, 0.13f, COLLIDER_OFFSET_POS);
	m_justDodge = std::make_unique<Sphere>(0.6f, COLLIDER_OFFSET_POS);

	//	PlayerBullet�N���X�̃|�C���^��ݒ�
	m_playerBullet = playerBullet;

	//	�f�o�C�X�̎擾
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	
	//	�G�t�F�N�g�t�@�N�g���[�̍쐬
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);

	//	�v���C���[�̃��f�������[�h����
	fx->SetDirectory(L"Resources/SDK/Player/");
	ModelLoaderFlags flags = ModelLoader_Clockwise | ModelLoader_IncludeBones;
	m_model = Model::CreateFromSDKMESH(device, L"Resources/SDK/Player/Defeated.sdkmesh", *fx, flags);

	//	�X�e�[�g�̍쐬
	m_idle = std::make_unique<Idle>(this);
	m_idle->SetModel(m_model.get(), L"Resources/SDK/Player/Idle.sdkmesh_anim");

	m_run = std::make_unique<Run>(this);
	m_run->SetModel(m_model.get(), L"Resources/SDK/Player/Running.sdkmesh_anim");

	m_shot = std::make_unique<Shot>(this);
	m_shot->SetModel(m_model.get(), L"Resources/SDK/Player/Shooting.sdkmesh_anim");

	m_dodge = std::make_unique<Dodge>(this, timeSpeed);
	m_dodge->SetModel(m_model.get(), L"Resources/SDK/Player/Standing Dive Forward.sdkmesh_anim");

	m_die = std::make_unique<Die>(this);
	m_die->SetModel(m_model.get(), L"Resources/SDK/Player/Falling Back Death.sdkmesh_anim");

	//	�����X�e�[�g�̐ݒ�
	m_currentState = m_idle.get();
	m_currentState->Initialize();

	//	�e�쐬
	m_shadow = std::make_unique<Shadow>();
	m_shadow->Update(m_position);
	m_shadow->SetSize(SimpleMath::Vector2::One * 0.3f);

	//	�I�u�U�[�o�[�쐬
	m_failureObs = std::make_unique<Observer>();

	//	�G�t�F�N�g�쐬
	m_chargeEffect = std::make_unique<ChargeEffect>();
	m_chargeEffect->Initialize();
}

void Player::Update(float elapsedTime)
{
	//	�X�e�[�g��nul�łȂ���Ό��݂̃X�e�[�g���X�V����
	if (m_currentState)
	{
		m_currentState->Update(elapsedTime);
		m_currentState->AnimeUpdate(elapsedTime);
	}

	//	�ʒu�̍X�V
	m_position += m_velocity * elapsedTime;
	//	�͈͊O�ɂ͏o�Ȃ��悤�ɂ���
	m_position.Clamp(SimpleMath::Vector3(-MOVEING_RANGE), SimpleMath::Vector3(MOVEING_RANGE));

	m_collider->SetPosition(m_position + COLLIDER_OFFSET_POS);
	m_justDodge->SetPosition(m_position + COLLIDER_OFFSET_POS);

	m_shadow->Update(m_position);

	m_chargeEffect->Update(elapsedTime);

	//	�N�[���^�C���̍X�V(�e�X�e�[�g�̏����r���Ȃ�X�V���Ȃ�)
	m_dodgeCoolTime += elapsedTime;
	m_shotCoolTime += elapsedTime;

	// TODO:�f�o�b�O�p�ł��ł��e�����˂ł���
	auto kbTracker = Input::GetInstace()->GetKeyboardTracker();
	if (kbTracker->IsKeyPressed(Keyboard::V))
	{
		ChangeState(m_shot.get());
	}

	// TODO:�f�o�b�O�p�Ŗ��G�ɂȂ�
	if (kbTracker->IsKeyPressed(Keyboard::C))
	{
		m_collider->SetIsValid(!m_collider->GetIsValid());
	}

	// TODO:�f�o�b�O�p�Ń`���[�W��MAX�ɂ���
	if (kbTracker->IsKeyPressed(Keyboard::B))
	{
		m_shotCoolTime = SHOT_COOLTIME;
	}

	//	�G�̒e����������������Ԃɂ���
	if (m_failureObs->GetNotification())
	{
		m_isAlive = false;
	}
}

void Player::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	�󂯓n���p
	m_view = view;
	m_proj = proj;

	//	�ʒu�Ɗp�x����world�s����쐬����
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateScale(0.005f);
	world *= SimpleMath::Matrix::CreateFromQuaternion(m_angle);
	world *= SimpleMath::Matrix::CreateTranslation(m_position);

	//	�`�悷��
	if (m_currentState) m_currentState->Render(world, view, proj);

	m_shadow->Render(view, proj);

	//	�`���[�W���������Ă�����G�t�F�N�g��\��
	if (GetShotRatio() >= 1.f) m_chargeEffect->Render();
}

void Player::Finalize()
{
	//	�O�̂��ߊJ�����������Ă���
	m_model.reset();
}

void Player::ShotBullet()
{
	SimpleMath::Vector3 shotPos = m_position + SHOT_OFFSET_POS;

	m_playerBullet->CreateBullet(shotPos, SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, m_angle));

	m_shotNum++;
}

bool Player::CanShot()
{
	return m_shotCoolTime > SHOT_COOLTIME;
}

float Player::GetShotRatio() const
{
	return Useful::Ratio01(m_shotCoolTime, SHOT_COOLTIME);
}

bool Player::CanDodge()
{
	return m_dodgeCoolTime > DODGE_COOLTIME;
}

float Player::GetDodgeRatio() const
{
	return Useful::Ratio01(m_dodgeCoolTime, DODGE_COOLTIME);
}

DirectX::SimpleMath::Vector3 Player::GetCenter()
{
	return m_position + COLLIDER_OFFSET_POS;
}

void Player::RetryGame()
{
	m_position = SimpleMath::Vector3(0.f, 0.f, 5.f);
	m_angle = SimpleMath::Quaternion::Identity;
	m_shotCoolTime = 0.f;
	m_dodgeCoolTime = 0.f;
	m_isAlive = true;
	m_shotNum = 0;
	m_collider->SetIsValid(true);
	ChangeState(m_idle.get());
	m_shadow->Update(m_position);
}

void Player::GetViewProj(DirectX::SimpleMath::Matrix* view, DirectX::SimpleMath::Matrix* proj) const
{
	*view = m_view;
	*proj = m_proj;
}

void Player::ChangeState(State* changeState)
{
	m_currentState = changeState;
	m_currentState->Initialize();
}
