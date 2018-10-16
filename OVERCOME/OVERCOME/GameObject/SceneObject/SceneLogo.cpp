//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneLogo.h"

#include "../../Utility/GameDebug.h"

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
	/*delete mp_game;
	mp_game = nullptr;*/
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneLogo::Initialize()
{
	m_toTitleMoveOnChecker = false;

	m_count = 0;
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
//}
void SceneLogo::Render(Game* game)
{
	// デバッグ用
	GameDebug::SingletonGetInstance().DebugRender("SceneLogo", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
}
