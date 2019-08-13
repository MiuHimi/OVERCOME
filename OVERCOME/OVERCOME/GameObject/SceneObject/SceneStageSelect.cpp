//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const int SceneStageSelect::STAGE_ICON_SIZE = 80;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toPlayMoveOnChecker(false),
	  m_selectedStage(0),
	  m_colorAlpha(0.0f),
	  mp_textureBackground(nullptr),
	  mp_sprite(nullptr),
	  mp_matrixManager(nullptr)
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

	// アイコン衝突判定の初期化
	for (int i = 0; i < stage::NUM; i++)
	{
		m_collideStageIcon[i].pos = SimpleMath::Vector2(float((i * 30) + 315 + i * STAGE_ICON_SIZE), 360.0f);
		m_collideStageIcon[i].width = float(STAGE_ICON_SIZE);
		m_collideStageIcon[i].height = float(STAGE_ICON_SIZE);
	}

	// テクスチャのロード
	for (int i = 0; i < stage::NUM; i++)
	{
		CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\StageSelect\\stage_len.png", nullptr, mp_textureStageIcon[i].GetAddressOf());
	}
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\StageSelect\\stageselect_background_image.png", nullptr, mp_textureBackground.GetAddressOf());

	// スプライトバッチの初期化
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// α値の設定(初期化)
	m_colorAlpha = 1.0f;

	// 行列管理変数の初期化
	mp_matrixManager = new MatrixManager();

	// ビュー行列の作成
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

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
	
	// サウンド再生
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"SceneStageSelect.acb", L"SceneStageSelect.awb");
	adx2le->Play(0);
}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneStageSelect::Finalize()
{
	// 行列管理変数の削除
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

	// サウンドの停止
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();
}

/// <summary>
/// ステージ選択シーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	InputManager::SingletonGetInstance().Update();
	// マウスの更新
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// サウンドの更新
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// ステージの選択
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		SimpleMath::Vector2 mousePos = SimpleMath::Vector2(float(InputManager::SingletonGetInstance().GetMousePosX()), float(InputManager::SingletonGetInstance().GetMousePosY()));
		for (int i = 0; i < stage::NUM; i++)
		{
			// マウスとアイコンの衝突判定
			if (mousePos.x > m_collideStageIcon[i].pos.x && mousePos.x < m_collideStageIcon[i].pos.x + float(STAGE_ICON_SIZE) &&
				mousePos.y > m_collideStageIcon[i].pos.y && mousePos.y < m_collideStageIcon[i].pos.y + float(STAGE_ICON_SIZE))
			{
				// 選択されたステージ番号を記憶
				m_selectedStage = i+1;
				// プレイステージを決定
				//SceneManager::SetStageNum(m_selectedStage);
				// シーン遷移発生
				m_toPlayMoveOnChecker = true;
				// サウンド再生
				adx2le->Play(1);
				break;
			}
		}
	}

	// シーン遷移開始
	if (m_toPlayMoveOnChecker)
	{
		m_colorAlpha -= 0.01f;
		if (m_colorAlpha < 0.0f) m_colorAlpha = 0.0f;
	}

	// シーン遷移
	if (m_toPlayMoveOnChecker && m_colorAlpha <= 0.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneStageSelect::Render()
{
	// ステージセレクト画面の描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// 背景
	RECT rectBG = { 0, 0, 800, 600};
	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectBG, SimpleMath::Vector4( 1.0f, 1.0f, 1.0f , m_colorAlpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// アイコン
	for (int i = 0; i < stage::NUM; i++)
	{
		RECT rect = { i*STAGE_ICON_SIZE, 0, i*STAGE_ICON_SIZE+STAGE_ICON_SIZE, STAGE_ICON_SIZE };
		mp_sprite->Draw(mp_textureStageIcon[i].Get(), m_collideStageIcon[i].pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
}

