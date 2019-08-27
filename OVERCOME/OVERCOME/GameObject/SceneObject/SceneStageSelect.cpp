//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2019/08/27
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
	  mp_background(nullptr), mp_stageSelectImage(nullptr),
	  mp_stageNum{nullptr}, mp_stageFlame{nullptr},
	  mp_fade(nullptr),
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

	// 背景の生成
	mp_background = std::make_unique<Obj2D>();
	mp_background->Create(L"Resources\\Images\\gray.png", nullptr);
	mp_background->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_background->SetRect(0.0f, 0.0f, mp_background->GetWidth(), mp_background->GetHeight());

	// StageSelectの生成
	mp_stageSelectImage = std::make_unique<Obj2D>();
	mp_stageSelectImage->Create(L"Resources\\Images\\StageSelect\\stageselect_image.png", nullptr); // ホバースプライトなし
	mp_stageSelectImage->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 525.0f, 50.0f, 1.0f, 1.0f);
	mp_stageSelectImage->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_stageSelectImage->GetWidth() * 0.5f),
													(mp_stageSelectImage->GetHeight() * 2.0f)));
	mp_stageSelectImage->SetRect(0.0f, 0.0f, mp_stageSelectImage->GetWidth(), mp_stageSelectImage->GetHeight());

	// ステージ番号の生成
	for (int i = 0; i < STAGE::NUM; i++)
	{
		mp_stageNum[i] = std::make_unique<Obj2D>();
		mp_stageNum[i]->Create(L"Resources\\Images\\StageSelect\\stageselect_num_len.png", L"Resources\\Images\\StageSelect\\stageselect_num_len_hover.png");

		mp_stageNum[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(STAGE_ICON_SIZE), float(STAGE_ICON_SIZE), 1.0f, 1.0f);
		mp_stageNum[i]->SetPos(SimpleMath::Vector2(float(((windowWidth*0.5f) - STAGE_ICON_SIZE*1.5f) + (i*(STAGE_ICON_SIZE*1.5f))), 
												   (windowHeight * 0.5f) - (mp_stageNum[i]->GetHeight() * 0.5f)));
		mp_stageNum[i]->SetRect(float((i+1)*STAGE_ICON_SIZE), 0.0f, float((i + 1)*STAGE_ICON_SIZE + STAGE_ICON_SIZE), float(STAGE_ICON_SIZE));
	}
	// ステージ番号フレームの生成
	for (int i = 0; i < STAGE::NUM; i++)
	{
		mp_stageFlame[i] = std::make_unique<Obj2D>();
		mp_stageFlame[i]->Create(L"Resources\\Images\\StageSelect\\stageselect_flame.png", L"Resources\\Images\\StageSelect\\stageselect_flame_hover.png");

		mp_stageFlame[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(STAGE_ICON_SIZE), float(STAGE_ICON_SIZE), 1.0f, 1.0f);
		mp_stageFlame[i]->SetPos(SimpleMath::Vector2(float(((windowWidth*0.5f) - STAGE_ICON_SIZE*1.5f) + (i*(STAGE_ICON_SIZE*1.5f))), 
													 (windowHeight * 0.5f) - (mp_stageFlame[i]->GetHeight() * 0.5f)));
		mp_stageFlame[i]->SetRect(0.0f, 0.0f, float(STAGE_ICON_SIZE), float(STAGE_ICON_SIZE));
	}
	
	// フェード画像の生成
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());
	mp_fade->SetAlpha(0.0f);

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

	// ステージ番号とマウスカーソルの衝突判定
	SimpleMath::Vector2 mousePos = SimpleMath::Vector2((float)InputManager::SingletonGetInstance().GetMousePosX(),
		(float)InputManager::SingletonGetInstance().GetMousePosY());
	for (int i = 0; i < STAGE::NUM; i++)
	{
		SimpleMath::Vector2 btnPos = mp_stageNum[i]->GetPos();
		float btnWidth = mp_stageNum[i]->GetWidth();
		float btnHeight = mp_stageNum[i]->GetHeight();
		// マウスがボタンに接触していたら
		if (mp_stageNum[i]->IsCollideMouse(mousePos, btnPos, btnWidth, btnHeight))
		{
			//	ホバー状態にする
			mp_stageNum[i]->SetHover(true);
			//	同じ場所のオブジェクトのため同様にホバー状態にする
			mp_stageFlame[i]->SetHover(true);
		}
		// 接触していなかったら
		else
		{
			//	ホバー状態にしない
			mp_stageNum[i]->SetHover(false);
			//	同じ場所のオブジェクトのため同様にホバー状態にしない
			mp_stageFlame[i]->SetHover(false);
		}
	}
	
	// ステージの選択
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		for (int i = 0; i < STAGE::NUM; i++)
		{
			// マウスとアイコンが衝突していたら
			if (mp_stageNum[i]->GetHover())
			{
				// 選択されたステージ番号を記憶
				m_selectedStage = i + 1;
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
		for (int i = 0; i < STAGE::NUM; i++)
		{
			if (i == m_selectedStage - 1)
			{
				// 選択されたステージをホバー状態にする
				mp_stageNum[i]->SetHover(true);
				mp_stageFlame[i]->SetHover(true);
			}
			else
			{
				// 選択されていなかったらホバー状態にしない
				mp_stageNum[i]->SetHover(false);
				mp_stageFlame[i]->SetHover(false);
			}
		}
		
		// フェードアウト
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// シーン遷移
	if (m_toPlayMoveOnChecker && mp_fade->GetAlpha() >= 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneStageSelect::Render()
{
	// 背景の表示
	mp_background->Render();
	// StageSelect文の表示
	mp_stageSelectImage->Render();

	// ステージ番号の表示
	for (int i = 0; i < STAGE::NUM; i++)
	{
		mp_stageNum[i]->Render();
		mp_stageFlame[i]->Render();
	}

	// フェード画像の表示
	mp_fade->RenderAlpha();
}

