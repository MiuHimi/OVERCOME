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
#include "../../Utility/CommonStateManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

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
	  mp_matrixManager(nullptr),
	  m_color(0.0f)
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

	// テクスチャのロード
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\title_background.png", nullptr, m_textureBackground.GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\title.png", nullptr, m_textureTitle.GetAddressOf());

	// スプライトバッチの初期化
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();
	
	// ビュー行列の作成
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

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

	// エフェクトマネージャーの初期化
	//mp_effectManager = nullptr;
	//mp_effectManager = new EffectManager();
	//mp_effectManager->Create();
	//mp_effectManager->Initialize(5, SimpleMath::Vector3(0, 0, 0), SimpleMath::Vector3(0, 0, 0));
	//mp_effectManager->SetRenderState(view, projection);

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
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

	/*if (mp_effectManager != nullptr) {
		mp_effectManager->Lost();
		delete mp_effectManager;
		mp_effectManager = nullptr;
	}*/
	
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

	//mp_effectManager->Update(timer);
	
	// マウスの更新
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
	InputManager::SingletonGetInstance().Update();

	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toPlayMoveOnChecker = true;
		adx2le->Play(1);
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_color += 0.01f;
	}

	if (m_toPlayMoveOnChecker == true && m_color > 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
	}

}

/// <summary>
/// タイトルシーンの描画処理
/// </summary>
void SceneTitle::Render()
{
	//mp_effectManager->Render();

	// タイトルの描画
	mp_sprite->Begin(DirectX::SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());
	
	// 切り取る場所を設定
	RECT rectBG;
	rectBG.top = LONG(0.0f);
	rectBG.left = LONG(0.0f);
	rectBG.right = LONG(800.0f);
	rectBG.bottom = LONG(600.0f);

	RECT rectTite;
	rectTite.top = LONG(0.0f);
	rectTite.left = LONG(0.0f);
	rectTite.right = LONG(500.0f);
	rectTite.bottom = LONG(120.0f);

	mp_sprite->Draw(m_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectBG, SimpleMath::Vector4(1.0f- m_color, 1.0f- m_color, 1.0f- m_color, 1.0f), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	mp_sprite->Draw(m_textureTitle.Get(), SimpleMath::Vector2(150.0f, 100.0f), &rectTite, SimpleMath::Vector4(1.0f- m_color, 1.0f- m_color, 1.0f- m_color, 1.0f), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	
	mp_sprite->End();

}
