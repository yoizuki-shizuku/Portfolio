/*/
 *  ファイル名		：	ResultButtonManager.cpp
 *  概要			：	リザルト画面でのボタンを管理する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/18
/*/
#include "pch.h"
#include "ResultButtonManager.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/Audio/AudioManager.h"

//	表示タイミング
#define DISPLAY_OFFSET		(0.2f)
// TODO:最終ステージ番号
#define LAST_STAGE_INDEX	(2)

ResultButtonManager::ResultButtonManager() :
	m_timer(0.f),
	m_isLastStage(false)
{
}

void ResultButtonManager::Initialize(int stageIndex)
{
	//	画面の大きさ取得
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	ボタンの画像の生成と初期化、ファイル読み込み
	for (int i = 0; i < SIZE; i++)
	{
		m_texture[i] = std::make_unique<UserInterface>();
		m_texture[i]->SetAnchor(Anchor::MIDDLE_CENTER);
		m_texture[i]->SetScale(SimpleMath::Vector2::Zero);
	}
	m_texture[Process::SELECT]->LoadTexture(L"Resources/Texture/Result/Select.png");
	m_texture[Process::RETRY]->LoadTexture(L"Resources/Texture/Result/Retry.png");
	m_texture[Process::NEXT]->LoadTexture(L"Resources/Texture/Result/Next.png");

	m_texture[Process::SELECT]->SetPosition(SimpleMath::Vector2(width * 0.9f, height - 100.f));
	m_texture[Process::RETRY]->SetPosition(SimpleMath::Vector2(width * 0.73f, height - 100.f));
	m_texture[Process::NEXT]->SetPosition(SimpleMath::Vector2(width * 0.6f, height - 100.f));

	//	個別で位置調整
	m_texture[Process::RETRY]->SetPosition(m_texture[Process::RETRY]->GetPosition() + SimpleMath::Vector2(15.f, 0.f));

	//	インフォメーション画像の生成と初期化、読み込み
	for (int i = 0; i < SIZE; i++)
	{
		m_info[i] = std::make_unique<UserInterface>();
		m_info[i]->SetAnchor(Anchor::MIDDLE_LEFT);
		m_info[i]->SetPosition(SimpleMath::Vector2(20.f, height - 90.f));
		m_info[i]->SetAlpha(0.f);
	}
	m_info[Process::SELECT]->LoadTexture(L"Resources/Texture/Result/SelectTelop.png");
	m_info[Process::RETRY]->LoadTexture(L"Resources/Texture/Result/RetryTelop.png");
	m_info[Process::NEXT]->LoadTexture(L"Resources/Texture/Result/NextTelop.png");

	//	アニメーションとボタンの生成
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_fadeinAnimator[i]->LoadFile("Resources/UIAnimation/Result/ResultButtonIn.json");

		m_choiceAnimator[i] = std::make_unique<Animator>(m_texture[i].get());
		m_choiceAnimator[i]->LoadFile("Resources/UIAnimation/Result/ChoiceButton.json");
	
		m_buttons[i] = std::make_unique<Button>(m_texture[i].get());
	}

	m_timer = 0.f;
	m_isLastStage = stageIndex == LAST_STAGE_INDEX;
}

ResultButtonManager::Process ResultButtonManager::Update()
{
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	//	アニメーションとボタンの更新
	for (int i = 0; i < SIZE; i++)
	{
		m_fadeinAnimator[i]->Update(elapsedTime);
		m_choiceAnimator[i]->Update(elapsedTime);
		m_buttons[i]->Update();
	}

	//	タイミングをずらして画面に表示する
	m_fadeinAnimator[Process::NEXT]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET) m_fadeinAnimator[Process::RETRY]->PlayAnimation();
	if (m_timer > DISPLAY_OFFSET * 2) m_fadeinAnimator[Process::SELECT]->PlayAnimation();

	//	全てのアニメーションが終了するまで処理しない
	if (!m_fadeinAnimator[Process::SELECT]->GetFinishAnimation()) return Process::MAINTAIN;

	//	マウスが重なっていたら説明を表示し、アニメーションさせる
	for (int i = 0; i < SIZE; i++)
	{
		//	最終ステージだったならばNEXTを表示しない
		if (m_isLastStage && i == Process::NEXT) continue;

		if (m_buttons[i]->OnMouse()) AudioManager::GetInstance()->PlaySE(CRI_SE_MOVECURSOR);

		//	重なっているかどうか
		if (m_buttons[i]->StayMouse())
		{
			m_info[i]->SetAlpha(1.f);
			m_choiceAnimator[i]->PlayAnimation();
		}
		else
		{
			m_info[i]->SetAlpha(0.f);
			m_choiceAnimator[i]->PlayReverseAnimation();
			continue;
		}

		//	クリックされたかどうか
		if (m_buttons[i]->IsPresed())
		{
			AudioManager::GetInstance()->PlaySE(CRI_SE_DECIDE);
			//	ボタンに対応したキーを送る
			return (Process)i;
		}
	}

	return Process::MAINTAIN;
}

void ResultButtonManager::Render()
{
	for (int i = 0; i < SIZE; i++)
	{
		//	最終ステージだったならばNEXTを表示しない
		if (m_isLastStage && i == (int)Process::NEXT) continue;

		m_texture[i]->Render();
		m_info[i]->Render();
	}
}

void ResultButtonManager::Finalize()
{
}
