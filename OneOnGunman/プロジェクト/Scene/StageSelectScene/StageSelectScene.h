/*/
 *  ファイル名		：	StageSelectScene.h
 *  概要			：	ステージ選択をするシーン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/04
/*/
#pragma once

#include "../Scene.h"

#include "Library/Camera.h"
#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include "Model/HologramManager.h"
#include "Model/InfinitePlane.h"
#include "Information.h"

class StageSelectScene : public Scene
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	xのみ
	};

public:
	StageSelectScene(int stageIndex);
	~StageSelectScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	//	カメラ
	std::unique_ptr<Camera> m_camera;
	//	ステージセレクトの表示
	std::unique_ptr<UserInterface> m_stageSelect;
	//	敵のホログラム
	std::unique_ptr<HologramManager> m_hologram;
	//	背景用の無限(に見える)平面
	std::unique_ptr<InfinitePlane> m_plane;
	//	ステージ情報
	std::unique_ptr<Information> m_infomation;
	//	スタートボタン
	std::unique_ptr<UserInterface> m_start;
	std::unique_ptr<Button> m_startButton;
	std::unique_ptr<Animator> m_startAnimation;
	//	タイトルボタン
	std::unique_ptr<UserInterface> m_title;
	std::unique_ptr<Button> m_titleButton;
	std::unique_ptr<Animator> m_titleAnimation;

	//	始めに表示するステージ
	int m_stageIndex;

	//	後処理エフェクト
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	タイマー
	float m_timer;

};