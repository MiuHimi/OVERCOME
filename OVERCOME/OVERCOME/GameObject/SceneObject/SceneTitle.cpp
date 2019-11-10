//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <math.h>

#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const int SceneTitle::TITLE_STR_WIDTH = 60;
const int SceneTitle::MAX_GHOST_POS_X = 20;
const float SceneTitle::MAX_GHOST_POS_Y = 57.0f;
const float SceneTitle::MIN_GHOST_POS_Y = 43.0f;
const int SceneTitle::RESPAWN_COUNT_MIN = 180;
const int SceneTitle::RESPAWN_COUNT_MAX = 240;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneTitle::SceneTitle(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toStageSelectMoveOnChecker(false),
	  m_colorAlpha(0.0f),
	  mp_title{ nullptr }, mp_startBtn(nullptr), mp_fade(nullptr),
	  mp_camera(nullptr),
	  m_ghostPos(SimpleMath::Vector3(0.0f,0.0f,0.0f)), m_ghostDir(SceneTitle::GHOST_DIR::NONE),
	  m_spawnCount(0), m_nextSpawnCount(0),
	  mp_modelHouse(nullptr), mp_modelEnemy(nullptr),
	  mp_matrixManager(nullptr),
	  mp_effectManager(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneTitle::~SceneTitle()
{
}

/// <summary>
/// タイトルシーンの初期化処理
/// </summary>
void SceneTitle::Initialize()
{
	m_toStageSelectMoveOnChecker = false;

	// α値の設定(初期化)
	m_colorAlpha = 1.0f;

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

	// タイトルの生成
	for (int i = 0; i < MAX_TITLE_STR; i++)
	{
		mp_title[i] = std::make_unique<Obj2D>();
		mp_title[i]->Create(L"Resources\\Images\\Title\\title_stitch_image.png", nullptr); // ホバースプライトなし
		if (GetFullScreen())
		{
			mp_title[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), (float)TITLE_STR_WIDTH * 2.0f, 200 * 2.0f, 1.0f, 1.0f);
			mp_title[i]->SetScale(2.0f);
		}
		else
		{
			mp_title[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), (float)TITLE_STR_WIDTH, 200.0f, 1.0f, 1.0f);
		}
		mp_title[i]->SetPos(SimpleMath::Vector2(((windowWidth * 0.5f) - ((float)TITLE_STR_WIDTH * (float)MAX_TITLE_STR/* * 0.5f*/))+ i*mp_title[i]->GetWidth(),0.0f));
		mp_title[i]->SetRect((float)i * (float)TITLE_STR_WIDTH, 0.0f, ((float)i * (float)TITLE_STR_WIDTH) + (float)TITLE_STR_WIDTH, mp_title[i]->GetHeight() * 0.5f);
	}
	
	// スタートボタンの生成
	mp_startBtn = std::make_unique<Obj2D>();
	mp_startBtn->Create(L"Resources\\Images\\Title\\title_gamestart.png", L"Resources\\Images\\Title\\title_gamestart_hover.png");
	mp_startBtn->Initialize(SimpleMath::Vector2(0.0f,0.0f), 640.0f, 100.0f, 1.0f, 1.0f);
	mp_startBtn->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_startBtn->GetWidth() * 0.5f),
										   ((windowHeight - (mp_startBtn->GetHeight() * 2.0f)))));
	mp_startBtn->SetRect(0.0f, 0.0f, mp_startBtn->GetWidth(), mp_startBtn->GetHeight());

	// フェード画像の生成
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	// カメラオブジェクトの作成
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom, m_isFullScreen);

	// 幽霊初期位置
	m_ghostPos = SimpleMath::Vector3(-20.0f, 45.0f, 130.0f);
	m_ghostDir = GHOST_DIR::RIGHT_DIR;
	m_nextSpawnCount = 30; // デフォルト値

	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// 家モデルを作成
	mp_modelHouse = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\house.cmo", fx);
	// 敵モデルを作成
	mp_modelEnemy = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost.cmo", fx);

	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();
	
	// ビュー行列の作成
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

	// ウインドウサイズからアスペクト比を算出する
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

	// エフェクトマネージャーの初期化
	mp_effectManager = new EffectManager();
	mp_effectManager->Create();
	mp_effectManager->Initialize();
	mp_effectManager->SetRenderState(view, projection);

	// サウンド再生
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"SceneTitle.acb", L"SceneTitle.awb");
	adx2le->Play(0);
}
/// <summary>
/// タイトルシーンの終了処理
/// </summary>
void SceneTitle::Finalize()
{
	// 行列管理変数の削除
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

	// エフェクト管理変数の削除
	if (mp_effectManager != nullptr) {
		mp_effectManager->Finalize();
		delete mp_effectManager;
		mp_effectManager = nullptr;
	}

	// サウンドの停止
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();
}

