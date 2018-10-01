//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "ScenePlay.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

std::unique_ptr<Player> ScenePlay::mp_player;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
ScenePlay::ScenePlay(SceneManager* sceneManager)
	: SceneBase(sceneManager)
{
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
ScenePlay::ScenePlay(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
ScenePlay::~ScenePlay()
{
	/*delete mp_game;
	mp_game = nullptr;*/
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void ScenePlay::Initialize()
{
	m_toResultMoveOnChecker = false;

	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"SegoeUI_18.spritefont");

	// カメラオブジェクトの作成
	mp_camera = std::make_unique<MyCamera>();

	// ゲーム床の生成
	mp_gameFloor = std::make_unique<GameFloor>(mp_game);
	// ゲーム床のモデル読み込み
	mp_gameFloor->Create();

	// ゲーム道路の生成
	mp_gameRoad = std::make_unique<GameRoad>(mp_game);
	mp_gameRoad->Initialize();
	// ゲーム道路のモデル読み込み
	mp_gameRoad->Create(mp_game);

	// プレイヤーの生成
	mp_player = std::make_unique<Player>(mp_game);
	mp_player->Initialize();
	// プレイヤーのモデルの読み込み
	mp_player->Create();

	// スカイドームの生成
	mp_skydome = std::make_unique<SkyDome>(mp_game);
	mp_skydome->Initialize();
	// スカイドームのモデルの読み込み
	mp_skydome->Create();

	// 制限時間の生成
	mp_gameTimer = std::make_unique<GameTimer>();
	mp_gameTimer->Create();
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void ScenePlay::Finalize()
{
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void ScenePlay::Update(DX::StepTimer const& timer, Game* game)
{
	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	// 床とプレイヤーの衝突判定
	m_hitPlayerToFloorFlag = false;
	if (Collision::HitCheck_Box2Box(mp_gameFloor->GetCollision(), mp_player->GetCollision()) == true)
	{
		m_hitPlayerToFloorFlag = true;
		mp_player->SetJumpState(false);
		if (mp_player->GetPos().y < 0.0f)
		{
			mp_player->SetHeightPos(0.0f);
		}
	}

	// 道路とプレイヤーの衝突判定
	m_hitPlayerToRoadFlag = false;
	mp_player->SetCollideState(false);
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			if (mp_gameRoad->GetRoadObject(j, i).roadType == 1 || mp_gameRoad->GetRoadObject(j, i).roadType == 2 || mp_gameRoad->GetRoadObject(j, i).roadType == 3)
			{
				if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
				{
					m_hitPlayerToRoadFlag = true;

					if (mp_player->GetPos().y <= 0.0f)
					{
						mp_player->SetHeightVel(0.0f);
					}
					if (mp_player->GetPos().y >= 0.5f && mp_player->GetPos().y <= 1.0f)
					{
						mp_player->SetJumpState(false);
						mp_player->SetHeightVel(1.0f);
					}
					mp_player->SetCollideState(true);

					if (i == 5 && j == 7)
					{
						m_toResultMoveOnChecker = true;
					}
				}
			}
		}
	}

	// ゲーム床の更新
	mp_gameFloor->Update(timer);
	// ゲーム道路の更新
	mp_gameRoad->Update(timer);

	// プレイヤーの更新
	mp_player->Update(timer);

	// 制限時間の更新
	mp_gameTimer->Update(timer);

	// カメラの更新
	mp_camera->Update(timer, mp_player->GetPlayer());

	

	// シーン操作
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Z))
	{
		m_toResultMoveOnChecker = true;
	}
	if (m_toResultMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_RESULT);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
//void SceneLogo::Render()
//{
//}
void ScenePlay::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// ビュー行列の作成
	DirectX::SimpleMath::Matrix m_view = DirectX::SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), DirectX::SimpleMath::Vector3::Up);

	// ゲーム床の描画
	mp_gameFloor->Render(game, m_view);
	// ゲーム道路の描画
	mp_gameRoad->Render(m_view);

	// スカイドームの描画
	mp_skydome->Render(m_view);

	// プレイヤーの描画
	mp_player->Render(m_view);
	//mp_player->DrawDebugCollision(m_view);

	// 制限時間の描画
	mp_gameTimer->Render();

	// デバッグ用
	/*sprites->Begin();
	m_font->DrawString(sprites, L"ScenePlay", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"Zkey to SceneResult", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);

	if (m_hitPlayerToFloorFlag == true)
	{
		m_font->DrawString(sprites, L"FloorHIT", DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	}
	if (m_hitPlayerToRoadFlag == true)
	{
		m_font->DrawString(sprites, L"RoadHIT", DirectX::SimpleMath::Vector2(20.0f, 70.0f));
	}
	sprites->End();*/
}
