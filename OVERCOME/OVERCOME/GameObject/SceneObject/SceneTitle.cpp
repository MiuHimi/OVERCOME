//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneTitle::SceneTitle(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toStageSelectMoveOnChecker(false),
	  m_colorAlpha(0.0f),
	  mp_title(nullptr), mp_startBtn(nullptr), mp_fade(nullptr),
	  mp_camera(nullptr),
	  mp_modelHouse(nullptr),
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
	mp_title = std::make_unique<Obj2D>();
	mp_title->Create(L"Resources\\Images\\Title\\title_image.png", nullptr); // ホバースプライトなし
	if (GetFullScreen())
		mp_title->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 500.0f * 2.0f, 120.0f * 2.0f, 1.0f, 1.0f);
	else
		mp_title->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 500.0f, 120.0f, 1.0f, 1.0f);
	mp_title->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_title->GetWidth() * 0.5f),
										 (windowHeight * 0.5f) - (mp_title->GetHeight() * 1.5f) - titlebarHeight));
	mp_title->SetRect(0.0f, 0.0f, mp_title->GetWidth(), mp_title->GetHeight());

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
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// 家モデルを作成
	mp_modelHouse = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\house.cmo", fx);

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
	// サウンドの更新
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// エフェクトの更新
	mp_effectManager->Update(timer);

	// カメラの更新(タイトルシーンのカメラは定点カメラ)
	mp_camera->Update(timer, SimpleMath::Vector3(0.0f,0.0f,0.0f), 0.0f, SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	
	// マウスの更新
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
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
		mp_title->SetAlpha(m_colorAlpha);

		// フェードアウト
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// シーン遷移
	if (m_toStageSelectMoveOnChecker && m_colorAlpha <= 0.0f && mp_fade->GetAlpha() >= 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
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

	// エフェクトの描画
	mp_effectManager->Render();

	// タイトルの表示
	if (GetFullScreen())
	{
		float width = mp_title->GetWidth() * 0.5f;
		float height = mp_title->GetHeight() * 0.5f;
		mp_title->SetRect(0, 0, width, height);
		mp_title->SetScale(2.0f);
		mp_title->RenderAlphaScale();
	}
	else
	{
		mp_title->RenderAlphaScale();
	}

	// スタートボタンの表示
	mp_startBtn->RenderAlpha();

	// フェード画像の表示
	mp_fade->RenderAlpha();
}
