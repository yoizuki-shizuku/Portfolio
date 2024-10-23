/*/
 *  ファイル名		：	Transition.h
 *  概要			：	画面遷移を行う
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/16
/*/
#pragma once

class Transition
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector2 windowSize;
		DirectX::SimpleMath::Vector2 ratio;
	};

public:
	~Transition() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	/// <summary>
	/// フェードにかかる時間を設定
	/// </summary>
	/// <param name="seconds">フェードにかかる時間</param>
	void SetFadeSpeed(float seconds) { m_fadeSeconds = seconds; }

	/// <summary>
	/// フェードにかかる時間を取得
	/// </summary>
	/// <returns>単位 : 秒(s)</returns>
	const float GetFadeSecond() { return m_fadeSeconds; }

	//	フェードインを開始する
	void StartFadeIn();
	//	フェードアウトを開始する
	void StartFadeOut();

	/// <summary>
	/// フェードインの処理中かチェックする
	/// </summary>
	/// <returns>true : 処理中</returns>
	bool ProcessCheckFadeIn() { return m_fadeIn; }
	/// <summary>
	/// フェードアウトの処理中かチェックする
	/// </summary>
	/// <returns>true : 処理中</returns>
	bool ProcessCheckFadeOut() { return m_fadeOut; }
	/// <summary>
	/// フェードインの終了時を検知
	/// </summary>
	/// <returns>true : 終了時の1f</returns>
	bool FinishFadeIn() { return !m_fadeIn && m_beforeFadeIn; }
	/// <summary>
	/// フェードアウトの終了時を検知
	/// </summary>
	/// <returns>true : 終了時の1f</returns>
	bool FinishFadeOut() { return !m_fadeOut && m_beforeFadeOut; }

public:
	//	インスタンス生成
	static void CreateInstance();
	//	インスタンス取得
	static Transition* const GetInstance();

	//	代入、コピーのコンストラクタを削除
	void operator=(const Transition& object) = delete;
	Transition(const Transition& object) = delete;

private:
	Transition();

	//	フェードインの処理
	void FadeInUpdate(float elapsedTime);
	//	フェードアウトの処理
	void FadeOutUpdate(float elapsedTime);
	//	シェーダーの作成
	void CreateShader();

private:
	//	インスタンスへのポインタ
	static std::unique_ptr<Transition> s_instance;

	//	画面のサイズ
	int m_screenW, m_screenH;

	//	実行中のフェード処理
	std::atomic<bool> m_fadeIn, m_fadeOut;
	//	終了時検知用
	bool m_beforeFadeIn, m_beforeFadeOut;

	//	透明度
	float m_alpha;
	//	フェードにかかる秒数
	float m_fadeSeconds;
	//	処理の時間を計測
	float m_processTimer;

	//	シェーダーに情報を渡すためのバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

};