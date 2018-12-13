//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneLogo.h"

#include "../../Utility/GameDebug.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/MatrixManager.h"

#include "../Utility/DrawManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneLogo::SceneLogo(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  m_toTitleMoveOnChecker(false),
	  m_fadeoutNeedTime(2),
	  m_changeSceneNeedTime(4),
	  fadeoutCount(0)
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
	// テクスチャのロード
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureBackground.GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\logo_image.png", nullptr, m_textureLogo.GetAddressOf());
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneLogo::Finalize()
{
}

/// <summary>
/// ロゴシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneLogo::Update(DX::StepTimer const& timer)
{
	// フレームをカウント
	static int count = 0;
	count++;

	// フェードアウト開始
	if (count / 60 >= m_fadeoutNeedTime)
	{
		fadeoutCount += 0.01f;
	}

	// シーン遷移
	if (count / 60 >= m_changeSceneNeedTime)
	{
		m_toTitleMoveOnChecker = true;
		count = 0;
	}

	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneLogo::Render()
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

	// ロゴの描画
	DrawManager::SingletonGetInstance().Draw(m_textureLogo.Get(), SimpleMath::Vector2(0.0f, 0.0f));
	DrawManager::SingletonGetInstance().DrawAlpha(m_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0, 1.0f, 1.0f, fadeoutCount));
}