/// <summary>
/// タイトルシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneTitle::Update(DX::StepTimer const& timer)
{
	// サイン波が変動するための値
	static float ghostWave;
	ghostWave += 0.05f;

	// サウンドの更新
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// エフェクトの更新
	mp_effectManager->Update(timer);

	// ライトの計算
	/*auto SetLight = [&](IEffect* effect)
	{
		IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
		if (lights)
		{
			lights->SetAmbientLightColor(SimpleMath::Vector3(0.5f, 0.5f, 0.5f)); // アンビエント色を設定する
			lights->SetLightEnabled(0, true);                        // １番のライトON
			lights->SetLightEnabled(1, false);                       // ２番のライトOFF
			lights->SetLightEnabled(2, false);                       // ３番のライトOFF
			lights->SetLightDiffuseColor(0, Colors::Red);            // ライトの色
			SimpleMath::Vector3 light_dir(0.0f, 0.0f, -1);           // ライトの方向ベクトル
			light_dir.Normalize();                                   // ライトの方向ベクトルを正規化する
			lights->SetLightDirection(0, light_dir);                 // １番のライトの方向を設定する
			lights->SetPerPixelLighting(true);                       // ピクセルシェーダで光の影響を計算する
		}
	};
	mp_modelEnemy->UpdateEffects(SetLight);*/

	// 幽霊の移動
	if(m_ghostDir == GHOST_DIR::RIGHT_DIR)
		m_ghostPos.x += 0.1f;
	else if (m_ghostDir == GHOST_DIR::LEFT_DIR)
		m_ghostPos.x -= 0.1f;
	// ふわふわさせるためにサイン波を使用
	float sinWave = sin(ghostWave) * 0.05f;
	m_ghostPos.y += sinWave;

	// 幽霊の方向転換
	if (m_ghostPos.x > (int)MAX_GHOST_POS_X)
	{
		m_spawnCount++;
		if (m_spawnCount > m_nextSpawnCount)
		{
			// 幽霊を反対向きに
			m_ghostDir = GHOST_DIR::LEFT_DIR;
			// 幽霊の高さ設定
			m_ghostPos.y = (float)(MIN_GHOST_POS_Y + rand()*(MAX_GHOST_POS_Y - MIN_GHOST_POS_Y + 1) / (1 + RAND_MAX));
			// カウントをリセット
			m_spawnCount = 0;
			// 次にスポーンするのに必要なカウントを設定
			m_nextSpawnCount = RESPAWN_COUNT_MIN + rand()*(RESPAWN_COUNT_MAX - RESPAWN_COUNT_MIN + 1) / (1 + RAND_MAX);
		}
	}
	else if (m_ghostPos.x < (int)-MAX_GHOST_POS_X)
	{
		m_spawnCount++;
		if (m_spawnCount > m_nextSpawnCount)
		{
			// 幽霊を反対向きに
			m_ghostDir = GHOST_DIR::RIGHT_DIR;
			// 幽霊の高さ設定
			m_ghostPos.y = (float)(MIN_GHOST_POS_Y + rand()*(MAX_GHOST_POS_Y - MIN_GHOST_POS_Y + 1) / (1 + RAND_MAX));
			// カウントをリセット
			m_spawnCount = 0;
			// 次にスポーンするのに必要なカウントを設定
			m_nextSpawnCount = RESPAWN_COUNT_MIN + rand()*(RESPAWN_COUNT_MAX - RESPAWN_COUNT_MIN + 1) / (1 + RAND_MAX);
		}
	}


	// カメラの更新(タイトルシーンのカメラは定点カメラ)
	mp_camera->Update(timer, SimpleMath::Vector3(0.0f,0.0f,0.0f), 0.0f, SimpleMath::Vector3(0.0f, 0.0f, 0.0f), true);
	
	// マウスの更新
	InputManager::SingletonGetInstance().Update();

	// スタートボタンとマウスカーソルの衝突判定
	SimpleMath::Vector2 mousePos = SimpleMath::Vector2((float)InputManager::SingletonGetInstance().GetMousePosX(),
													   (float)InputManager::SingletonGetInstance().GetMousePosY());
	SimpleMath::Vector2 btnPos = mp_startBtn->GetPos();
	float btnWidth = mp_startBtn->GetWidth();
	float btnHeight = mp_startBtn->GetHeight();
	// マウスがボタンに接触していたら
	if (mp_startBtn->IsCollideMouse(mousePos, btnPos, btnWidth, btnHeight))
		mp_startBtn->SetHover(true);	//	ホバー状態にする
	// 接触していなかったら
	else
		mp_startBtn->SetHover(false);	// ホバー状態にしない

	// サイン波が変動するための値
	static float wave;
	wave += 0.01f;

	// タイトルロゴのアニメーション
	for (int i = 0; i < MAX_TITLE_STR; i++)
	{
		float sinWave = (sin(wave + (i*2.0f)) - 1.0f)* 30.0f;
		mp_title[i]->SetPos(SimpleMath::Vector2(mp_title[i]->GetPos().x, sinWave));
	}

	// シーン遷移せず、α値が0でなかったら
	if (!m_toStageSelectMoveOnChecker && mp_fade->GetAlpha() != 0.0f)
	{
		// フェードイン
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_IN);
	}

	// スタートボタン左クリックでフェード開始
	if (mp_startBtn->GetHover() && InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toStageSelectMoveOnChecker = true;
		adx2le->Play(1);
	}
	if (m_toStageSelectMoveOnChecker)
	{
		m_colorAlpha -= 0.01f;
		if (m_colorAlpha < 0.0f) m_colorAlpha = 0.0f;

		// クリックされたらホバー状態に固定
		mp_startBtn->SetHover(true);
		// α値設定
		mp_startBtn->SetAlpha(m_colorAlpha);
		for (int i = 0; i < MAX_TITLE_STR; i++)
		{
			mp_title[i]->SetAlpha(m_colorAlpha);
		}

		adx2le->SetVolume(m_colorAlpha);

		// フェードアウト
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// シーン遷移
	if (m_toStageSelectMoveOnChecker && m_colorAlpha <= 0.0f && mp_fade->GetAlpha() >= 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// タイトルシーンの描画処理
/// </summary>
void SceneTitle::Render()
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

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	// 家の描画
	mp_modelHouse->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
						world, mp_matrixManager->GetView(), mp_matrixManager->GetProjection());

	
	SimpleMath::Matrix rota = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians((float)m_ghostDir));
	world *= rota;
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_ghostPos);
	world *= trans;

	mp_modelEnemy->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
						world, mp_matrixManager->GetView(), mp_matrixManager->GetProjection());

	// エフェクトの描画
	mp_effectManager->Render();

	// タイトルの表示
	if (GetFullScreen())
	{
		for (int i = 0; i < MAX_TITLE_STR; i++)
		{
			mp_title[i]->RenderAlphaScale();
		}
	}
	else
	{
		for (int i = 0; i < MAX_TITLE_STR; i++)
		{
			mp_title[i]->RenderAlphaScale();
		}
	}

	// スタートボタンの表示
	mp_startBtn->RenderAlpha();

	// フェード画像の表示
	mp_fade->RenderAlpha();
}
