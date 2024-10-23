/*/
 *  �t�@�C����		�F	Animator.cpp
 *  �T�v			�F	�A�j���[�V�������t�@�C�����琶���A�ێ����Ă����A���s���Ă����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/04
/*/
#include "pch.h"
#include "Animator.h"

using namespace DirectX;

#include "UserInterface.h"
#include <fstream>

#include "UIAnimation/MoveAnimation.h"
#include "UIAnimation/RotationAnimation.h"
#include "UIAnimation/ScaleAnimation.h"
#include "UIAnimation/AlphaAnimation.h"

Animator::Animator(UserInterface* ui) :
	m_userInterface(ui),
	m_timer(0.f),
	m_requiredTime(0.f),
	m_isAnimation(false),
	m_isLoop(false),
	m_relative(false),
	m_reverse(false)
{
	m_commands.clear();
}

void Animator::LoadFile(const std::string path)
{
	//	�t�@�C�����J��
	std::ifstream ifs;
	ifs.open(path, std::ios::in | std::ios::binary);

	//	�J���Ȃ���Β��f
	assert(ifs);

	//	�ǂݍ���
	picojson::value val;
	ifs >> val;

	//	ifs�ϐ��͂����g��Ȃ��̂ŕ���
	ifs.close();

	//	�ǂݍ��񂾃f�[�^���I�u�W�F�N�g�֕ϊ�
	picojson::object obj = val.get<picojson::object>();

	//	���s����
	m_requiredTime = static_cast<float>(obj["RequiredTime"].get<double>());
	//	���[�v���邩�ǂ���
	m_isLoop = obj["Loop"].get<bool>();
	//	���΃A�j���[�V�������ǂ���
	m_relative = obj["Relative"].get<bool>();

	//	MoveCommand�𐶐�
	picojson::array move = obj["Move"].get<picojson::array>();
	MakeMoveCommand(move);
	//	RotationCommand�𐶐�
	picojson::array rote = obj["Rotation"].get<picojson::array>();
	MakeRotationCommand(rote);
	//	ScaleCommand�𐶐�
	picojson::array scale = obj["Scale"].get<picojson::array>();
	MakeScaleCommand(scale);
	//	AlphaCommand�𐶐�
	picojson::array alpha = obj["Alpha"].get<picojson::array>();
	MakeAlphaCommand(alpha);
}

void Animator::Update(float elapsedTime)
{
	//	�A�j���[�V���������s���łȂ���Ώ������Ȃ�
	if (!m_isAnimation) return;

	//	�^�C�}�[�̍X�V
	if (m_reverse) m_timer -= elapsedTime;
	else m_timer += elapsedTime;

	//	�^�C�}�[�����v���Ԃ𒴂��Ă�����ǉ��̏���
	if (m_timer > m_requiredTime && !m_reverse)
	{
		//	���[�v���邩�ǂ����ŏ�����ς���
		if (m_isLoop)
		{
			m_timer = m_timer - m_requiredTime;
		}
		else
		{
			m_timer = m_requiredTime;
			m_isAnimation = false;
		}
	}
	else if (m_timer < 0.f && m_reverse)
	{
		//	���[�v���邩�ǂ����ŏ�����ς���
		if (m_isLoop)
		{
			m_timer = m_requiredTime + m_timer;
		}
		else
		{
			m_timer = 0.f;
			m_isAnimation = false;
		}
	}

	//	�e�R�}���h�̍X�V
	for (std::vector<std::unique_ptr<UIAnimation>>::iterator it = m_commands.begin(); it != m_commands.end(); it++)
	{
		//	�R�}���h�̎��s�����^�C�~���O�łȂ���Ώ������Ȃ�
		if (!(it->get()->WithinTime(m_timer))) continue;

		it->get()->Execute();
	}
}

void Animator::Finalize()
{
	std::vector<std::unique_ptr<UIAnimation>>::iterator it = m_commands.begin();

	while (it != m_commands.end())
	{
		it->reset();
		it = m_commands.erase(it);
	}
}

void Animator::FinishAnimation()
{
	m_isAnimation = false;
	if (m_reverse) m_timer = 0.f;
	else m_timer = m_requiredTime;

	//	�e�R�}���h�̍X�V
	for (std::vector<std::unique_ptr<UIAnimation>>::iterator it = m_commands.begin(); it != m_commands.end(); it++)
	{
		//	�R�}���h�̎��s�����^�C�~���O�łȂ���Ώ������Ȃ�
		if (!(it->get()->WithinTime(m_timer))) continue;

		it->get()->Execute();
	}
}

void Animator::ResetAnimation()
{
	m_timer = 0.f;
	m_reverse = false;
	Update(0.f);
	m_isAnimation = false;
}


