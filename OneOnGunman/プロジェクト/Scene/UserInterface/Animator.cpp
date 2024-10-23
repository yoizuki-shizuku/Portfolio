/*/
 *  ファイル名		：	Animator.cpp
 *  概要			：	アニメーションをファイルから生成、保持しておき、実行していくクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
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
	//	ファイルを開く
	std::ifstream ifs;
	ifs.open(path, std::ios::in | std::ios::binary);

	//	開けなければ中断
	assert(ifs);

	//	読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();

	//	読み込んだデータをオブジェクトへ変換
	picojson::object obj = val.get<picojson::object>();

	//	実行時間
	m_requiredTime = static_cast<float>(obj["RequiredTime"].get<double>());
	//	ループするかどうか
	m_isLoop = obj["Loop"].get<bool>();
	//	相対アニメーションかどうか
	m_relative = obj["Relative"].get<bool>();

	//	MoveCommandを生成
	picojson::array move = obj["Move"].get<picojson::array>();
	MakeMoveCommand(move);
	//	RotationCommandを生成
	picojson::array rote = obj["Rotation"].get<picojson::array>();
	MakeRotationCommand(rote);
	//	ScaleCommandを生成
	picojson::array scale = obj["Scale"].get<picojson::array>();
	MakeScaleCommand(scale);
	//	AlphaCommandを生成
	picojson::array alpha = obj["Alpha"].get<picojson::array>();
	MakeAlphaCommand(alpha);
}

void Animator::Update(float elapsedTime)
{
	//	アニメーションを実行中でなければ処理しない
	if (!m_isAnimation) return;

	//	タイマーの更新
	if (m_reverse) m_timer -= elapsedTime;
	else m_timer += elapsedTime;

	//	タイマーが所要時間を超えていたら追加の処理
	if (m_timer > m_requiredTime && !m_reverse)
	{
		//	ループするかどうかで処理を変える
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
		//	ループするかどうかで処理を変える
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

	//	各コマンドの更新
	for (std::vector<std::unique_ptr<UIAnimation>>::iterator it = m_commands.begin(); it != m_commands.end(); it++)
	{
		//	コマンドの実行されるタイミングでなければ処理しない
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

	//	各コマンドの更新
	for (std::vector<std::unique_ptr<UIAnimation>>::iterator it = m_commands.begin(); it != m_commands.end(); it++)
	{
		//	コマンドの実行されるタイミングでなければ処理しない
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
		//	代入用変数
		double sx, sy, ex, ey, st, et, easing;
		//	オブジェクト型に変換して使いやすく
		picojson::object obj = it->get<picojson::object>();

		//	各変数へ読み込む
		sx     = obj["StartPosX"].get<double>();
		sy     = obj["StartPosY"].get<double>();
		ex     = obj["EndPosX"].get<double>();
		ey     = obj["EndPosY"].get<double>();
		st     = obj["StartTime"].get<double>();
		et     = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	変数へ代入
		SimpleMath::Vector2 startPos(static_cast<float>(sx), static_cast<float>(sy));
		SimpleMath::Vector2 endPos(static_cast<float>(ex), static_cast<float>(ey));
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	相対アニメーションなら初期位置を設定
		if (m_relative)
		{
			startPos += m_userInterface->GetPosition();
			endPos += m_userInterface->GetPosition();
		}

		//	コマンドを生成
		std::unique_ptr<MoveAnimation> command = 
			std::make_unique<MoveAnimation>(m_userInterface, startPos, endPos, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	コマンドを設定
		m_commands.emplace_back(std::move(command));
	}
}

void Animator::MakeRotationCommand(picojson::array rota)
{
	for (picojson::array::iterator it = rota.begin(); it != rota.end(); it++)
	{
		//	代入用変数
		double sd, ed, st, et, easing;
		//	オブジェクト型に変換して使いやすく
		picojson::object obj = it->get<picojson::object>();

		//	各変数へ読み込む
		sd     = obj["StartDegree"].get<double>();
		ed     = obj["EndDegree"].get<double>();
		st     = obj["StartTime"].get<double>();
		et     = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	変数へ代入
		float startDegree = static_cast<float>(sd);
		float endDegree = static_cast<float>(ed);
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	相対アニメーションなら初期位置を設定
		if (m_relative)
		{
			startDegree += m_userInterface->GetDegree();
			endDegree += m_userInterface->GetDegree();
		}

		//	コマンドを生成
		std::unique_ptr<RotationAnimation> command =
			std::make_unique<RotationAnimation>(m_userInterface, startDegree, endDegree
				, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	コマンドを設定
		m_commands.emplace_back(std::move(command));
	}
}

void Animator::MakeScaleCommand(picojson::array scale)
{
	for (picojson::array::iterator it = scale.begin(); it != scale.end(); it++)
	{
		//	代入用変数
		double sx, sy, ex, ey, st, et, easing;
		//	オブジェクト型に変換して使いやすく
		picojson::object obj = it->get<picojson::object>();

		//	各変数へ読み込む
		sx = obj["StartScaleX"].get<double>();
		sy = obj["StartScaleY"].get<double>();
		ex = obj["EndScaleX"].get<double>();
		ey = obj["EndScaleY"].get<double>();
		st = obj["StartTime"].get<double>();
		et = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	変数へ代入
		SimpleMath::Vector2 startScale(static_cast<float>(sx), static_cast<float>(sy));
		SimpleMath::Vector2 endScale(static_cast<float>(ex), static_cast<float>(ey));
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	相対アニメーションなら初期位置を設定
		if (m_relative)
		{
			startScale += m_userInterface->GetScale();
			endScale += m_userInterface->GetScale();
		}

		//	コマンドを生成
		std::unique_ptr<ScaleAnimation> command =
			std::make_unique<ScaleAnimation>(m_userInterface, startScale, endScale, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	コマンドを設定
		m_commands.emplace_back(std::move(command));
	}
}

void Animator::MakeAlphaCommand(picojson::array alpha)
{
	for (picojson::array::iterator it = alpha.begin(); it != alpha.end(); it++)
	{
		//	代入用変数
		double sa, ea, st, et, easing;
		//	オブジェクト型に変換して使いやすく
		picojson::object obj = it->get<picojson::object>();

		//	各変数へ読み込む
		sa = obj["StartAlpha"].get<double>();
		ea = obj["EndAlpha"].get<double>();
		st = obj["StartTime"].get<double>();
		et = obj["EndTime"].get<double>();
		easing = obj["Easing"].get<double>();

		//	変数へ代入
		float startAlpha = static_cast<float>(sa);
		float endAlpha = static_cast<float>(ea);
		std::function<float(float)> easingFunc = Easing::EASING_ARRAY[static_cast<int>(easing)];

		//	相対アニメーションなら初期位置を設定
		if (m_relative)
		{
			startAlpha += m_userInterface->GetAlpha();
			endAlpha += m_userInterface->GetAlpha();
		}

		//	コマンドを生成
		std::unique_ptr<AlphaAnimation> command =
			std::make_unique<AlphaAnimation>(m_userInterface, startAlpha, endAlpha
				, static_cast<float>(st), static_cast<float>(et), easingFunc);

		//	コマンドを設定
		m_commands.emplace_back(std::move(command));
	}
}
