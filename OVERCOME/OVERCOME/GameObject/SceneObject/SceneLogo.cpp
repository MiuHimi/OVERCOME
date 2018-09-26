//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneLogo.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneLogo::SceneLogo(SceneManager* sceneManager) 
	               : SceneBase(sceneManager)
{
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneLogo::SceneLogo(Game * game, SceneManager * sceneManager)
	               : mp_game(game)
	               , SceneBase(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneLogo::~SceneLogo()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneLogo::Initialize()
{
	m_toTitleMoveOnChecker = false;

	m_count = 0;

	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(/*device*/mp_game->GetDevice(), L"SegoeUI_18.spritefont");
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneLogo::Finalize()
{
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneLogo::Update(DX::StepTimer const& timer, Game* game)
{
	m_count++;

	if (m_count == 120)
	{
		m_toTitleMoveOnChecker = true;
	}

	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
//void SceneLogo::Render()
//{
//	// デバッグ用
//	/*DebugText* debugText = DebugText::GetInstance();
//	debugText->AddText(Vector2(10, 10), L"SceneLogo");
//	debugText->AddText(Vector2(10, 30), L"Count = %3d", m_count);*/
//}
void SceneLogo::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// デバッグ用
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneLogo", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);
	sprites->End();
}
