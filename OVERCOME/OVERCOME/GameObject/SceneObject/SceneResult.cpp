//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2019/06/12
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
	  m_scorePos(0.0f,0.0f),
	  mp_gameScore(nullptr),
	  mp_textureScore(nullptr),
	  mp_textureBackground(nullptr),
	  mp_sprite(nullptr),
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

	// スコアの表示位置の初期化
	m_scorePos = SimpleMath::Vector2(280.0f, 400.0f);

	// スコアのポインタの初期化
	mp_gameScore = std::make_unique<GameScore>();

	// テクスチャのロード
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_length.png", nullptr, mp_textureScore.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Result\\result_background_image.png", nullptr, mp_textureBackground.GetAddressOf());

	// スプライトバッチの初期化
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

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

	// 画面クリックでシーン遷移発生
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toTitleMoveOnChecker = true;
	}
	// キー入力でもシーン遷移発生
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}

	// シーン遷移が発生したら
	if (m_toTitleMoveOnChecker)
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

	// リザルト画面の描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// 背景
	RECT rectBG = { 0, 0, 800, 600 };
	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectBG, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// スコア
	for (int i = 0; i < Digit::NUM; i++)
	{
		RECT rect;
		// 桁に応じて切り取る位置を変える
		switch (i)
		{
		// 1000の位
		case THOUSAND:
			rect = { int(SCORE_SIZE) * thousandDigit, 0, int(SCORE_SIZE) * thousandDigit + int(SCORE_SIZE), int(SCORE_SIZE) };
			break;
		// 100の位
		case HUNDRED: 
			rect = { SCORE_SIZE * hundredDigit, 0, SCORE_SIZE * hundredDigit + SCORE_SIZE, SCORE_SIZE }; 
			break;
		// 10の位
		case TEN:
			rect = { SCORE_SIZE * tenDigit, 0, SCORE_SIZE * tenDigit + SCORE_SIZE, SCORE_SIZE };
			break;
		// 1の位
		case ONE:
			rect = { SCORE_SIZE * oneDigit, 0, SCORE_SIZE * oneDigit + SCORE_SIZE, SCORE_SIZE };
			break;
		// それ以外の位
		default:
			break;
		}

		mp_sprite->Draw(mp_textureScore.Get(), SimpleMath::Vector2(m_scorePos.x + float(i * SCORE_SIZE), m_scorePos.y - (float)SCORE_SIZE / 2.0f), &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
}
