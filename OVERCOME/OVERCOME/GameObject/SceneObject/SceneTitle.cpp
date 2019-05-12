//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneTitle::SceneTitle(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  mp_matrixManager(nullptr)
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
	m_toPlayMoveOnChecker = false;

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
/// タイトルシーンの終了処理
/// </summary>
void SceneTitle::Finalize()
{
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

}

/// <summary>
/// タイトルシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneTitle::Update(DX::StepTimer const& timer)
{
	// マウスの更新
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
	InputManager::SingletonGetInstance().Update();

	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
	}

}

/// <summary>
/// タイトルシーンの描画処理
/// </summary>
void SceneTitle::Render()
{
	GameDebug::SingletonGetInstance().DebugRender("SceneTitle", SimpleMath::Vector2(10.0f, 10.0f));
	GameDebug::SingletonGetInstance().Render();
}
