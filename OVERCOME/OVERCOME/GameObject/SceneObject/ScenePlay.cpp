//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2019/06/24
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "ScenePlay.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/MathManager.h"
#include "../../Utility/MatrixManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

#include "../../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const float ScenePlay::CLEAR_FADE = 0.02f;
const float ScenePlay::FAILD_FADE = 0.01f;

bool SceneManager::m_clearSceneState;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
ScenePlay::ScenePlay(SceneManager* sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toResultMoveOnChecker(false),
	  m_returnToTitleChecker(false),
	  isStartPlay(false),
	  mp_camera(nullptr),
	  mp_player(nullptr),
	  mp_gameEnemy(nullptr), mp_gameEnemyManager(nullptr),
	  mp_gameRoad(nullptr), mp_gameMap(nullptr),
	  mp_gameScore(nullptr), mp_gameDecorateObject(nullptr),
	  m_startGudeWave(0.0f), mp_startGuide(nullptr),
	  mp_outline(nullptr),
	  mp_fade(nullptr),
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

	// アクティブなウィンドウのサイズ
	RECT activeWndRect;
	// アクティブなウィンドウのハンドルを取得
	HWND activeWnd = GetActiveWindow();
	// アクティブなウィンドウのハンドルからその画面の大きさを取得
	GetWindowRect(activeWnd, &activeWndRect);

	// ウィンドウのサイズを取得
	float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
	float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);

	// タイトルバーの高さを取得
	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// カメラオブジェクトの作成
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom, m_isFullScreen);

	// プレイヤーの生成
	mp_player = std::make_unique<Player>();
	mp_player->Initialize();
	// プレイヤーのモデルの読み込み
	mp_player->Create(GetFullScreen());

	// ゲーム敵管理の生成
	mp_gameEnemyManager = std::make_unique<GameEnemyManager>();
	mp_gameEnemyManager->Initialize();
	// ゲーム敵管理のモデル読み込み
	mp_gameEnemyManager->Create();

	// ゲーム道路の生成
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	// ゲーム道路のモデル読み込み
	mp_gameRoad->Create();

	// ゲームマップの生成
	mp_gameMap = std::make_unique<GameMap>();
	mp_gameMap->Initialize();
	// ゲームマップのモデル読み込み
	mp_gameMap->Create();

	// マップ装飾品の生成
	mp_gameDecorateObject = std::make_unique<GameDecorateObject>();
	mp_gameDecorateObject->Create();
	mp_gameDecorateObject->Initialize();

	// スコアの生成
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create(m_isFullScreen, L"Resources\\Images\\GameScore\\score_len.png", L"Resources\\Images\\GameScore\\score_background.png");

	// フェード画像の生成
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());
	mp_fade->SetAlpha(1.0f);

	// スタート案内オブジェクトの生成
	m_startGudeWave = 0.1f;

	mp_startGuide = std::make_unique<Obj2D>();
	mp_startGuide->Create(L"Resources\\Images\\Play\\clicktocenter.png", nullptr);
	mp_startGuide->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 450.0f, 50.0f, 1.0f, 1.0f);
	if (GetFullScreen())
	{
		mp_startGuide->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startGuide->GetWidth()*0.5f),
							 (activeWndRect.bottom - activeWndRect.top) - (mp_startGuide->GetHeight()*2.0f)));
	}
	else
	{
		mp_startGuide->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startGuide->GetWidth()*0.5f),
							 (activeWndRect.bottom - (activeWndRect.top + titlebarHeight)) - (mp_startGuide->GetHeight()*5.0f)));
	}
	mp_startGuide->SetRect(0.0f, 0.0f, mp_startGuide->GetWidth(), mp_startGuide->GetHeight());


	// あらすじオブジェクトの生成
	mp_outline = std::make_unique<Obj2D>();
	mp_outline->Create(L"Resources\\Images\\Play\\outline_image.png", nullptr);
	mp_outline->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 700.0f, 500.0f, 1.0f, 1.0f);
	mp_outline->SetRect(0.0f, 0.0f, mp_outline->GetWidth(), mp_outline->GetHeight());
	float scale = (windowWidth - 500) / mp_outline->GetWidth();
	mp_outline->SetScale(scale);
	float outlineWidth = mp_outline->GetWidth() * scale;
	float outlineHeight = mp_outline->GetHeight() * scale;
	mp_outline->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (outlineWidth * 0.5f), (windowHeight * 0.5f) - (outlineHeight * 0.5f)));

	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();

	// ビュー行列の作成
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

	float aspectRatio = float(size.right) / float(size.bottom);
	// 画角を設定
	float angle = 45.0f;
	float fovAngleY = XMConvertToRadians(angle);

	// 射影行列を作成
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// 行列を設定
	mp_matrixManager->SetViewProjection(view, projection);

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
	// 行列管理変数の削除
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void ScenePlay::Update(DX::StepTimer const& timer)
{
	// シーン遷移せず、α値が0でなかったら
	if (!m_toResultMoveOnChecker && mp_fade->GetAlpha() != 0.0f)
	{
		// フェードイン
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_IN);
	}

	// サウンドの更新
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	// 相対モードなら何もしない
	if (InputManager::SingletonGetInstance().GetMouseState().positionMode == Mouse::MODE_RELATIVE) return;
	
	// アクティブなウィンドウのサイズ
	RECT activeWndRect;
	// アクティブなウィンドウのハンドルを取得
	HWND activeWnd = GetActiveWindow();
	// アクティブなウィンドウのハンドルからその画面の大きさを取得
	GetWindowRect(activeWnd, &activeWndRect);

	// ウィンドウのサイズを取得
	float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
	float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);

	// マウスポインターと円の衝突判定
	float distX = (windowWidth * 0.5f) - float(InputManager::SingletonGetInstance().GetMouseState().x);
	float distY;
	if(m_isFullScreen) distY = (windowHeight * 0.5f) - float(InputManager::SingletonGetInstance().GetMouseState().y);
	else distY = (windowHeight * 0.5f) - float(InputManager::SingletonGetInstance().GetMouseState().y + GetSystemMetrics(SM_CYCAPTION));
	float distX2 = distX * distX;
	float distY2 = distY * distY;
	float r = 100.0f;
	float r2 = r * r;

	// マウスポインターが画面中央に来たらゲームを開始する
	if (distX2 + distY2 <= r2 &&
		InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		isStartPlay = true;
	}

	// カメラの更新
	mp_camera->Update(timer, mp_player->GetPos(), mp_player->GetHeight(), mp_player->GetDir(), isStartPlay);

	//--------------------Judge Collision--------------------//

	// 道路とプレイヤーの衝突判定
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			SimpleMath::Vector3 roadpos = mp_gameRoad->GetRoadObject((int)mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x, (int)mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y).pos;
			SimpleMath::Vector3 playerpos = mp_player->GetPos();
			float distX = roadpos.z - playerpos.x;
			float distY = roadpos.x - playerpos.z;
			float dist = distX*distX + distY*distY;

			float radius = 1.0f + 5.0f;
			float rad = radius * radius;

			// ゴールに到達したら
			if (rad > dist)
			{
				// シーン遷移開始
				m_toResultMoveOnChecker = true;
				// クリア
				SceneManager::SetResultSceneState(true);
			}
		}
	}

	// 敵とプレイヤーの衝突判定
	for (int i = 0; i < mp_gameEnemyManager->GetMaxEnemyNum(); i++)
	{
		if (!mp_gameEnemyManager->GetEnemyState(i))continue;
		if (mp_gameEnemyManager->GetEnemyState(i))
		{
			if (Collision::HitCheck_Sphere2Box(mp_gameEnemyManager->GetEnemyCollide(i), mp_player->GetCollision()))
			{
				// 敵を非表示に
				mp_gameEnemyManager->SetEnemyState(i, false);
				// 敵に応じて負うダメージを分ける
				switch (mp_gameEnemyManager->GetEnemyType(i))
				{
				case GameEnemy::EnemyType::SPEED:
					mp_player->Damage(mp_gameEnemyManager->SPEED_ENEMY_DAMAGE);
					break;
				case GameEnemy::EnemyType::NORMAL:
					mp_player->Damage(mp_gameEnemyManager->NORMAL_ENEMY_DAMAGE);
					break;
				case GameEnemy::EnemyType::POWER:
					mp_player->Damage(mp_gameEnemyManager->POWER_ENEMY_DAMAGE);
					break;
				}
				// 負傷フラグを立てる
				mp_player->SetDamaged(true);
				break;
			}
		}
	}

	// 敵と弾の衝突判定
	for (int i = 0; i < mp_gameEnemyManager->GetMaxEnemyNum(); i++)
	{
		// 敵が生存していたら衝突判定を実行
		if (!mp_gameEnemyManager->GetEnemyState(i))continue;
		for (int j = 0; j < mp_player->GetBulletManager()->GetMaxBulletNum(); j++)
		{
			// 弾が発射されていたら衝突判定を実行
			if (!mp_player->GetBulletManager()->GetBulletState(j))continue;
			if (Collision::HitCheck_Sphere2Sphere(mp_gameEnemyManager->GetEnemyCollide(i),
				mp_player->GetBulletManager()->GetBulletCollide(j)))
			{
				// HPを削る
				mp_gameEnemyManager->SetEnemyHP(i, mp_gameEnemyManager->GetEnemyHP(i) - 1);
				// ヒットエフェクト
				mp_gameEnemyManager->SetHit(i, true, mp_player->GetBulletManager()->GetPos(j));
				ADX2Le* adx2le = ADX2Le::GetInstance();
				adx2le->Play(4);
				// HPが0になったら
				if (mp_gameEnemyManager->GetEnemyHP(i) == 0)
				{
					// 種類別で得点獲得
					switch (mp_gameEnemyManager->GetEnemyType(i))
					{
					case GameEnemy::EnemyType::NORMAL:
						mp_gameScore->FluctuationScore((int)GameEnemy::NORMAL_ENEMY_POINT);
						break;
					case GameEnemy::EnemyType::POWER:
						mp_gameScore->FluctuationScore((int)GameEnemy::POWER_ENEMY_POINT);
						break;
					case GameEnemy::EnemyType::SPEED:
						mp_gameScore->FluctuationScore((int)GameEnemy::SPEED_ENEMY_POINT);
						break;
					}

					// 敵を倒す
					mp_gameEnemyManager->SetEnemyState(i, false);
					// 煙と得点を表示
					mp_gameEnemyManager->ShockEnemy(i, true);
				}

				// stateをfalseに
				mp_player->GetBulletManager()->SetBulletState(j, false);
			}
		}
	}

	//--------------------------------------------------------//

	//--------------------Update 3D Object--------------------//

	// 弾の表示限界の設定
	SimpleMath::Vector3 pPos = mp_player->GetPos();
	SimpleMath::Vector3 bPos[10];
	for (int i = 0; i < mp_player->GetBulletManager()->GetMaxBulletNum(); i++)
	{
		// 発射されている弾のみ計測
		if (!mp_player->GetBulletManager()->GetBulletState(i))
		{
			break;
		}

		// しきい値(100)
		float length = 50.0f;
		float len = 0.0f;
		bPos[i] = mp_player->GetBulletManager()->GetPos(i);

		// 弾とプレイヤーの距離を計測
		len = MathManager::SingletonGetInstance().GetDistancePoints3D(pPos.x, pPos.y, pPos.z, bPos[i].x, bPos[i].y, bPos[i].z);

		// 距離が100を超えたら弾を消す
		if (length*length < len)
		{
			mp_player->GetBulletManager()->SetBulletState(i, false);
		}
	}

	// 道オブジェクトの更新
	mp_gameRoad->Update(timer);
	// マップの更新
	mp_gameMap->Update(timer, mp_player->GetPos());
	
	// プレイヤーの更新
	mp_player->Update(timer, isStartPlay, mp_camera->GetCameraAngle(), mp_gameMap->GetCorrectPos(), mp_gameEnemyManager->GetAssaultedState());

	// 敵の更新
	SimpleMath::Vector3 playerPassPos = mp_player->GetPassingRoad();
	mp_gameEnemyManager->Update(timer, mp_player->GetPos(),
								mp_gameRoad->GetRoadObject((int)playerPassPos.y, (int)playerPassPos.x).roadType,
								mp_gameRoad->GetRoadObject((int)playerPassPos.y, (int)playerPassPos.x).roadNum,
								mp_camera->GetCameraAngle(),
								mp_gameDecorateObject->GetChestPos(),
								mp_gameDecorateObject->GetChestHeight(),
								mp_gameDecorateObject->GetChestIsOpen());

	// 道中オブジェクトの更新
	int roadID = (mp_gameRoad->GetRoadObject((int)playerPassPos.y, (int)playerPassPos.x).roadType)*10 + 
				  mp_gameRoad->GetRoadObject((int)playerPassPos.y, (int)playerPassPos.x).roadNum;
	mp_gameDecorateObject->Update(roadID);

	//--------------------------------------------------------//

	//--------------------Update 2D Object--------------------//

	// スコアの更新
	mp_gameScore->Update(timer);
	if (mp_gameScore->GetScore() == 0)
	{
		// スコアが0だったらゲームオーバー
		SceneManager::SetResultSceneState(false);
	}
	// 体力がなくなったら
	if (mp_player->GetHP() == 0)
	{
		// ゲームオーバー
		SceneManager::SetResultSceneState(false);
		// シーン遷移開始
		m_toResultMoveOnChecker = true;
	}


	// スタート案内オブジェクトのフェード
	float sinWave = m_startGudeWave;
	m_startGudeWave += 0.1f;
	mp_startGuide->SetAlpha((sin(sinWave) * 2) * 0.5f);	// 0～1

	//--------------------------------------------------------//

	//------------------Operate Scene Object------------------//
	
	// シーン遷移発生
	if (m_toResultMoveOnChecker)
	{
		float fadeSpeed = 0.0f;
		if (SceneManager::GetResultSceneState())
		{
			fadeSpeed = CLEAR_FADE;
		}
		else
		{
			fadeSpeed = FAILD_FADE;
		}
		// フェードアウト
		mp_fade->Fade(fadeSpeed, Obj2D::FADE::FADE_OUT);
	}

	// フェードアウトが終わり、シーン遷移が発生していたら
	if (mp_fade->GetAlpha() >= 1.0f && m_toResultMoveOnChecker)
	{
		// リザルトシーンへ
		m_sceneManager->RequestToChangeScene(SCENE_RESULT);
	}

	//--------------------------------------------------------//
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void ScenePlay::Render()
{
	// ビュー行列の作成
	SimpleMath::Matrix view = SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), SimpleMath::Vector3::Up);
	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	float aspectRatio = float(size.right) / float(size.bottom);
	// 画角を設定
	float angle = 45.0f;
	float fovAngleY = XMConvertToRadians(angle);

	// 射影行列を作成
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// 行列を設定
	mp_matrixManager->SetViewProjection(view, projection);

	// 道オブジェクトの描画
	mp_gameRoad->Render(mp_matrixManager);
	// マップの描画
	mp_gameMap->Render(mp_matrixManager);

	// 装飾品の描画
	mp_gameDecorateObject->Render(mp_matrixManager);

	// 敵の描画
	SimpleMath::Vector3 playerGlance = mp_player->GetPos();
	playerGlance.y = mp_player->GetHeight();
	mp_gameEnemyManager->Render(mp_matrixManager, playerGlance);

	// プレイヤーの描画
	mp_player->Render(mp_matrixManager, mp_gameEnemyManager->GetDangerDir());

	// スタート前に表示
	if (!isStartPlay)
	{
		// あらすじの表示
		mp_outline->RenderAlphaScale();
		// スタート案内の表示
		mp_startGuide->RenderAlpha();
	}

	// スコアの描画
	mp_gameScore->Render();

	
	// フェード画像の表示
	mp_fade->RenderAlpha();
}
