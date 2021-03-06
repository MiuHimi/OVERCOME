//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "ScenePlay.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

std::unique_ptr<Player> ScenePlay::mp_player;
bool SceneManager::m_clearSceneState;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
ScenePlay::ScenePlay(SceneManager* sceneManager)
	: SceneBase(sceneManager),
   	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
ScenePlay::~ScenePlay()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void ScenePlay::Initialize()
{
	m_toResultMoveOnChecker = false;

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// カメラオブジェクトの作成
	//mp_camera = std::make_unique<GameCamera>();
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// ゲーム床の生成
	mp_gameFloor = std::make_unique<GameFloor>();
	// ゲーム床のモデル読み込み
	mp_gameFloor->Create();

	// ゲーム道路の生成
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	// ゲーム道路のモデル読み込み
	mp_gameRoad->Create();

	// ゲーム道路の生成
	//mp_gameTarget = std::make_unique<GameTarget>();
	//mp_gameTarget->Initialize();
	// ゲーム道路のモデル読み込み
	//mp_gameTarget->Create();

	// ゲームマップの生成
	mp_gameMap = std::make_unique<GameMap>();
	mp_gameMap->Initialize();
	// ゲームマップのモデル読み込み
	mp_gameMap->Create();

	// ゲーム敵管理の生成
	mp_gameEnemyManager = std::make_unique<GameEnemyManager>();
	mp_gameEnemyManager->Initialize();
	// ゲーム敵管理のモデル読み込み
	mp_gameEnemyManager->Create();

	// プレイヤーの生成
	mp_player = std::make_unique<Player>();
	mp_player->Initialize();
	// プレイヤーのモデルの読み込み
	mp_player->Create();

	// スカイドームの生成
	//mp_skydome = std::make_unique<SkyDome>();
	//mp_skydome->Initialize();
	// スカイドームのモデルの読み込み
	//mp_skydome->Create();

	// スコアの生成
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create();

	m_fadeInCount = 1;
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureFadeIn.GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureBackground.GetAddressOf());

	//// メッシュ衝突判定
	//m_collisionStage = std::make_unique<CollisionMesh>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\StageMap\\stage01.obj");


	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();

	// ビュー行列の作成
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	float aspectRatio = float(size.right) / float(size.bottom);
	// 画角を設定
	float fovAngleY = XMConvertToRadians(45.0f);

	// 射影行列を作成
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// 行列を設定
	mp_matrixManager->SetViewProjection(view, projection);

	// エフェクトマネージャーの初期化
	//mp_effectManager = nullptr;
	//mp_effectManager = new EffectManager();
	//mp_effectManager->Create();
	//mp_effectManager->Initialize(5, SimpleMath::Vector3(0, 0, 0), SimpleMath::Vector3(0, 0, 0));
	//mp_effectManager->SetRenderState(view, projection);


	// サウンド再生
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"ScenePlay.acb", L"ScenePlay.awb");
	adx2le->Play(0);
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void ScenePlay::Finalize()
{
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}
	
	/*if (mp_effectManager != nullptr) {
		mp_effectManager->Lost();
		delete mp_effectManager;
		mp_effectManager = nullptr;
	}*/
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void ScenePlay::Update(DX::StepTimer const& timer)
{
	// フェードイン
	m_fadeInCount -= 0.01f;
	if (m_fadeInCount < 0.0f)m_fadeInCount = 0.0f;

	//mp_effectManager->Update(timer);

	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	// カメラの更新
	mp_camera->Update(timer, mp_player->GetPlayer());

	// 道路とプレイヤーの衝突判定
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			/*if (mp_gameRoad->GetRoadObject(j, i).roadType != 0)
			{
				if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
				{
					
				}
			}*/

			if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
			{
				if (i != mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x || j != mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y)break;

				// ゴールに到達したら
				if (i == mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x &&
					j == mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y)
				{
					m_toResultMoveOnChecker = true;
					SceneManager::SetResultSceneState(true);
					// マウスカーソルの表示
					ShowCursor(TRUE);
				}
			}
		}
	}

	// 的と弾の衝突判定
	/*for (int j = 0; j < mp_gameTarget->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameTarget->GetMaxFloorBlock(); i++)
		{
			// 的の状態取得
			if (mp_gameTarget->GetState(j, i))
			{
				for (int k = 0; k < mp_player->GetBulletManager()->GetMaxBulletNum(); k++)
				{
					// 弾の状態取得
					if (mp_player->GetBulletManager()->GetBulletState(k))
					{
						if (Collision::HitCheck_Sphere2Box(mp_player->GetBulletManager()->GetBulletCollide(k), mp_gameTarget->GetCollisionObject(j, i)->GetCollision()))
						{
							// 的と弾の状態
							mp_gameTarget->SetState(j, i, false);
							mp_player->GetBulletManager()->SetBulletState(k, false);

							// 加点
							mp_gameScore->FluctuationScore(100);
						}
					}
				}
			}
		}
	}*/

	// マップの更新
	mp_gameMap->Update(timer, mp_player->GetPlayer());

	// 敵とプレイヤーの衝突判定
	for (int i = 0; i < mp_gameEnemyManager->GetMaxEnemyNum(); i++)
	{
		if (!mp_gameEnemyManager->GetEnemyState(i))continue;
		if (mp_gameEnemyManager->GetEnemyState(i))
		{
			if (Collision::HitCheck_Sphere2Box(mp_gameEnemyManager->GetEnemyCollide(i), mp_player->GetCollision()))
			{
				mp_gameEnemyManager->SetEnemyState(i, false);
			}
		}
	}

	// 敵と弾の衝突判定
	for (int i = 0; i < mp_gameEnemyManager->GetMaxEnemyNum(); i++)
	{
		if (!mp_gameEnemyManager->GetEnemyState(i))continue;
		if (mp_gameEnemyManager->GetEnemyState(i))
		{
			for (int j = 0; j < mp_player->GetBulletManager()->GetMaxBulletNum(); j++)
			{
				if (!mp_player->GetBulletManager()->GetBulletState(j))continue;
				if (mp_player->GetBulletManager()->GetBulletState(j))
				{
					if (Collision::HitCheck_Sphere2Sphere(mp_gameEnemyManager->GetEnemyCollide(i),
						mp_player->GetBulletManager()->GetBulletCollide(j)))
					{
						// 20点獲得
						mp_gameScore->FluctuationScore(20);
						// 互いのstateをfalseに
						mp_gameEnemyManager->SetEnemyState(i, false);
						mp_player->GetBulletManager()->SetBulletState(j, false);
					}
				}
			}
		}
	}

	// 弾の表示限界の設定
	SimpleMath::Vector3 pPos = mp_player->GetPos();
	SimpleMath::Vector3 bPos[5];
	for (int i = 0; i < mp_player->GetBulletManager()->GetMaxBulletNum(); i++)
	{
		// 発射されている弾のみ計測
		if (!mp_player->GetBulletManager()->GetBulletState(i))
		{
			break;
		}

		// しきい値(100)
		float length = 100.0f;
		float len = 0.0f;
		bPos[i] = mp_player->GetBulletManager()->GetPos(i);

		// 弾とプレイヤーの距離を計測
		len = ((pPos.x - bPos[i].x)*(pPos.x - bPos[i].x)) +
			((pPos.y - bPos[i].y)*(pPos.y - bPos[i].y)) +
			((pPos.z - bPos[i].z)*(pPos.z - bPos[i].z));

		// 距離が100を超えたら弾を消す
		if (length*length < len)
		{
			mp_player->GetBulletManager()->SetBulletState(i, false);
		}
	}

	// ゲーム床の更新
	mp_gameFloor->Update(timer);
	// ゲーム道路の更新
	mp_gameRoad->Update(timer);
	// ゲーム的の更新
	//mp_gameTarget->Update(timer);

	// プレイヤーの更新
	mp_player->Update(timer);

	// 敵の更新
	mp_gameEnemyManager->Update(timer, mp_player->GetPlayer());

	// スコアの更新
	mp_gameScore->Update(timer);
	if (mp_gameScore->GetScore() == 0)
	{
		// スコアが0だったらゲームオーバー
		SceneManager::SetResultSceneState(false);
	}

	// シーン操作
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
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), DirectX::SimpleMath::Vector3::Up);
	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	float aspectRatio = float(size.right) / float(size.bottom);
	// 画角を設定
	float fovAngleY = XMConvertToRadians(45.0f);

	// 射影行列を作成
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// 行列を設定
	mp_matrixManager->SetViewProjection(view, projection);

	// ゲーム床の描画
	mp_gameFloor->Render(mp_matrixManager);
	// ゲーム道路の描画
	//mp_gameRoad->Render(mp_matrixManager);
	// ゲーム的の描画
	//mp_gameTarget->Render(mp_matrixManager);

	// マップの描画
	mp_gameMap->Render(mp_matrixManager);

	// プレイヤーの描画
	mp_player->Render(mp_matrixManager);

	// 敵の描画
	mp_gameEnemyManager->Render(mp_matrixManager);

	// スカイドームの描画
	//mp_skydome->Render(mp_matrixManager);
	
	//mp_effectManager->Render();

	// スコアの描画
	mp_gameScore->Render();

	DrawManager::SingletonGetInstance().DrawAlpha(m_textureBackground.Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0, 1.0f, 1.0f, m_fadeInCount));
}
