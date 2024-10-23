/*/
 *  ファイル名		：	Animator.h
 *  概要			：	アニメーションをファイルから生成、保持しておき、実行していくクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
/*/
#pragma once

#include "UIAnimation.h"
#include <array>
#include <sstream>
#include "Library/Easing.h"
#include "picojson.h"

class UserInterface;

class Animator
{
public:
	Animator(UserInterface* ui);
	~Animator() = default;

	void LoadFile(const std::string path);
	void Update(float elapsedTime);
	void Finalize();

public:
	//	アニメーションを始めから行う
	void StartAnimation() { m_isAnimation = true; m_timer = 0.f; m_reverse = false; }
	//	アニメーションを逆再生する
	void StartReverseAnimation() { m_isAnimation = true; m_timer = m_requiredTime; m_reverse = true; }
	//	アニメーションを止める
	void StopAnimation() { m_isAnimation = false; }
	//	アニメーションを再開する(途中から)
	void PlayAnimation() { m_isAnimation = true; m_reverse = false; }
	//	アニメーションを再開する(逆再生)
	void PlayReverseAnimation() { m_isAnimation = true; m_reverse = true; }
	//	アニメーションを終了する
	void FinishAnimation();
	//	アニメーションをリセットする(0秒の状態にする)
	void ResetAnimation();

	//	アニメーションが再生されているかどうか
	bool GetIsAnimation() const { return m_isAnimation; }
	//	アニメーションがすでに再生され、終了したかどうか
	//	(ループしないアニメーションのみ)
	bool GetFinishAnimation() const { return (m_timer >= m_requiredTime && !m_reverse) || (m_timer <= 0.f && m_reverse) && !m_isAnimation; }

private:
	//	MoveCommandを生成
	void MakeMoveCommand(picojson::array move);
	//	RotationCommandを生成
	void MakeRotationCommand(picojson::array rota);
	//	ScaleCommandを生成
	void MakeScaleCommand(picojson::array scale);
	//	AlphaCommandを作成
	void MakeAlphaCommand(picojson::array alpha);

private:
	//	UserInterfaceのポインタ
	UserInterface* m_userInterface;

	//	コマンド
	std::vector<std::unique_ptr<UIAnimation>> m_commands;

	//	アニメーションの実行時間
	float m_timer;
	//	読み込んだアニメーションの所要時間
	float m_requiredTime;

	//	アニメーションを実行中かどうか
	bool m_isAnimation;

	//	ループするのかどうか
	bool m_isLoop;
	//	相対アニメーションかどうか
	//	(UserInterfaceの初期状態を元にアニメーションさせる)
	//	(同じようなアニメーションを1つのファイルで実現できる)
	bool m_relative;
	//	逆再生中かどうか
	bool m_reverse;

};
