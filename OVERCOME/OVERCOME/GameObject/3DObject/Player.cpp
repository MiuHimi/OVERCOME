//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "Player.h"
#include "GameRoad.h"

#include "../../Utility/InputManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

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
	  m_restartFlag(false), m_restartTime(0),
	  m_spawnFlag(false), m_spawnElapsedTime(0), m_assaultPoint(0),
	  m_passedRoadPos(0.0f, 0.0f), m_nextPos(0.0f, 0.0f), m_velFlag(false),
	  m_world(SimpleMath::Matrix::Identity),
	  mp_bulletManager(nullptr), mp_gameCamera(nullptr), mp_gameRoad(nullptr),
	  m_posRestartUI(0.0f, 0.0f), m_textureRestart(nullptr),
	  m_posCountUI(0.0f, 0.0f), m_textureCount(nullptr),
	  m_texturePointer(nullptr)
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
	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
	mp_bulletManager = new GameBulletManager();
	mp_bulletManager->Initialize();
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	mp_gameRoad->Create();
	
}
/// <summary>
/// 生成処理
/// </summary>
void Player::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelPlayer = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\player.cmo", fx);

	// プレイヤーの作成
	SetModel(m_modelPlayer.get());

	mp_bulletManager->Create();

	// リスタートUIの設定
	m_posRestartUI = SimpleMath::Vector2(175.0f, 450.0f);
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\clicktocenter.png", nullptr, m_textureRestart.GetAddressOf());

	// カウント数字の設定
	m_posCountUI = SimpleMath::Vector2(360.0f, 260.0f);
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_len.png", nullptr, m_textureCount.GetAddressOf());
	
	// ポインターの設定
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\pointer.png", nullptr, m_texturePointer.GetAddressOf());

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool Player::Update(DX::StepTimer const & timer)
{
	// カメラの更新
	mp_gameCamera->Update(timer,this);

	// マウスの更新
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// マウスが初期位置についてからのカウントダウン
	if (!m_playStartFlag)
	{
		if (mp_gameCamera->GetStartPosMouse())
		{
			m_playStartTime++;
		}

		if (m_playStartTime > 180)
		{
			m_playStartTime = 0;
			m_playStartFlag = true;
			// マウスカーソルの非表示
			ShowCursor(FALSE);
		}
	}
	
	// ゲームスタートしてからの行動
	if (m_playStartFlag)
	{
		// 弾の更新
		mp_bulletManager->Update(timer, m_pos, mp_gameCamera->GetCameraAngle());

		// プレイヤー移動(ベクトル)
		if (mp_gameCamera->GetStartPosMouse())
		{
			const int OFFSETNUM = 8;
			// 特定のマスの周囲八マス分の相対座標
			SimpleMath::Vector2 OFFSET[OFFSETNUM] =
			{
				{ -1,-1 },{ 0,-1 },{ 1,-1 },
				{ -1, 0 },{ 1, 0 },
				{ -1, 1 },{ 0, 1 },{ 1, 1 }
			};

			SimpleMath::Vector2 nowPos = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);
			if (abs(m_pos.x - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x) < 0.01f &&
				abs(m_pos.z - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z) < 0.01f &&
				m_spawnFlag == false)
			{
				m_pos.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x;
				m_pos.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z;

				if (abs(m_vel.x) > abs(m_vel.z))
				{
					if (m_vel.x < 0.00f)
						m_passedRoadPos.x = m_nextPos.x + 1;
					else if (m_vel.x > 0.00f)
						m_passedRoadPos.x = m_nextPos.x - 1;
					m_passedRoadPos.y = m_nextPos.y;
				}
				else if (abs(m_vel.x) < abs(m_vel.z))
				{
					if (m_vel.z < 0.00f)
						m_passedRoadPos.y = m_nextPos.y + 1;
					else if (m_vel.z > 0.00f)
						m_passedRoadPos.y = m_nextPos.y - 1;
					m_passedRoadPos.x = m_nextPos.x;
				}

				SimpleMath::Vector2 now = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);

				// 次の行き先を決定
				float distTmp = 0.0f;
				for (int k = 0; k < OFFSETNUM; k++)
				{
					SimpleMath::Vector2 pos = nowPos;
					pos += OFFSET[k];

					if (distTmp == 0.0f)
					{
						// 仮の行き先登録
						distTmp = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
							(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);
					}

					if (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).roadType != 0)
					{
						float dist = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
							(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);

						if (dist <= distTmp)
						{
							int pX = (int)m_passedRoadPos.x;
							int pY = (int)m_passedRoadPos.y;
							SimpleMath::Vector2 p = SimpleMath::Vector2((float)pX, (float)pY);

							// 既に通過したところじゃなければ次の移動先に決定
							if (p != pos &&
								nowPos != pos)
							{
								m_nextPos = pos;

								distTmp = dist;
							}
						}
					}
				}

				if (mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).roadType == 3 && m_velFlag == true)
				{
					m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
					m_spawnFlag = true;
					m_assaultPoint = mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).rotaAngle;
				}
				if (mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).roadType != 3)
				{
					m_velFlag = false;
				}
			}
			else if (m_spawnFlag)
			{
				m_spawnElapsedTime++;
				if (m_velFlag == true && m_spawnElapsedTime > SPAWNTIME)
				{
					m_spawnElapsedTime = 0;
					m_velFlag = false;
					m_spawnFlag = false;
				}
			}
			else if(!m_velFlag)
			{
				// 次の行き先に近づくまで差分で移動し続ける
				m_vel.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x;
				m_vel.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z;

				m_vel.Normalize();
				m_vel.x /= 10.0f;
				//if (m_vel.x > 1.0f || m_vel.x < -1.0f)m_vel.x = 0.0f;
				m_vel.y = 0.0f;
				m_vel.z /= 10.0f;
				//if (m_vel.z > 1.0f || m_vel.z < -1.0f)m_vel.z = 0.0f;

				m_velFlag = true;
			}
		}
		
		// リスタートの準備
		/*if (m_restartFlag)
		{
			m_restartTime++;
		}
		if (m_restartTime > 180)
		{
			SimpleMath::Vector3 pos = m_pos;

			m_pos.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x;
			m_pos.y = 1.0f;
			m_pos.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z;

			m_vel = pos - m_pos;
			m_vel.Normalize();
			m_vel.x /= 10.0f;
			m_vel.y = 0.0f;
			m_vel.z /= 10.0f;

			m_restartTime = 0;
			m_restartFlag = false;
			m_velFlag = false;
		}*/

	}

	// スタートするまでは初期位置で固定
	if(!mp_gameCamera->GetStartPosMouse())
	{
		const int OFFSETNUM = 8;
		// 特定のマスの周囲八マス分の相対座標
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },{ 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		// 初期位置で固定
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
		float distTmp = 0.0f;
		for (int k = 0; k < OFFSETNUM; k++)
		{
			SimpleMath::Vector2 pos = SimpleMath::Vector2(m_passedRoadPos.x, m_passedRoadPos.y);
			pos += OFFSET[k];

			if (k == 0)
			{
				m_nextPos = pos;

				// 仮の行き先登録
				distTmp = (mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x) +
					(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z);
			}

			if (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).roadType != 0)
			{
				float dist = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
					(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);

				if (dist < distTmp)
				{
					// 既に通過したところじゃなければ次の移動先に決定
					if (m_passedRoadPos != pos) m_nextPos = pos;

					distTmp = dist;
				}
			}
		}
	}
	
	// 移動前の座標を記憶
	m_posTmp = m_pos;
	// プレイヤー移動(座標)
	m_pos += m_vel;
	// 移動後の座標との偏差から移動方向を算出
	m_dir = m_pos - m_posTmp;
	//m_dir.Normalize();

	if (m_vel != SimpleMath::Vector3::Zero)
	{
		m_assaultPoint = 0;
	}

	// ワールド行列の作成
	m_world = SimpleMath::Matrix::CreateTranslation(m_pos);

	// 衝突判定用の仮想オブジェクト生成
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // 境界箱の中心
	box.r = DirectX::SimpleMath::Vector3(1.0f, m_height / 2.0f, 1.0f);                        // 各半径
	SetCollision(box);

	return true;
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Render(MatrixManager* matrixManager)
{
	// プレイヤーの描画
	//m_modelPlayer->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
	//	           m_world, matrixManager->GetView(), matrixManager->GetProjection());

	mp_bulletManager->Render(matrixManager);

	// スタートカウントの描画
	if (!m_playStartFlag)
	{
		if (mp_gameCamera->GetStartPosMouse())
		{
			// 切り取る場所を設定
			RECT rect;
			rect.top = LONG(0.0f);
			rect.left = LONG((2 * COUNTUISIZE) - (m_playStartTime/60 * COUNTUISIZE));
			rect.right = LONG(2 * COUNTUISIZE) - (m_playStartTime / 60 * COUNTUISIZE) + COUNTUISIZE;
			rect.bottom = LONG(COUNTUISIZE);

			DrawManager::SingletonGetInstance().DrawRect(m_textureCount.Get(), m_posCountUI, &rect);
		}
	}
	// リスタートカウントの描画
	if (m_restartFlag)
	{
		// 取る場所を設定
		RECT rect;
		rect.top = LONG(0.0f);
		rect.left = LONG((2 * COUNTUISIZE) - (m_restartTime / 60 * COUNTUISIZE));
		rect.right = LONG(2 * COUNTUISIZE) - (m_restartTime / 60 * COUNTUISIZE) + COUNTUISIZE;
		rect.bottom = LONG(COUNTUISIZE);

		DrawManager::SingletonGetInstance().DrawRect(m_textureCount.Get(), m_posCountUI, &rect);
	}

	if (!m_playStartFlag)
		DrawManager::SingletonGetInstance().Draw(m_textureRestart.Get(), m_posRestartUI);

	// ポインターの描画
	if (mp_gameCamera->GetStartPosMouse() && m_playStartFlag)
	{
		DrawManager::SingletonGetInstance().Draw(m_texturePointer.Get(), SimpleMath::Vector2(350.0f, 250.0f));
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
