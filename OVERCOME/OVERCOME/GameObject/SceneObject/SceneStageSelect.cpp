//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneStageSelect.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneStageSelect::SceneStageSelect(SceneManager* sceneManager)
	: SceneBase(sceneManager)
{
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneStageSelect::SceneStageSelect(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
	/*delete mp_game;
	mp_game = nullptr;*/
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneStageSelect::Initialize()
{
	m_toPlayMoveOnChecker = false;

	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(/*device*/mp_game->GetDevice(), L"SegoeUI_18.spritefont");
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneStageSelect::Finalize()
{
}

/// <summary>
/// ステージ選択シーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneStageSelect::Update(DX::StepTimer const& timer, Game* game)
{
	// 入力情報を更新
	InputManager::GetInstance().Update();

	static int count = 0;
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	{
		count++;
		if (count > 3)count = 0;
		SceneManager::SetStageNum(count);
	}

	// キー入力
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Z))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
//void SceneStageSelect::Render()
//{
//}
void SceneStageSelect::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// デバッグ用
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneStageSelect", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"Zkey to ScenePlay", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);

	if (SceneManager::GetStageNum() != 1 && SceneManager::GetStageNum() != 2)
	{
		m_font->DrawString(sprites, L"StageNone", DirectX::SimpleMath::Vector2(20.0f, 50.0f), Colors::Yellow);
	}
	else if (SceneManager::GetStageNum() == 1)
	{
		m_font->DrawString(sprites, L"Stage1", DirectX::SimpleMath::Vector2(20.0f, 50.0f), Colors::Yellow);
	}
	else if (SceneManager::GetStageNum() == 2)
	{
		m_font->DrawString(sprites, L"Stage2", DirectX::SimpleMath::Vector2(20.0f, 50.0f), Colors::Yellow);
	}
	sprites->End();
}
