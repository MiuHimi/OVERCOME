//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneResult.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneResult::SceneResult(SceneManager* sceneManager)
	: SceneBase(sceneManager)
{
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneResult::SceneResult(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneResult::~SceneResult()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneResult::Initialize()
{
	m_toTitleMoveOnChecker = false;

	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(/*device*/mp_game->GetDevice(), L"SegoeUI_18.spritefont");
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneResult::Finalize()
{
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneResult::Update(DX::StepTimer const& timer, Game* game)
{
	// 入力情報を更新
	InputManager::GetInstance().Update();
	// キー入力
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
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
//}
void SceneResult::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// デバッグ用
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneResult", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"SPACEkey to SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);
	sprites->End();
}
