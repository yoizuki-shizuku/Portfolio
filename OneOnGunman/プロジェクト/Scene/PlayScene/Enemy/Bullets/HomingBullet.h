/*/
 *  ファイル名		：	HomingBullet.h
 *  概要			：	プレイヤーを追尾する弾
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/21
/*/
#pragma once

#include "../EnemyBullet.h"
#include "GeometricPrimitive.h"
#include "../../Player/Player.h"

class HomingBullet final : public EnemyBullet
{
public:
	HomingBullet(DirectX::SimpleMath::Vector3 position, Player* target, float speed);
	~HomingBullet() = default;

	void Initialize(ID3D11InputLayout* input, ID3D11VertexShader* vertex, ID3D11PixelShader* pixel) override;
	bool Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void Finalize() override;

	DirectX::SimpleMath::Vector3 GetColor() const override { return DirectX::SimpleMath::Vector3(1.f, 1.f, 0.f); }

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 position;
		DirectX::SimpleMath::Vector4 color;
	};

private:
	//	プレイヤーへの方向ベクトル
	DirectX::SimpleMath::Vector3 ToPlayerVector();

private:
	//	プレイヤーのポインタ
	Player* m_target;
	//	進行方向ベクトル
	DirectX::SimpleMath::Vector3 m_direction;
	//	弾の速度
	float m_speed;
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