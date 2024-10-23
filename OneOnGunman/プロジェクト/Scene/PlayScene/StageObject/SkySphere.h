/*/
 *  ファイル名		：	SkySphere.h
 *  概要			：	天球を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/26
/*/
#pragma once

class SkySphere
{
public:
	SkySphere();
	~SkySphere() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	//	モデル
	std::unique_ptr<DirectX::Model> m_model;

};
