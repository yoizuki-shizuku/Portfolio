/*/
 *  ファイル名		：	Volume.h
 *  概要			：	Optionで音の大きさを変更する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/02/07
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Button.h"

class Volume
{
public:
	Volume();
	~Volume() = default;

	void Initialize(float volume, float offsetY);
	void Update();
	void Render();

	float GetVolume() { return m_volume; }

private:
	std::unique_ptr<UserInterface> m_Plus;
	std::unique_ptr<UserInterface> m_Minus;
	std::unique_ptr<Button> m_PlusButton;
	std::unique_ptr<Button> m_MinusButton;
	std::unique_ptr<UserInterface> m_volumeGauge;
	std::unique_ptr<UserInterface> m_volumeFrame;

	//	現在のボリューム
	float m_volume;

};
