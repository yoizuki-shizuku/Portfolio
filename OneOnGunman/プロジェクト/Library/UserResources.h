/*/
 *	ファイル名		：	UserResources.h
 *	概要			：	各シーンで共通のものにアクセスできるようにしたシングルトン
 *
 *	作成者			：	山本玄臣
 *	作成日			：	2023/06/05
/*/
#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"

class UserResources
{
public:
	//	デストラクタ
	~UserResources() = default;

	//	デバイスリソース
	void SetDeviceResources(DX::DeviceResources* device) { m_deviceResources = device; }
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }

	//	タイマー
	void SetStepTimer(DX::StepTimer* timer) { m_stepTimer = timer; }
	DX::StepTimer* GetStepTimer() const { return m_stepTimer; }

	//	共通ステート
	void SetCommonStates(DirectX::CommonStates* states) { m_commonStates = states; }
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }

public:
	//	インスタンス生成
	static void CreateInstance();

	//	インスタンス取得
	static UserResources* const GetInstance();

private:
	//	コンストラクタ
	UserResources();

	//	代入、コピーのコンストラクタを削除
	void operator=(const UserResources& object) = delete;
	UserResources(const UserResources& object) = delete;

private:
	//	インスタンスへのポインタ
	static std::unique_ptr<UserResources> s_instance;

	//	デバイスリソース
	DX::DeviceResources*   m_deviceResources;

	//	ステップタイマー
	DX::StepTimer*         m_stepTimer;

	//	共通ステート
	DirectX::CommonStates* m_commonStates;
};