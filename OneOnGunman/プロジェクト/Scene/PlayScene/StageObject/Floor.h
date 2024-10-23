/*/
 *  ファイル名		：	Floor.h
 *  概要			：	床を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/26
/*/
#pragma once

class Floor
{
public:
	Floor();
	~Floor() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	//	モデル
	std::unique_ptr<DirectX::Model> m_model;

};