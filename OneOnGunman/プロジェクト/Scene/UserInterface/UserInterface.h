/*/
 *  ファイル名		：	UserInterface.h
 *  概要			：	UIを表示するためのクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/02
/*/
#pragma once

//	アンカーポイントの列挙体
enum class Anchor : int
{
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,

	MIDDLE_LEFT,
	MIDDLE_CENTER,
	MIDDLE_RIGHT,

	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT
};

//	ゲージの方向
enum class Direction
{
	Down,
	Up,
	Right,
	Left,
};

class UserInterface
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		float windowWidth;						//	画面の横幅
		float windowHeight;						//	画面の縦幅
		float ratio;							//	ゲージとして使う際の表示する割合
		int direction;							//	ゲージとして使う際の方向
		DirectX::SimpleMath::Vector4 color;		//	色
	};

public:
	UserInterface();
	~UserInterface() = default;

	//	画像データを読み込む
	void LoadTexture(const wchar_t* path);

	void Render();

public:
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; }
	void SetRadian(const float& radian)                            { m_radian = radian; }
	void SetScale(const DirectX::SimpleMath::Vector2& scale)       { m_scale = scale; }
	void SetScale(const float& scale)                              { m_scale = DirectX::SimpleMath::Vector2(scale); }
	void SetAnchor(const Anchor anchor)                            { m_anchor = anchor; }
	void SetAlpha(const float& alpha)                              { m_alpha = alpha; }
	void SetRatio(const float& ratio)                              { m_ratio = ratio; }
	void SetDirection(const Direction dir)                         { m_direction = dir; }
	void SetColor(const DirectX::SimpleMath::Vector3& color)	   { m_color = color; }

	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	float GetDegree()                          const { return DirectX::XMConvertToDegrees(m_radian); }
	DirectX::SimpleMath::Vector2 GetScale()    const { return m_scale; }
	Anchor GetAnchor()                         const { return m_anchor; }
	float GetAlpha()                           const { return m_alpha; }
	DirectX::SimpleMath::Vector2 GetSize() const 
	{ return DirectX::SimpleMath::Vector2(static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight)); }

private:
	//	シェーダーの作成
	void CreateShader();

private:
	//	シェーダーに情報を渡すためのバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//	頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	//	ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	位置
	DirectX::SimpleMath::Vector2 m_position;
	//	角度
	float m_radian;
	//	大きさ
	DirectX::SimpleMath::Vector2 m_scale;
	//	アンカーポイント
	Anchor m_anchor;
	//	透明度
	float m_alpha;
	//	画面の大きさ
	int m_windowWidth, m_windowHeight;
	//	テクスチャの元の大きさ
	int m_textureWidth, m_textureHeight;
	//	ゲージとしての割合
	float m_ratio;
	//	ゲージとしての方向
	Direction m_direction;
	//	色
	DirectX::SimpleMath::Vector3 m_color;

};