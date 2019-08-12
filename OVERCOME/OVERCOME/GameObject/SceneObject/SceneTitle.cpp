//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/06/06
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
SceneTitle::SceneTitle(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  m_toStageSelectMoveOnChecker(false),
      m_colorAlpha(0.0f),
	  mp_textureBackground(nullptr),
	  mp_textureTitle(nullptr),
	  mp_sprite(nullptr),
	  m_titleWidth(0.0f),
	  m_titleHeight(0.0f),
	  m_TitlePos(0.0f, 0.0f),
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

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	// カメラオブジェクトの作成
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// テクスチャのロード
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Title\\title_background.png", nullptr, mp_textureBackground.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Title\\title.png", nullptr, mp_textureTitle.GetAddressOf());

	// スプライトバッチの初期化
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// タイトルの幅、高さ、位置設定
	m_titleWidth = 500.0f;
	m_titleHeight = 120.0f;
	m_TitlePos = SimpleMath::Vector2(150.0f, 100.0f);

	// α値の設定(初期化)
	m_colorAlpha = 1.0f;

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

	// カメラの更新
	mp_camera->Update(timer, SimpleMath::Vector3(0.0f,0.0f,0.0f), 0.0f, SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	
	// マウスの更新
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
	InputManager::SingletonGetInstance().Update();

	// 右クリックでシーン遷移開始
	if (/*InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED*/
		InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space))
	{
		m_toStageSelectMoveOnChecker = true;
		adx2le->Play(1);
	}
	if (m_toStageSelectMoveOnChecker)
	{
		m_colorAlpha -= 0.01f;
		if (m_colorAlpha < 0.0f) m_colorAlpha = 0.0f;
	}

	// シーン遷移
	if (m_toStageSelectMoveOnChecker && m_colorAlpha <= 0.0f)
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

	// タイトルの描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());
	
	RECT rectTiteBG = { 0, 0, 800, 600 };
	RECT rectTite = { 0, 0, int(m_titleWidth), int(m_titleHeight) };

	//mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectTiteBG, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	mp_sprite->Draw(mp_textureTitle.Get(), m_TitlePos, &rectTite, SimpleMath::Vector4( 1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	
	mp_sprite->End();
}
