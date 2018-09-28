//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneTitle.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneTitle::SceneTitle(SceneManager* sceneManager) 
	                 : SceneBase(sceneManager)
{
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneTitle::SceneTitle(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneTitle::~SceneTitle()
{
	/*delete mp_game;
	mp_game = nullptr;*/
}

/// <summary>
/// タイトルシーンの初期化処理
/// </summary>
void SceneTitle::Initialize()
{
	m_toPlayMoveOnChecker = false;

	/*std::unique_ptr<DX::DeviceResources> dr;
	dr = std::make_unique<DX::DeviceResources>();
	dr->GetD3DDevice();*/
	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(/*mp_game->GetDevice()*/DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"SegoeUI_18.spritefont");
}
/// <summary>
/// タイトルシーンの終了処理
/// </summary>
void SceneTitle::Finalize()
{
}

/// <summary>
/// タイトルシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneTitle::Update(DX::StepTimer const& timer, Game* game)
{
	InputManager::SingletonGetInstance().Update();
	// 入力情報を更新
	//InputManager::GetInstance().Update();
	// キー入力
	//if (InputManager::GetInstance().GetTracker().leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
	if (/*InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space)*/InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
	}

}

/// <summary>
/// タイトルシーンの描画処理
/// </summary>
//void SceneTitle::Render()
//{
//	/*DebugText* debugText = DebugText::GetInstance();
//	debugText->AddText(Vector2(10, 10), L"SceneTitle");
//	debugText->AddText(Vector2(10, 30), L"SPACEkey to SceneLogo");*/
//}
void SceneTitle::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// デバッグ用
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"SPACEkey to SceneSelectStage", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);
	sprites->End();
}
