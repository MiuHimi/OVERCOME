//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneResult.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"
#include "../../Utility/DrawManager.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

int GameScore::m_score;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneResult::SceneResult(SceneManager * sceneManager)
	: SceneBase(sceneManager)
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
	m_toTitleMoveOnChecker = false;

	for (int i = 0; i < 3; i++)
	{
		DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_length.png", nullptr, m_textureScore[i].GetAddressOf());
	}

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Result\\result_background_image.png", nullptr, m_textureBackground.GetAddressOf());

	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();

	// ビュー行列の作成
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

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
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneResult::Finalize()
{
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

	// リザルトシーンの状態を設定
	m_resultState = SceneManager::GetResultSceneState();

	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toTitleMoveOnChecker = true;
	}

	// キー入力
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}
	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneResult::Render()
{
	int score = GameScore::GetScore();

	// 背景
	DrawManager::SingletonGetInstance().Draw(m_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// スコア
	int hundredDigit = score / 100;
	int tenDigit = ((score % 100) / 10);
	int oneDigit = score % 10;

	/*for (int i = 0; i < 3; i++)
	{
		RECT rect;
		// 切り取る場所を設定
		if (i = 0)
		{
			rect.top = LONG(0.0f);
			rect.left = LONG(m_scoreNumSize * hundredDigit);
			rect.right = LONG(m_scoreNumSize * hundredDigit + m_scoreNumSize);
			rect.bottom = LONG(m_scoreNumSize);
			DrawManager::SingletonGetInstance().DrawRect(m_textureScore[i].Get(), DirectX::SimpleMath::Vector2(320 + (i * float(m_scoreNumSize)), 400.0f - m_scoreNumSize / 2), &rect);

		}
		if (i = 1)
		{
			rect.top = LONG(0.0f);
			rect.left = LONG(m_scoreNumSize * tenDigit);
			rect.right = LONG(m_scoreNumSize * tenDigit + m_scoreNumSize);
			rect.bottom = LONG(m_scoreNumSize);
			DrawManager::SingletonGetInstance().DrawRect(m_textureScore[i].Get(), DirectX::SimpleMath::Vector2(320 + (i * float(m_scoreNumSize)), 400.0f - m_scoreNumSize / 2), &rect);

		}
		if (i = 2)
		{
			rect.top = LONG(0.0f);
			rect.left = LONG(m_scoreNumSize * oneDigit);
			rect.right = LONG(m_scoreNumSize * oneDigit + m_scoreNumSize);
			rect.bottom = LONG(m_scoreNumSize);
			DrawManager::SingletonGetInstance().DrawRect(m_textureScore[i].Get(), DirectX::SimpleMath::Vector2(320 + (i * float(m_scoreNumSize)), 400.0f - m_scoreNumSize / 2), &rect);

		}
		
	}*/

	RECT rect;

	rect.top = LONG(0.0f);
	rect.left = LONG(m_scoreNumSize * hundredDigit);
	rect.right = LONG(m_scoreNumSize * hundredDigit + m_scoreNumSize);
	rect.bottom = LONG(m_scoreNumSize);
	DrawManager::SingletonGetInstance().DrawRect(m_textureScore[0].Get(), DirectX::SimpleMath::Vector2(320.0f, 400.0f - m_scoreNumSize / 2), &rect);

	rect.top = LONG(0.0f);
	rect.left = LONG(m_scoreNumSize * tenDigit);
	rect.right = LONG(m_scoreNumSize * tenDigit + m_scoreNumSize);
	rect.bottom = LONG(m_scoreNumSize);
	DrawManager::SingletonGetInstance().DrawRect(m_textureScore[0].Get(), DirectX::SimpleMath::Vector2(400.0f, 400.0f - m_scoreNumSize / 2), &rect);

	rect.top = LONG(0.0f);
	rect.left = LONG(m_scoreNumSize * oneDigit);
	rect.right = LONG(m_scoreNumSize * oneDigit + m_scoreNumSize);
	rect.bottom = LONG(m_scoreNumSize);
	DrawManager::SingletonGetInstance().DrawRect(m_textureScore[0].Get(), DirectX::SimpleMath::Vector2(480.0f, 400.0f - m_scoreNumSize / 2), &rect);

	// デバッグ用
	//GameDebug::SingletonGetInstance().DebugRender("SceneResult", DirectX::SimpleMath::Vector2(20.0f, 10.0f));
	//if (m_resultState == true) GameDebug::SingletonGetInstance().DebugRender("Clear", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	//if (m_resultState == false)GameDebug::SingletonGetInstance().DebugRender("GameOver", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	//GameDebug::SingletonGetInstance().DebugRender("SPACEkey to SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	//GameDebug::SingletonGetInstance().Render();
}
