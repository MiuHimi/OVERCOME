//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2019/09/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneResult.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const int SceneResult::SCORE_SIZE = 80;

// スコアを取得できるようにする
int GameScore::m_score;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneResult::SceneResult(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toTitleMoveOnChecker(false),
	  m_returnToPlayChecker(false),
	  m_resultState(false),
	  m_scoreBasePos(0.0f,0.0f),
	  mp_gameScore(nullptr),
	  mp_fade(nullptr), mp_score(nullptr), mp_bg(nullptr), mp_resultStr(nullptr),
	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneResult::~SceneResult()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneResult::Initialize()
{
	// シーン遷移、リザルト状態のフラグの初期化
	m_toTitleMoveOnChecker = false;
	m_returnToPlayChecker = false;
	m_resultState = false;

	// スコアのポインタの初期化
	mp_gameScore = std::make_unique<GameScore>();

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

	// フェード画像の生成
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());

	// スコアの表示位置の初期化
	m_scoreBasePos = SimpleMath::Vector2(float((windowWidth * 0.5f)) - float((SCORE_SIZE)*(Digit::NUM * 0.5f)), float(windowHeight * 0.5f) - float(SCORE_SIZE * 0.5f));

	// スコアオブジェクトの生成
	mp_score = std::make_unique<Obj2D>();
	mp_score->Create(L"Resources\\Images\\ScoreCount\\count_length.png", nullptr);
	mp_score->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(SCORE_SIZE), float(SCORE_SIZE), 1.0f, 1.0f);
	if (m_isFullScreen)
	{
		mp_score->SetPos(SimpleMath::Vector2(m_scoreBasePos.x,
											 (activeWndRect.bottom - activeWndRect.top) - (10.0f + mp_score->GetHeight())));
	}
	else
	{
		mp_score->SetPos(SimpleMath::Vector2(m_scoreBasePos.x,
											 float(activeWndRect.bottom - (activeWndRect.top + titlebarHeight + 20.0f)) - float(10.0f + mp_score->GetHeight())));
	}
	mp_score->SetRect(0.0f, 0.0f, mp_score->GetWidth(), mp_score->GetHeight());

	// 背景の生成
	mp_bg = std::make_unique<Obj2D>();
	mp_bg->Create(L"Resources\\Images\\gray.png", nullptr);
	mp_bg->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_bg->SetRect(0.0f, 0.0f, mp_bg->GetWidth(), mp_bg->GetHeight());

	// RESULT文字列の生成
	mp_resultStr = std::make_unique<Obj2D>();
	mp_resultStr->Create(L"Resources\\Images\\Result\\result_image.png", nullptr);
	mp_resultStr->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 275.0f, 50.0f, 1.0f, 1.0f);
	mp_resultStr->SetRect(0.0f, 0.0f, mp_resultStr->GetWidth(), mp_resultStr->GetHeight());
	if (m_isFullScreen)
	{
		mp_resultStr->SetScale(2.0f);
		mp_resultStr->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - ((mp_resultStr->GetWidth() * 2.0f) * 0.5f), mp_resultStr->GetHeight() * 2.0f));
	}
	else
	{
		mp_resultStr->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_resultStr->GetWidth() * 0.5f), mp_resultStr->GetHeight()));
	}

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
void SceneResult::Finalize()
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
void SceneResult::Update(DX::StepTimer const& timer)
{
	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();
	// マウスの更新
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// スコアの更新
	mp_gameScore->Update(timer);

	// リザルトシーンの状態を取得
	m_resultState = SceneManager::GetResultSceneState();

	// 画面クリックまたはキー入力でシーン遷移発生
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED ||
		InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}

	bool isFinishedEffect = false;
	if (m_toTitleMoveOnChecker)
	{
		// α値が1でなかったら
		if (mp_fade->GetAlpha() != 1.0f)
		{
			// フェードアウト
			isFinishedEffect = mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
		}
	}

	// シーン遷移が終了したら
	if (isFinishedEffect)
	{
		// タイトルに戻る
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneResult::Render()
{
	// スコアの取得
	int score = mp_gameScore->GetScore();

	// スコア
	int thousandDigit = score / 1000;		   // 1000の位
	int hundredDigit = ((score / 100) % 10);   // 100の位
	int tenDigit = ((score % 100) / 10);	   // 10の位
	int oneDigit = score % 10;				   // 1の位

	// 背景の表示
	mp_bg->Render();

	// RESULT文字列の表示
	mp_resultStr->RenderAlphaScale();

	// スコア
	for (int i = 0; i < Digit::NUM; i++)
	{
		// 表示位置設定
		mp_score->SetPos(SimpleMath::Vector2(m_scoreBasePos.x + float(i * SCORE_SIZE), m_scoreBasePos.y));

		RECT rect;
		// 桁に応じて切り取る位置を変える
		switch (i)
		{
		// 1000の位
		case THOUSAND:
			rect = { int(SCORE_SIZE) * thousandDigit, 0, int(SCORE_SIZE) * thousandDigit + int(SCORE_SIZE), int(SCORE_SIZE) };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// 100の位
		case HUNDRED: 
			rect = { SCORE_SIZE * hundredDigit, 0, SCORE_SIZE * hundredDigit + SCORE_SIZE, SCORE_SIZE };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// 10の位
		case TEN:
			rect = { SCORE_SIZE * tenDigit, 0, SCORE_SIZE * tenDigit + SCORE_SIZE, SCORE_SIZE };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// 1の位
		case ONE:
			rect = { SCORE_SIZE * oneDigit, 0, SCORE_SIZE * oneDigit + SCORE_SIZE, SCORE_SIZE };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// それ以外の位
		default:
			break;
		}

		mp_score->Render();
	}

	// フェード画像の表示
	mp_fade->RenderAlpha();
}
