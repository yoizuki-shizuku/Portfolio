/*/
 *  ファイル名		：	OptionScene.h
 *  概要			：	各種オプションを行うシーン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/02/05
/*/
#pragma once

#include "Scene/Scene.h"
#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include "Library/Camera.h"
#include "../StageSelectScene/Model/InfinitePlane.h"
#include "UI/Volume.h"

class OptionScene final : public Scene
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	xのみ
	};

public:
	OptionScene();
	~OptionScene() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;
	void Finalize() override;

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	//	カメラ
	std::unique_ptr<Camera> m_camera;
	//	無限平面
	std::unique_ptr<InfinitePlane> m_plane;
	//	後処理エフェクト
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	「Option」の表示
	std::unique_ptr<UserInterface> m_option;
	//	タイトルへ戻るボタン
	std::unique_ptr<UserInterface> m_title;
	std::unique_ptr<Animator> m_titleAnimation;
	std::unique_ptr<Button> m_titleButton;
	
	//	BGMの音量調整
	std::unique_ptr<UserInterface> m_bgm;
	std::unique_ptr<Volume> m_bgmVolume;

	//	SEの音量調整
	std::unique_ptr<UserInterface> m_se;
	std::unique_ptr<Volume> m_seVolume;

	//	(デバッグ)セーブデータのリセット
	std::unique_ptr<UserInterface> m_resetData;
	std::unique_ptr<Button> m_resetDataButton;

	//	タイマー
	float m_timer;

};