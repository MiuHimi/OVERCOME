//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "Player.h"
//#include "GameRoad.h"

#include <math.h>

#include "../../Utility/InputManager.h"
#include "../../Utility/MathManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

#include "../../GameObject/3DObject/GameEnemyManager.h"


// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const int Player::DAMAGE_EFFECT_COUNT = 4;
const int Player::FINISH_DAMAGE_EFFECT_COUNT = 20;
const int Player::START_COUNT_TIME = 180;
const int Player::COUNT_UI_SIZE = 80;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
Player::Player()
	: m_pos(SimpleMath::Vector3::Zero), m_vel(SimpleMath::Vector3::Zero), m_dir(SimpleMath::Vector3::Zero),
	  m_height(0.0f), m_posTmp(SimpleMath::Vector3::Zero),
	  m_hp(0), m_hpBasePos{0.0f}, m_damageCount(0), m_isDamaged(false),
	  m_playStartFlag(false), m_moveStartCountDown(0),
	  m_passedRoadPos(SimpleMath::Vector2::Zero), m_passingRoadPos(SimpleMath::Vector2::Zero), m_nextPos(SimpleMath::Vector2::Zero), m_velFlag(false),
	  m_world(SimpleMath::Matrix::Identity),
	  mp_bulletManager(nullptr), mp_gameRoad(nullptr),
	  m_isFullScreen(false),
	  mp_startCount(nullptr), mp_shootPointer(nullptr), mp_dengerousSign(nullptr),
	  mp_hp(nullptr), mp_hpFrame(nullptr), mp_damageEffect(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	delete mp_bulletManager;
	mp_bulletManager = nullptr;
}

