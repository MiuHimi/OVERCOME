//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneLogo.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const int SceneLogo::FADEOUT_NEED_SECOND = 2;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneLogo::SceneLogo(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toTitleMoveOnChecker(false),
	  mp_background(nullptr), mp_logo(nullptr), mp_fade(nullptr),
	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneLogo::~SceneLogo()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneLogo::Initialize()
{
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

	// 背景の生成
	mp_background = std::make_unique<Obj2D>();
	mp_background->Create(L"Resources\\Images\\black.png", nullptr);
	mp_background->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_background->SetRect(0.0f, 0.0f, mp_background->GetWidth(), mp_background->GetHeight());

	// ロゴの生成
	mp_logo = std::make_unique<Obj2D>();
	mp_logo->Create(L"Resources\\Images\\Logo\\himi_logo.png", nullptr); // ホバースプライトなし
	if (GetFullScreen())
	{
		mp_logo->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 420.0f * 1.5f, 350.0f * 1.5f, 1.0f, 1.0f);
		mp_logo->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_logo->GetWidth() * 0.5f),
											(windowHeight * 0.5f) - (mp_logo->GetHeight() * 0.5f)));
		float width = mp_logo->GetWidth() / 1.5f;
		float height = mp_logo->GetHeight() / 1.5f;
		mp_logo->SetRect(0, 0, width, height);
		mp_logo->SetScale(1.5f);
	}
	else
	{
		mp_logo->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 420.0f, 350.0f, 1.0f, 1.0f);
		// ウィンドウ上部分だけ追加で演算する
		mp_logo->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_logo->GetWidth() * 0.5f),
											(windowHeight * 0.5f) - (mp_logo->GetHeight() * 0.5f) - titlebarHeight));
		mp_logo->SetRect(0.0f, 0.0f, mp_logo->GetWidth(), mp_logo->GetHeight());
	}
	
	// フェード画像の生成
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());

	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();

	// ビュー行列の作成
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

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
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneLogo::Finalize()
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
void SceneLogo::Update(DX::StepTimer const& timer)
{
	// フレームをカウント
	static int frameCount = 0;
	frameCount++;

	// 秒数をカウントするために割る
	int framesPerSecond = 60;
	int elapsedSecond = frameCount / framesPerSecond;

	// フェードアウト開始
	bool fadeOutFlag = false;
	if (elapsedSecond >= FADEOUT_NEED_SECOND)
	{
		fadeOutFlag = mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// フェードアウトが終了したら
	if (fadeOutFlag)
	{
		m_toTitleMoveOnChecker = true;
	}

	// シーン遷移
	if (m_toTitleMoveOnChecker)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneLogo::Render()
{
	// 背景の表示
	mp_background->Render();
	//ロゴの表示
	mp_logo->RenderAlphaScale();
	// フェード画像の表示
	mp_fade->RenderAlpha();
}
