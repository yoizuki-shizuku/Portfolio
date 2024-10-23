/*/
 *  ファイル名		：	TorpedoBullet.h
 *  概要			：	魚雷のように徐々に加速していく弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/20
/*/
#pragma once

#include "../EnemyBullet.h"
#include "GeometricPrimitive.h"

class TorpedoBullet final : public EnemyBullet
{
public:
	TorpedoBullet(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 maxVelocity);
	~TorpedoBullet() = default;

	void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) override;
	bool Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	DirectX::SimpleMath::Vector3 GetColor() const override { return DirectX::SimpleMath::Vector3(1.f, 0.647f, 0.f); }

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 color;
	};

private:
	//	最大速度
	DirectX::SimpleMath::Vector3 m_maxVelocity;
	//	現在速度
	DirectX::SimpleMath::Vector3 m_nowVelocity;
	//	タイマー
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