//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager)
	: SceneBase(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneStageSelect::Initialize()
{
	m_toPlayMoveOnChecker = false;
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneStageSelect::Finalize()
{
}

/// <summary>
/// ステージ選択シーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	static int count = 0;
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	{
		count++;
		if (count > 3)count = 0;
		SceneManager::SetStageNum(count);
	}

	// キー入力
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneStageSelect::Render()
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
	GameDebug::SingletonGetInstance().DebugRender("SceneStageSelect", DirectX::SimpleMath::Vector2(20.0f, 10.0f));

	if (SceneManager::GetStageNum() != 1 && SceneManager::GetStageNum() != 2)
	{
		GameDebug::SingletonGetInstance().DebugRender("StageNone", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	}
	else 
	{
		GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	}

	GameDebug::SingletonGetInstance().Render();
}
