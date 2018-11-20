//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneResult.h"

#include "../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


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
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneResult::Finalize()
{
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneResult::Update(DX::StepTimer const& timer)
{
	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	// リザルトシーンの状態を設定
	m_resultState = SceneManager::GetResultSceneState();

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
	MatrixManager::SingletonGetInstance().SetViewProjection(view, projection);

	// デバッグ用
	GameDebug::SingletonGetInstance().DebugRender("SceneResult", DirectX::SimpleMath::Vector2(20.0f, 10.0f));
	if (m_resultState == true) GameDebug::SingletonGetInstance().DebugRender("Clear", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	if (m_resultState == false)GameDebug::SingletonGetInstance().DebugRender("GameOver", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	GameDebug::SingletonGetInstance().DebugRender("SPACEkey to SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	GameDebug::SingletonGetInstance().Render();
}
