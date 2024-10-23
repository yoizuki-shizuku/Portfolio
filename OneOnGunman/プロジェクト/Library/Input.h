/*/
 *  ファイル名		：	Input.h
 *  概要			：	ステートトラッカーをシングルトン化
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/05
/*/
#pragma once

class Input
{
public:
	//	デストラクタ
	~Input() = default;

	void Initialize();
	void Update();

	//	キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardTracker() const { return m_kbTracker.get(); }
	//	マウストラッカー
	DirectX::Mouse::ButtonStateTracker* GetMouseTracker() const { return m_msTracker.get(); }

public:
	//	インスタンス生成
	static void CreateInstance();

	//	インスタンス取得
	static Input* const GetInstace();

private:
	//	コンストラクタ
	Input();

	//	代入、コピーのコンストラクタを削除
	void operator=(const Input& object) = delete;
	Input(const Input& object) = delete;

private:
	//	インスタンスへのポインタ
	static std::unique_ptr<Input> s_instance;

	//	キーボードトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_kbTracker;

	//	マウストラッカー
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_msTracker;

};