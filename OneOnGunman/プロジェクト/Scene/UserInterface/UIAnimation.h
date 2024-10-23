/*/
 *  ファイル名		：	UIAnimation.h
 *  概要			：	UserInterfaceで使うコマンドパターン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
/*/
#pragma once

class UIAnimation
{
public:
	UIAnimation(float startTime, float endTime) :
		m_nowTime(0.f),
		m_startTime(startTime),
		m_endTime(endTime)
	{
	}

	virtual ~UIAnimation() = default;

	virtual void Execute() = 0;

	virtual bool WithinTime(float time) final
	{
		m_nowTime = time;

		//	現在の処理時間が開始から終了までの間なら実行されるようにする
		return time >= m_startTime && time <= m_endTime;
	}

	virtual float GetRatio() final
	{
		//	割合を計算
		return (m_nowTime - m_startTime) / (m_endTime - m_startTime);
	}

private:
	//	現在の実行時間
	float m_nowTime;
	//	アニメーションが始まる時間
	float m_startTime;
	//	アニメーションが終了する時間
	float m_endTime;

};