/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize()
{
	// 各変数初期化
	m_height	= 1.75f;										// プレイヤー自身の高さ
	m_hp		= 10;											// 体力

	m_moveStartCountDown = START_COUNT_TIME;					// 動き出せるまでのカウントダウン用

	m_world = SimpleMath::Matrix::Identity;						// ワールド行列

	// 他オブジェクトの初期化
	mp_bulletManager = new GameBulletManager();
	mp_bulletManager->Initialize();
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	mp_gameRoad->Create();
}
/// <summary>
/// 生成処理
/// </summary>
void Player::Create(const bool isFulleScreen)
{
	// エフェクトファクトリー
	//EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	//fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	//m_modelPlayer = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\player.cmo", fx);

	// プレイヤーの作成
	//SetModel(m_modelPlayer.get());

	mp_bulletManager->Create();

	// スクリーンモード取得
	m_isFullScreen = isFulleScreen;

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

	// スタートカウントダウンオブジェクトの生成
	mp_startCount = std::make_unique<Obj2D>();
	mp_startCount->Create(L"Resources\\Images\\ScoreCount\\count_length.png", nullptr);
	mp_startCount->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(COUNT_UI_SIZE), float(COUNT_UI_SIZE), 1.0f, 1.0f);
	if (isFulleScreen)
	{
		mp_startCount->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startCount->GetWidth()*0.5f),
												  (windowHeight*0.5f) - (mp_startCount->GetHeight()*0.5f)));
	}
	else
	{
		mp_startCount->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startCount->GetWidth()*0.5f),
												  ((activeWndRect.bottom - (activeWndRect.top + titlebarHeight))*0.5f) - (mp_startCount->GetHeight()*0.5f)));
	}
	mp_startCount->SetRect(0.0f, 0.0f, mp_startCount->GetWidth(), mp_startCount->GetHeight());

	// 発射ポインターオブジェクトの生成
	mp_shootPointer = std::make_unique<Obj2D>();
	mp_shootPointer->Create(L"Resources\\Images\\Play\\pointer.png", nullptr);
	mp_shootPointer->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 100.0f, 100.0f, 1.0f, 1.0f);
	if (isFulleScreen)
	{
		mp_shootPointer->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_shootPointer->GetWidth()*0.5f),
													(windowHeight*0.5f) + (windowHeight / 20.0f)));
	}
	else
	{
		mp_shootPointer->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_shootPointer->GetWidth()*0.5f),
													(windowHeight*0.5f) + titlebarHeight + (windowHeight / 20.0f)));
	}
	mp_shootPointer->SetRect(0.0f, 0.0f, mp_shootPointer->GetWidth(), mp_shootPointer->GetHeight());

	// 危険サインオブジェクトの生成
	mp_dengerousSign = std::make_unique<Obj2D>();
	mp_dengerousSign->Create(L"Resources\\Images\\Play\\dangerous_signV.png", nullptr);
	mp_dengerousSign->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 50.0f, 500.0f, 1.0f, 1.0f);
	mp_dengerousSign->SetRect(0.0f, 0.0f, mp_dengerousSign->GetWidth(), mp_dengerousSign->GetHeight());

	// ダメージ効果オブジェクトの生成
	mp_damageEffect = std::make_unique<Obj2D>();
	mp_damageEffect->Create(L"Resources\\Images\\Play\\damaged.png", nullptr);
	mp_damageEffect->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_damageEffect->SetRect(0.0f, 0.0f, mp_damageEffect->GetWidth(), mp_damageEffect->GetHeight());

	// 体力オブジェクトの生成
	mp_hp = std::make_unique<Obj2D>();
	mp_hp->Create(L"Resources\\Images\\Play\\player_hp.png", nullptr);
	mp_hp->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 100.0f, 25.0f, 1.0f, 1.0f);
	mp_hp->SetRect(0.0f, 0.0f, mp_hp->GetWidth(), mp_hp->GetHeight());
	mp_hp->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_hp->GetWidth()*5.0f), windowHeight - (mp_hp->GetHeight()*2.0f)));

	// 体力の枠オブジェクトの生成
	mp_hpFrame = std::make_unique<Obj2D>();
	mp_hpFrame->Create(L"Resources\\Images\\Play\\player_hp_frame.png", nullptr);
	mp_hpFrame->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 1010.0f, 45.0f, 1.0f, 1.0f);
	mp_hpFrame->SetRect(0.0f, 0.0f, mp_hpFrame->GetWidth(), mp_hpFrame->GetHeight());
	mp_hpFrame->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_hpFrame->GetWidth()*0.5f), windowHeight - (mp_hpFrame->GetHeight() + 18.0f)));

	// 基準位置設定
	m_hpBasePos = mp_hp->GetPos();
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool Player::Update(DX::StepTimer const & timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir, SimpleMath::Vector3& correctPos, const bool isAssaultedState)
{
	// マウスの更新
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// 外部の当たり判定等で矯正された位置に設定
	m_pos.y = correctPos.y;

	// 進みだす前の処理
	bool isGoOn = BeforeGoOnPlayer(isPlayFlag);

	// 進み始めた後の処理
	AfterGoOnPlayer(isGoOn, timer, cameraDir, isAssaultedState);

	// ダメージの更新
	UpdateDamage();

	// HPの更新
	UpdateHP();

	// 移動前の座標を記憶
	m_posTmp = m_pos;
	// プレイヤー移動(座標)
	m_pos += m_vel;
	// 移動後の座標との偏差から移動方向を算出
	m_dir = m_pos - m_posTmp;

	// 衝突判定用の仮想オブジェクト生成
	Collision::Box box;
	box.c = SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // 境界箱の中心
	box.r = SimpleMath::Vector3(1.0f, m_height / 2.0f, 1.0f);                        // 各半径
	SetCollision(box);

	return true;
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Render(MatrixManager* matrixManager, GameEnemyManager::DANGERDIRECTION dangerDir)
{
	// 弾の描画
	mp_bulletManager->Render(matrixManager);

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

	// スタートカウントの描画
	if (m_moveStartCountDown < START_COUNT_TIME && !m_playStartFlag)
	{
		mp_startCount->SetRect(float(((m_moveStartCountDown / 60)+1) * COUNT_UI_SIZE),
							   float(0.0f),
							   float(((m_moveStartCountDown / 60)+1) * COUNT_UI_SIZE) + COUNT_UI_SIZE,
							   float(COUNT_UI_SIZE));
		mp_startCount->Render();
	}
	
	// 発射ポインターの描画
	if (m_playStartFlag)
	{
		mp_shootPointer->Render();
	}

	// 至近距離の敵が画面内にいなかったら危険サイン表示
	if (dangerDir == GameEnemyManager::DANGERDIRECTION::DIR_LEFT)
	{
		if (m_isFullScreen)
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(float(activeWndRect.left) + mp_dengerousSign->GetWidth(),
														 (windowHeight*0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		else
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(mp_dengerousSign->GetWidth(),
														 ((windowHeight - titlebarHeight) * 0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		mp_dengerousSign->Render();
	}
	else if (dangerDir == GameEnemyManager::DANGERDIRECTION::DIR_RIGHT)
	{
		if (m_isFullScreen)
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(float(activeWndRect.right) - (mp_dengerousSign->GetWidth()*2.0f),
														 (windowHeight*0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		else
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(float(activeWndRect.right - activeWndRect.left) - (mp_dengerousSign->GetWidth()*2.0f),
														 ((windowHeight - titlebarHeight) * 0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		mp_dengerousSign->Render();
	}

	// HP表示
	if (m_playStartFlag)
	{
		mp_hpFrame->Render();
		for (int i = 0; i < m_hp; i++)
		{
			mp_hp->SetPos(SimpleMath::Vector2(m_hpBasePos.x + float(mp_hp->GetWidth() * i), m_hpBasePos.y));
			mp_hp->Render();
		}
	}
	
	// ダメージ演出表示
	mp_damageEffect->RenderAlpha();
}

/// <summary>
/// 削除処理
/// </summary>
void Player::Depose()
{
}

/// <summary>
/// 進みだす前の処理
/// </summary>
/// <param name="isClickCenter">進み出すためのフラグ</param>
/// <returns>true=プレイヤー進行可能、false=プレイヤー進行不可</returns>
bool Player::BeforeGoOnPlayer(const bool isClickCenter)
{
	// スタートするまでは初期位置で固定
	if (!isClickCenter)
	{
		// スタート位置で固定
		if (mp_gameRoad->GetPosType(mp_gameRoad->START).x != m_pos.x ||
			mp_gameRoad->GetPosType(mp_gameRoad->START).y != m_pos.z)
		{
			m_pos.x = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.x;
			m_pos.z = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.z;
		}

		// 通過済みの道路を記憶
		m_passedRoadPos = SimpleMath::Vector2(mp_gameRoad->GetPosType(mp_gameRoad->START).x,
			mp_gameRoad->GetPosType(mp_gameRoad->START).y);

		// 次の行き先を決定
		SearchNextRoad(m_passedRoadPos);

		// プレイヤー進行不可
		m_playStartFlag = false;
	}
	// マウスでクリックされてからの処理
	else
	{
		// マウスで中心をクリックしてからカウントダウン
		if (!m_playStartFlag)
		{
			m_moveStartCountDown--;

			// 3秒経過でゲーム開始
			if (m_moveStartCountDown <= 0)
			{
				// カウントダウン用タイムリセット
				m_moveStartCountDown = START_COUNT_TIME;
				// プレイヤー進行フラグを立てる
				m_playStartFlag = true;
				// 移動開始
				m_velFlag = true;
			}
		}
	}

	return m_playStartFlag;
}

/// <summary>
/// 進み始めた後の処理
/// </summary>
/// <param name="isGoOn">プレイヤーが初回の進行を始めたかどうか</param>
/// <param name="timer">更新時間</param>
/// <param name="cameraDir">カメラの向き</param>
void Player::AfterGoOnPlayer(const bool isGoOn, DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& cameraDir, const bool isAssaultedState)
{
	// プレイヤーが進行を開始したら以降の処理を行う
	if (!isGoOn)return;

	//マウスカーソルの非表示
	InputManager::SingletonGetInstance().SetVisibleMouseCursor(false);

	// 弾の更新
	SimpleMath::Vector3 eyePos = SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height * 0.5f), m_pos.z);
	mp_bulletManager->Update(timer, eyePos, cameraDir);

	// 到着し、次の行動を選択
	bool isMove = ArrivalNextRoad();

	// フラグに応じて進行するかどうかを決定する
	SetPlayerMove(isMove, isAssaultedState);
}

/// <summary>
/// 目指す道に到着
/// </summary>
/// <returns>true=そのまま進行可能、false=進行停止</returns>
bool Player::ArrivalNextRoad()
{
	// 次の行き先に決定した位置と現在座標が限りなく近い位置にいたら
	if (abs(m_pos.x - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x) < 0.01f &&
		abs(m_pos.z - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z) < 0.01f)
	{
		// 次の位置に現在座標をずらす
		m_pos.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x;
		m_pos.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z;

		// どの方向に進んでいたか比べる
		if (abs(m_vel.x) > abs(m_vel.z))
		{
			// X軸方向に進んでいた場合
			// 今通った道を通過済みにする
			if (m_vel.x < 0.0f)      m_passedRoadPos.x = m_nextPos.x + 1;
			else if (m_vel.x > 0.0f) m_passedRoadPos.x = m_nextPos.x - 1;
			// 同時に軸違いのほうも記憶しておく
			m_passedRoadPos.y = m_nextPos.y;
		}
		else if (abs(m_vel.x) < abs(m_vel.z))
		{
			// Z軸方向に進んでいた場合
			// 今通った道を通過済みにする
			if (m_vel.z < 0.00f)      m_passedRoadPos.y = m_nextPos.y + 1;
			else if (m_vel.z > 0.00f) m_passedRoadPos.y = m_nextPos.y - 1;
			// 同時に軸違いのほうも記憶しておく
			m_passedRoadPos.x = m_nextPos.x;
		}

		// 次の位置に着いたため通過中の道路IDを更新
		m_passingRoadPos = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);

		// 次の行き先を決定
		SearchNextRoad(m_passingRoadPos);

		// 到着した道が襲撃ポイントだったら
		if (mp_gameRoad->GetRoadObject((int)m_passingRoadPos.y, (int)m_passingRoadPos.x).roadType == (int)GameRoad::RoadType::ASSAULT)
		{
			m_velFlag = false;
		}
		else
		{
			// そうでなければ進行する
			m_velFlag = true;
		}
	}

	return m_velFlag;
}

/// <summary>
/// フラグに応じて進行するかどうかを決定する
/// </summary>
/// <param name="isMove">進行是非フラグ</param>
/// <param name="isAssaultedState">敵の攻撃状況フラグ</param>
void Player::SetPlayerMove(bool isMove, const bool isAssaultedState)
{
	// 移動可能だったら
	if (isMove)
	{
		// 次の行き先に近づくまで差分で移動し続ける
		m_vel.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x;
		m_vel.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z;

		m_vel.Normalize();
		m_vel.x /= 10.0f;
		m_vel.y = 0.0f;
		m_vel.z /= 10.0f;

		m_velFlag = true;
	}
	else
	{
		// 移動をやめる
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		m_velFlag = false;

		// 敵の襲撃が止んだら
		if (isAssaultedState)
		{
			// 移動を開始する
			m_velFlag = true;
		}
	}
}

/// <summary>
/// 次の行き先を決定(周囲八マスを調べる)
/// </summary>
void Player::SearchNextRoad(const SimpleMath::Vector2 nowPos)
{
	const int OFFSETNUM = 8;
	// プレイヤーの周囲8マスの相対座標
	SimpleMath::Vector2 OFFSET[OFFSETNUM] =
	{
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 },		   { 1, 0 },
		{ -1, 1 },{ 0, 1 },{ 1, 1 }
	};

	// 次の行き先を決定(周囲八マスを調べる)
	float distTmp = 0.0f;
	for (int k = 0; k < OFFSETNUM; k++)
	{
		// 現在地を基準に
		SimpleMath::Vector2 comparePos = nowPos;
		// 周囲八マスを調べる
		comparePos += OFFSET[k];

		// 比べるものがないため仮決定
		if (k == 0)
		{
			// 次の行き先(マス目上の座標)仮決定
			m_nextPos = comparePos;

			// 次の行き先(仮)と今の位置の距離を求める
			SimpleMath::Vector3 roadPos = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos;
			distTmp = MathManager::SingletonGetInstance().GetDistancePoints2D(m_pos.x, m_pos.z, roadPos.x, roadPos.z);
		}
		// 最初に調べる座標で無ければ
		else
		{
			// 道が存在しているところなら以下の処理を行う
			if ((mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).roadType == (int)mp_gameRoad->NONE)) continue;

			// 次の行き先(仮)と今の位置の距離を求める
			SimpleMath::Vector3 roadPos = mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos;
			float dist = MathManager::SingletonGetInstance().GetDistancePoints2D(m_pos.x, m_pos.z, roadPos.x, roadPos.z);

			// distのほうが小さい(ここで比べたところのほうが近い)、
			// 既に通過したところでなければ
			if (dist < distTmp &&
				m_passedRoadPos != comparePos)
			{
				// 次の移動先に決定
				m_nextPos = comparePos;
				// 最短距離更新
				distTmp = dist;
			}
		}
	}
}

/// <summary>
/// ダメージの更新
/// </summary>
void Player::UpdateDamage()
{
	// 敵と衝突したら
	if (m_isDamaged)
	{
		// カウントを進める
		m_damageCount++;
		// 表示切替
		if (m_damageCount % DAMAGE_EFFECT_COUNT == 0)
		{
			if (mp_damageEffect->GetAlpha() == 0.0f)
			{
				mp_damageEffect->SetAlpha(1.0f);
			}
			else if (mp_damageEffect->GetAlpha() == 1.0f)
			{
				mp_damageEffect->SetAlpha(0.0f);
			}
		}
		// 演出終了
		if (m_damageCount > FINISH_DAMAGE_EFFECT_COUNT)
		{
			mp_damageEffect->SetAlpha(0.0f);
			m_damageCount = 0;
			m_isDamaged = false;
		}
	}
	else
	{
		m_damageCount = 0;
	}

}

/// <summary>
/// HPの更新
/// </summary>
void Player::UpdateHP()
{
	// HPバー
	if (m_hp <= PLAYER_STATE::MORIBUND)
	{
		mp_hp->SetRect(mp_hp->GetWidth()*2.0f, 0.0f, (mp_hp->GetWidth()*2.0f) + mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	else if (m_hp <= PLAYER_STATE::WOUND)
	{
		mp_hp->SetRect(mp_hp->GetWidth(), 0.0f, mp_hp->GetWidth() + mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	else if (m_hp <= PLAYER_STATE::NORMAL)
	{
		mp_hp->SetRect(0.0f, 0.0f, mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	// HPが0以下になったら
	if (m_hp <= 0)
	{
		// 移動を止める
		m_hp = 0;
		m_vel = SimpleMath::Vector3::Zero;
	}
}
