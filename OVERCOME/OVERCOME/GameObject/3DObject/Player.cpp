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

#include "../../Utility/InputManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

#include "../../GameObject/3DObject/GameEnemyManager.h"

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
const int Player::SPAWNTIME = 600;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
Player::Player()
	: m_pos(0.0f, 2.0f, 0.0f), m_vel(0.0f, 0.0f, 0.0f), m_dir(0.0f, 0.0f, 0.0f),
	  m_height(0.0f), m_jumpForce(0.0f), m_gravity(0.0f), m_posTmp(0.0f, 0.0f, 0.0f),
	  m_playStartFlag(false), m_playStartTime(0),
	  m_spawnFlag(false), m_spawnElapsedTime(0),
	  m_passedRoadPos(0.0f, 0.0f), m_passingRoadPos(0.0f, 0.0f), m_nextPos(0.0f, 0.0f), m_velFlag(false),
	  m_world(SimpleMath::Matrix::Identity),
	  mp_bulletManager(nullptr), mp_gameRoad(nullptr),
	  mp_startGuide(nullptr),
	  m_posCountUI(0.0f, 0.0f), m_textureCount(nullptr),
	  m_texturePointer(nullptr), m_textureDengerous(nullptr)
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
	m_height       = 1.75f;                                       // プレイヤー自身の高さ
	m_jumpForce    = 0.0f;                                        // ジャンプ力
	m_gravity      = 0.1f;                                        // 重力

	m_world = SimpleMath::Matrix::Identity;                       // ワールド行列

	// 他オブジェクトの初期化
	//mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
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

	// スタート案内オブジェクトの生成
	mp_startGuide = std::make_unique<Obj2D>();
	mp_startGuide->Create(L"Resources\\Images\\Play\\clicktocenter.png", nullptr);
	mp_startGuide->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 450.0f, 50.0f, 1.0f, 1.0f);
	if (isFulleScreen)
	{
		mp_startGuide->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startGuide->GetWidth()*0.5f),
							 (activeWndRect.bottom - activeWndRect.top) - (mp_startGuide->GetHeight()*2.0f)));
	}
	else
	{
		mp_startGuide->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startGuide->GetWidth()*0.5f),
							 (activeWndRect.bottom - (activeWndRect.top + titlebarHeight)) - (mp_startGuide->GetHeight()*5.0f)));
	}
	
	mp_startGuide->SetRect(0.0f, 0.0f, mp_startGuide->GetWidth(), mp_startGuide->GetHeight());

	// カウント数字の設定
	m_posCountUI = SimpleMath::Vector2(360.0f, 260.0f);
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_len.png", nullptr, m_textureCount.GetAddressOf());
	
	// ポインターの設定
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\pointer.png", nullptr, m_texturePointer.GetAddressOf());

	// 危険サインのテクスチャ読み込み
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\dangerous_signV.png", nullptr, m_textureDengerous.GetAddressOf());
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool Player::Update(DX::StepTimer const & timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir)
{
	// カメラの更新
	//mp_gameCamera->Update(timer, m_pos, m_height, m_dir);

	// マウスの更新
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// スタートするまでは初期位置で固定
	if(!isPlayFlag)
	{
		// スタート位置で固定
		if (mp_gameRoad->GetPosType(mp_gameRoad->START).x != m_pos.x ||
			mp_gameRoad->GetPosType(mp_gameRoad->START).y != m_pos.z)
		{
			m_pos.x = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.x;
			m_pos.z = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.z;
		}

		const int OFFSETNUM = 8;
		// 特定のマスの周囲八マス分の相対座標
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },		   { 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		// 通過済みの道路を記憶
		m_passedRoadPos = SimpleMath::Vector2(mp_gameRoad->GetPosType(mp_gameRoad->START).x,
											  mp_gameRoad->GetPosType(mp_gameRoad->START).y);

		// 次の行き先を決定(周囲八マスを調べる)
		float distTmp = 0.0f;
		for (int k = 0; k < OFFSETNUM; k++)
		{
			// マス目上の位置座標
			SimpleMath::Vector2 comparePos = SimpleMath::Vector2(m_passedRoadPos.x, m_passedRoadPos.y);
			comparePos += OFFSET[k];

			// 比べるものがないため仮決定
			if (k == 0)
			{
				// 次の行き先(マス目上の座標)
				m_nextPos = comparePos;

				// 次の行き先(仮)と今の位置の距離を求める
				distTmp = (mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x) +
						  (mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z);
			}
			// 最初に調べる座標でもなく、道が存在しているところなら
			else if (k != 0 && mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).roadType != (int)mp_gameRoad->NONE)
			{
				// 次の行き先と今の位置の距離を求める
				float dist = (mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.x - m_pos.x) +
							 (mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.z - m_pos.z);

				// distのほうが小さい(ここで比べたところのほうが近い)なら
				if (dist < distTmp)
				{
					// 既に通過したところじゃなければ次の移動先に決定
					if (m_passedRoadPos != comparePos) m_nextPos = comparePos;

					distTmp = dist;
				}
			}
		}
	}
	// マウスでクリックされてからの処理
	else
	{
		// マウスで中心をクリックしてからカウントダウン
		if (!m_playStartFlag)
		{
			m_playStartTime++;

			// 3秒経過でゲーム開始
			if (m_playStartTime > 180)
			{
				// カウントダウン用タイムリセット
				m_playStartTime = 0;
				// ゲーム開始フラグを立てる
				m_playStartFlag = true;
				// 移動開始
				m_velFlag = true;
				// マウスカーソルの非表示
				ShowCursor(FALSE);
			}
		}
	}

	// ゲームが開始されてからの処理
	if (m_playStartFlag)
	{
		// 弾の更新
		mp_bulletManager->Update(timer, m_pos, cameraDir);

		// プレイヤー移動(ベクトル)
		const int OFFSETNUM = 8;
		// 特定のマスの周囲八マス分の相対座標
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },		   { 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		SimpleMath::Vector2 nowPos = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);
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
				if (m_vel.x < 0.00f)      m_passedRoadPos.x = m_nextPos.x + 1;
				else if (m_vel.x > 0.00f) m_passedRoadPos.x = m_nextPos.x - 1;
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

			// 次の位置に着いたためnextからnowに変えておく(わかりやすくするため)
			SimpleMath::Vector2 now = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);

			// 次の行き先を決定
			float distTmp = 0.0f;
			for (int k = 0; k < OFFSETNUM; k++)
			{
				SimpleMath::Vector2 pos = nowPos;
				// 通過中の道路(ID)を記憶
				m_passingRoadPos = nowPos;
				// マス目上の位置座標
				pos += OFFSET[k];

				// 比べるものがないため設定しておく
				if (distTmp == 0.0f)
				{
					// 仮の行き先登録
					distTmp = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
							  (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);
				}

				// 道が存在しているところなら
				if (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).roadType != mp_gameRoad->NONE)
				{
					// 調べている行き先と今の位置の距離を求める
					float dist = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
								 (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);

					// distのほうが小さい(ここで比べたところのほうが近い)なら
					if (dist <= distTmp)
					{
						// 移動してきたマスの情報を取得
						int pX = (int)m_passedRoadPos.x;
						int pY = (int)m_passedRoadPos.y;
						SimpleMath::Vector2 p = SimpleMath::Vector2((float)pX, (float)pY);

						// 既に通過したところでなければ次の移動先に決定
						if (p != pos && nowPos != pos)
						{
							// 次の行き先決定
							m_nextPos = pos;
							// 最短距離更新、次はこれと比べる
							distTmp = dist;
						}
					}
				}
			}

			//if (mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).roadType != mp_gameRoad->NONE)
			//{
			//	// 道のIDに設定していないところでなければ移動を開始する
			//	m_velFlag = false;
			//}

			// 到着した道が襲撃ポイントだったら
			if (mp_gameRoad->GetRoadObject((int)m_passingRoadPos.y, (int)m_passingRoadPos.x).roadType == 3)
			{
				// 移動をやめる
				/*m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
				m_velFlag = false;*/
				m_spawnFlag = true;

				// カウント
				/*m_spawnElapsedTime++;

				// 一定数を超えたら
				if (m_spawnElapsedTime > SPAWNTIME)
				{
					// カウントリセット
					m_spawnElapsedTime = 0;
					// 移動を開始する
					m_velFlag = true;
				}*/
			}
		}
		if (m_spawnFlag)
		{
			// 移動をやめる
			m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			m_velFlag = false;

			// カウント
			m_spawnElapsedTime++;

			// 一定数を超えたら
			if (m_spawnElapsedTime > SPAWNTIME)
			{
				// カウントリセット
				m_spawnElapsedTime = 0;
				// 移動を開始する
				m_velFlag = true;
				m_spawnFlag = false;
			}
		}
		// 移動可能だったら
		if(m_velFlag)
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
	}

	// 移動前の座標を記憶
	m_posTmp = m_pos;
	// プレイヤー移動(座標)
	m_pos += m_vel;
	// 移動後の座標との偏差から移動方向を算出
	m_dir = m_pos - m_posTmp;
	//m_dir.Normalize();

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

	// スタートカウントの描画
	if (m_playStartTime > 0 && !m_playStartFlag)
	{
		// 切り取る場所を設定
		RECT rect;
		rect.top = LONG(0.0f);
		rect.left = LONG((2 * COUNTUISIZE) - (m_playStartTime / 60 * COUNTUISIZE));
		rect.right = LONG(2 * COUNTUISIZE) - (m_playStartTime / 60 * COUNTUISIZE) + COUNTUISIZE;
		rect.bottom = LONG(COUNTUISIZE);

		DrawManager::SingletonGetInstance().DrawRect(m_textureCount.Get(), m_posCountUI, &rect);
	}

	// スタート案内の表示
	if (!m_playStartFlag)
	{
		mp_startGuide->Render();
	}
		
	// ポインターの描画
	if (m_playStartFlag)
	{
		DrawManager::SingletonGetInstance().Draw(m_texturePointer.Get(), SimpleMath::Vector2(350.0f, 330.0f));
	}

	// 敵がいたら危険サイン表示
	if (dangerDir == GameEnemyManager::DANGERDIRECTION::DIR_RIGHT)
	{
		DrawManager::SingletonGetInstance().Draw(m_textureDengerous.Get(), SimpleMath::Vector2(700.0f, 50.0f));
	}
	else if (dangerDir == GameEnemyManager::DANGERDIRECTION::DIR_LEFT)
	{
		DrawManager::SingletonGetInstance().Draw(m_textureDengerous.Get(), SimpleMath::Vector2(50.0f, 50.0f));
	}
}

/// <summary>
/// 削除処理
/// </summary>
void Player::Depose()
{
}

/// <summary>
/// プレイヤーの情報を返す
/// </summary>
/// <returns>プレイヤー情報</returns>
Player* Player::GetPlayer()
{
	return this;
}
