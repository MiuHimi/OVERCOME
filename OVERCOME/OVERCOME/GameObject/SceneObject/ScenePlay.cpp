//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "ScenePlay.h"

#include "../../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

std::unique_ptr<Player> ScenePlay::mp_player;
bool SceneManager::m_clearSceneState;

//DirectX::SimpleMath::Matrix MatrixManager::m_view;

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

	// カメラオブジェクトの作成
	mp_camera = std::make_unique<GameCamera>();

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

	// スコアの生成
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create();
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
void ScenePlay::Update(DX::StepTimer const& timer)
{
	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	// 床とプレイヤーの衝突判定
	//m_hitPlayerToFloorFlag = false;
	mp_player->SetFloorCollideState(false);
	if (Collision::HitCheck_Box2Box(mp_gameFloor->GetCollision(), mp_player->GetCollision()) == true)
	{
		// 当たったら床との衝突フラグを立てる
		//m_hitPlayerToFloorFlag = true;

		// ジャンプモーションを終了させる(ためのフラグを立てる)
		mp_player->SetJumpState(false);
		// 床との衝突フラグを立てる
		mp_player->SetFloorCollideState(true);
		// オブジェクトに接触したためフラグを伏せる
		mp_player->SetNotTouchState(false);

		// 地面にめり込まないようにする
		if (mp_player->GetPos().y < 0.0f)
		{
			mp_player->SetHeightPos(0.0f);
		}
	}

	// 道路とプレイヤーの衝突判定
	//m_hitPlayerToRoadFlag = false;
	mp_player->SetRoadCollideState(false);
	bool hitObject = false;
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			if (mp_gameRoad->GetRoadObject(j, i).roadType != 0)
			{
				if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
				{
					//m_hitPlayerToRoadFlag = true;

					// ジャンプモーションを終了させる(ためのフラグを立てる)
					mp_player->SetJumpState(false);
					// 道路との衝突フラグを立てる
					mp_player->SetRoadCollideState(true);
					// オブジェクトに接触したためフラグを伏せる
					mp_player->SetNotTouchState(false);

					// プレイヤーに加点のチャンス、飛ぶ時のために道路座標を取得
					if (mp_gameScore->GetPointChance() == false && mp_gameRoad->GetRoadObject(j, i).roadType == 2)
					{
						mp_gameScore->SetAddPointChance(j, i);
						mp_gameScore->SetPointChance(true);
					}
					if (mp_gameScore->GetPointChance() == true && mp_gameRoad->GetRoadObject(j, i).roadType == 2)
					{
						mp_gameScore->AddPointChance(j, i);
					}
					if (mp_gameScore->GetPointChance() == true && mp_gameRoad->GetRoadObject(j, i).roadType != 2)
					{
						mp_gameScore->SetAddPointChance(0, 0);
						mp_gameScore->SetPointChance(false);
					}

					// 道路にめり込まないようにする
					if (mp_player->GetPos().y > 0.5f && mp_player->GetPos().y <= 1.0f)
					{
						mp_player->SetHeightPos(1.0f);
					}

					hitObject = true;

					// ゴールに到達したら
					if (SceneManager::GetStageNum() == 1 && i == 5 && j == 7 ||       // ステージ１のゴール
						SceneManager::GetStageNum() == 2 && i == 11 && j == 13 ||     // ステージ２のゴール(分岐１)
						SceneManager::GetStageNum() == 2 && i == 13 && j == 15)       // ステージ２のゴール(分岐２)
					{
						m_toResultMoveOnChecker = true;
						SceneManager::SetResultSceneState(true);
					}
				}
			}
			if (hitObject == true) break;
		}
		if (hitObject == true) break;
	}

	// ゲーム床の更新
	mp_gameFloor->Update(timer);
	// ゲーム道路の更新
	mp_gameRoad->Update(timer);

	// プレイヤーの更新
	mp_player->Update(timer);

	// 制限時間の更新
	mp_gameTimer->Update(timer);
	if (mp_gameTimer->GetTimeUpFlag() == true)
	{
		// 制限時間がきたらゲームオーバー
		m_toResultMoveOnChecker = true;
		SceneManager::SetResultSceneState(false);
	}

	// スコアの更新
	if (hitObject == false && mp_player->GetJumpState() == false)
	{
		// 道路に乗ってないorジャンプしていなかったらフラグを立てる
		mp_gameScore->SetDeductFlag(true);
	}
	if (hitObject == true || mp_player->GetJumpState() == true)
	{
		// 道路に乗っているか、ジャンプしていたらフラグを伏せる
		mp_gameScore->SetDeductFlag(false);
	}
	mp_gameScore->Update(timer);
	if (mp_gameScore->GetScore() == 0)
	{
		// スコアが0だったらゲームオーバー
		SceneManager::SetResultSceneState(false);
	}

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
void ScenePlay::Render()
{
	// ビュー行列の作成
	DirectX::SimpleMath::Matrix m_view = DirectX::SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), DirectX::SimpleMath::Vector3::Up);

	// 射影行列を設定
	MatrixManager::SingletonGetInstance().SetView(m_view);

	// ゲーム床の描画
	mp_gameFloor->Render(m_view);
	// ゲーム道路の描画
	mp_gameRoad->Render(m_view);

	// スカイドームの描画
	mp_skydome->Render(m_view);

	// プレイヤーの描画
	//mp_player->Render(m_view);
	//mp_player->DrawDebugCollision(m_view);

	// 制限時間の描画
	mp_gameTimer->Render();

	// スコアの描画
	//mp_gameScore->Render();

	// デバッグ用
	//GameDebug::SingletonGetInstance().DebugRender("ScenePlay", DirectX::SimpleMath::Vector2(20.0f, 10.0f));
	//GameDebug::SingletonGetInstance().DebugIntRender("Test : ", mp_gameTimer->GetTime(), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	//GameDebug::SingletonGetInstance().DebugFloatRender("Test : ", mp_player->GetPos().x, DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	//GameDebug::SingletonGetInstance().DebugBoolRender("Test : ", mp_player->GetJumpState(), DirectX::SimpleMath::Vector2(20.0f, 70.0f));
}
