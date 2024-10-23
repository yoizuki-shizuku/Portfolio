/*/
 *  ファイル名		：	ResultScene.h
 *  概要			：	ステージをクリアした後にリザルトを表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/16
/*/
#pragma once

#include "../Scene.h"
#include "Scene/UserInterface/Numeral.h"
#include "Scene/UserInterface/UserInterface.h"
#include "UIs/ResultButtonManager.h"
#include "UIs/ClearTime.h"
#include "UIs/ShotCount.h"
#include "Library/Camera.h"
#include "../StageSelectScene/Model/InfinitePlane.h"

class ResultScene final : public Scene
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	xのみ
	};

public:
	ResultScene(float time, int shotCount, int stageIndex);
	~ResultScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	//	カメラ
	std::unique_ptr<Camera> m_camera;
	//	無限平面
	std::unique_ptr<InfinitePlane> m_plane;
	//	「リザルト」の表示
	std::unique_ptr<UserInterface> m_result;
	//	クリアタイムの表示
	std::unique_ptr<ClearTime> m_clearTimeDisplay;
	//	射撃回数の表示
	std::unique_ptr<ShotCount> m_shotCountDisplay;
	//	ボタン
	std::unique_ptr<ResultButtonManager> m_buttonManager;

	//	ステージのクリア時間
	float m_clearTime;
	//	射撃回数
	int m_shotCount;
	//	ステージ番号
	int m_stageIndex;

	//	後処理エフェクト
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	タイマー
	float m_timer;

};