void Animator::MakeMoveCommand(picojson::array move)
{
	for (picojson::array::iterator it = move.begin(); it != move.end(); it++)
	{
		//	����p�ϐ�
		double sx, sy, ex, ey, st, et, easing;
		//	�I�u�W�F�N�g�^�ɕϊ����Ďg���₷��
		picojson::object obj = it->get<picojson::object>();

		//	�e�ϐ��֓ǂݍ���
		sx     = obj["StartPosX"].get<double>();
		sy     = obj["StartPosY"].get<double>();
		ex     = obj["EndPosX"].get<double>();
		ey     = obj["EndPosY"].get<double>();
		st     = obj["StartTime"].get<double>();
		et     = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	�ϐ��֑��
		SimpleMath::Vector2 startPos(static_cast<float>(sx), static_cast<float>(sy));
		SimpleMath::Vector2 endPos(static_cast<float>(ex), static_cast<float>(ey));
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	���΃A�j���[�V�����Ȃ珉���ʒu��ݒ�
		if (m_relative)
		{
			startPos += m_userInterface->GetPosition();
			endPos += m_userInterface->GetPosition();
		}

		//	�R�}���h�𐶐�
		std::unique_ptr<MoveAnimation> command = 
			std::make_unique<MoveAnimation>(m_userInterface, startPos, endPos, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	�R�}���h��ݒ�
		m_commands.emplace_back(std::move(command));
	}
}

void Animator::MakeRotationCommand(picojson::array rota)
{
	for (picojson::array::iterator it = rota.begin(); it != rota.end(); it++)
	{
		//	����p�ϐ�
		double sd, ed, st, et, easing;
		//	�I�u�W�F�N�g�^�ɕϊ����Ďg���₷��
		picojson::object obj = it->get<picojson::object>();

		//	�e�ϐ��֓ǂݍ���
		sd     = obj["StartDegree"].get<double>();
		ed     = obj["EndDegree"].get<double>();
		st     = obj["StartTime"].get<double>();
		et     = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	�ϐ��֑��
		float startDegree = static_cast<float>(sd);
		float endDegree = static_cast<float>(ed);
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	���΃A�j���[�V�����Ȃ珉���ʒu��ݒ�
		if (m_relative)
		{
			startDegree += m_userInterface->GetDegree();
			endDegree += m_userInterface->GetDegree();
		}

		//	�R�}���h�𐶐�
		std::unique_ptr<RotationAnimation> command =
			std::make_unique<RotationAnimation>(m_userInterface, startDegree, endDegree
				, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	�R�}���h��ݒ�
		m_commands.emplace_back(std::move(command));
	}
}

void Animator::MakeScaleCommand(picojson::array scale)
{
	for (picojson::array::iterator it = scale.begin(); it != scale.end(); it++)
	{
		//	����p�ϐ�
		double sx, sy, ex, ey, st, et, easing;
		//	�I�u�W�F�N�g�^�ɕϊ����Ďg���₷��
		picojson::object obj = it->get<picojson::object>();

		//	�e�ϐ��֓ǂݍ���
		sx = obj["StartScaleX"].get<double>();
		sy = obj["StartScaleY"].get<double>();
		ex = obj["EndScaleX"].get<double>();
		ey = obj["EndScaleY"].get<double>();
		st = obj["StartTime"].get<double>();
		et = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	�ϐ��֑��
		SimpleMath::Vector2 startScale(static_cast<float>(sx), static_cast<float>(sy));
		SimpleMath::Vector2 endScale(static_cast<float>(ex), static_cast<float>(ey));
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	���΃A�j���[�V�����Ȃ珉���ʒu��ݒ�
		if (m_relative)
		{
			startScale += m_userInterface->GetScale();
			endScale += m_userInterface->GetScale();
		}

		//	�R�}���h�𐶐�
		std::unique_ptr<ScaleAnimation> command =
			std::make_unique<ScaleAnimation>(m_userInterface, startScale, endScale, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	�R�}���h��ݒ�
		m_commands.emplace_back(std::move(command));
	}
}

void Animator::MakeAlphaCommand(picojson::array alpha)
{
	for (picojson::array::iterator it = alpha.begin(); it != alpha.end(); it++)
	{
		//	����p�ϐ�
		double sa, ea, st, et, easing;
		//	�I�u�W�F�N�g�^�ɕϊ����Ďg���₷��
		picojson::object obj = it->get<picojson::object>();

		//	�e�ϐ��֓ǂݍ���
		sa = obj["StartAlpha"].get<double>();
		ea = obj["EndAlpha"].get<double>();
		st = obj["StartTime"].get<double>();
		et = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	�ϐ��֑��
		float startAlpha = static_cast<float>(sa);
		float endAlpha = static_cast<float>(ea);
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	���΃A�j���[�V�����Ȃ珉���ʒu��ݒ�
		if (m_relative)
		{
			startAlpha += m_userInterface->GetAlpha();
			endAlpha += m_userInterface->GetAlpha();
		}

		//	�R�}���h�𐶐�
		std::unique_ptr<AlphaAnimation> command =
			std::make_unique<AlphaAnimation>(m_userInterface, startAlpha, endAlpha
				, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	�R�}���h��ݒ�
		m_commands.emplace_back(std::move(command));
	}
}
