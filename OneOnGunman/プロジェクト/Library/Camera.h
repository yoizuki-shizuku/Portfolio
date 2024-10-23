/*/
 *  ファイル名		：	Camera.h
 *  概要			：	カメラの機能を持ったクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/12
/*/
#pragma once

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Initialize();
	void Update();
	
	//	ビュー行列の作成
	void CreateView();


public:	//	ゲッターとセッター
	void SetEye(const DirectX::SimpleMath::Vector3& eye) { m_eye = eye; }
	void SetTarget(const DirectX::SimpleMath::Vector3& target) { m_target = target; }

	DirectX::SimpleMath::Matrix GetView() const { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() const { return m_proj; }

private:
	//	射影行列の作成
	void CreateProjection();

private:
	//	カメラの位置
	DirectX::SimpleMath::Vector3 m_eye;
	//	注視の位置
	DirectX::SimpleMath::Vector3 m_target;
	//	カメラの上方向ベクトル
	DirectX::SimpleMath::Vector3 m_up;

	//	ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	//	射影行列
	DirectX::SimpleMath::Matrix m_proj;

};