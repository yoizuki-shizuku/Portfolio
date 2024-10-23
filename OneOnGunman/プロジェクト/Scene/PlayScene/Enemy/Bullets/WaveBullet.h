/*/
 *  ファイル名		：	WaveBullet.h
 *  概要			：	波のように動く弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/01/06
/*/
#pragma once

#include "../EnemyBullet.h"
#include "GeometricPrimitive.h"

class WaveBullet : public EnemyBullet
{
public:
	WaveBullet(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 velocity);
	~WaveBullet() = default;

	void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) override;
	bool Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	DirectX::SimpleMath::Vector3 GetColor() const override { return DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f); }

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 color;
	};

private:
	//	移動方向ベクトル
	DirectX::SimpleMath::Vector3 m_velocity;
	//	ウェーブをする前の位置
	DirectX::SimpleMath::Vector3 m_beforePos;
	//	velocityに対して垂直なベクトル
	DirectX::SimpleMath::Vector3 m_vertical;
	//	消去用タイマー
	float m_timer;
	//	仮モデル
	std::unique_ptr<DirectX::GeometricPrimitive> m_model;
	//	ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	定数バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

};
