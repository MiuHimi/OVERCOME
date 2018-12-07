//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "Player.h"

#include "../Utility/InputManager.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"
#include "../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
Player::Player() : m_pos(22.5f, 2.0f, 7.5f),
	               m_vel(0.0f, 0.0f, 0.0f),
	               mp_gameCamera(nullptr),
	               mp_bulletManager(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	//Player::Depose();
}

/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize()
{
	// 各変数初期化
	m_accel        = 0.0f;                                        // 加速度
	m_direction    = XMConvertToRadians(0.0f);                    // 向き(角度)
	m_rotation     = DirectX::SimpleMath::Quaternion::Identity;   // 回転
	m_height       = 1.75f;                                       // プレイヤー自身の高さ
	m_jumpForce    = 0.0f;                                        // ジャンプ力
	m_gravity      = 0.03f;                                       // 重力
	m_fallingPower = 0.0f;                                        // そのまま落ちる時の力

	m_world = SimpleMath::Matrix::Identity;                       // ワールド行列

	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
	mp_bulletManager = /*std::make_unique<GameBulletManager>();*/new GameBulletManager();
	mp_bulletManager->Initialize();
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
	
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool Player::Update(DX::StepTimer const & timer)
{
	mp_gameCamera->Update(timer,this);

	m_vel = DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_accel);

	if (mp_gameCamera->GetStartPosMouse())
	{
		debugFlag = true;
	}
	if(debugFlag)
	{
		mp_bulletManager->Update(timer, m_pos, mp_gameCamera->GetCameraAngle());
	}

	// プレイヤー移動(ベクトル)
	
	
	if (mp_gameCamera->GetStartPosMouse())
	{
		// 加速度設定
		m_accel -= 0.01f;
		if (m_accel < -0.05f)
		{
			m_accel = -0.05f;
		}
		// 速度初期化
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, -0.1f + m_accel);
	}
	if (InputManager::SingletonGetInstance().GetKeyState().S/*mp_gameCamera->GetStartPosMouse()*/)
	{
		// 加速度設定
		//m_accel += 0.001f;
		// 速度初期化
		m_vel = /*SimpleMath::Vector3(0.0f, 0.0f, 0.1f + m_accel);*/SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	}

	// プレイヤーの向き設定
	if (InputManager::SingletonGetInstance().GetKeyState().A)
	{
		m_direction += XMConvertToRadians(2.0f);
	}
	if (InputManager::SingletonGetInstance().GetKeyState().D)
	{
		m_direction -= XMConvertToRadians(2.0f);
	}
	
	
	if (InputManager::SingletonGetInstance().GetKeyState().Up == false &&
		InputManager::SingletonGetInstance().GetKeyState().Down == false &&
		m_vel.z < 0.1f && m_vel.z > -0.1f)
	{
		// 移動が遅すぎる場合は停止する
		m_vel.z = 0.0f;
	}

	// ジャンプ
	if (InputManager::SingletonGetInstance().GetKeyState().W && m_isJump == false ||
		InputManager::SingletonGetInstance().GetKeyState().W && m_collideToRoad == true && m_pos.y >= 0.95f)
	{
		m_isJump = true;
		m_vel.y = 0.0f;
		m_jumpForce = 0.5f;
	}

	// ジャンプ力調整
	if (m_isJump == true && m_collideToRoad == false || m_isJump == true && m_collideToRoad == true && m_pos.y >= 0.85f)
	{
		m_vel.y += m_jumpForce;
		if (m_vel.y < -1.0f) m_vel.y = -1.0f;
		m_jumpForce -= m_gravity;
	}
	else if (m_isJump == true && m_collideToRoad == true)
	{
		m_vel.y = 0.0f;
	}

	// 何のオブジェクトにも触れず、かつジャンプもしていないときの処理
	if (m_isJump == false && m_collideToRoad == false && m_collideToFloor == false && m_noTouchObectFlag == false)
	{
		m_fallingPower = 0;
		m_noTouchObectFlag = true;
	}
	if (m_noTouchObectFlag == true)
	{
		m_fallingPower -= m_gravity;
		m_vel.y += m_fallingPower;
	}

	// 速度上限
	if (m_vel.y < -0.3f)
	{
		m_vel.y = -0.3f;
	}

	// プレイヤー移動(座標)
	m_rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), m_direction);
	m_vel = SimpleMath::Vector3::Transform(m_vel, m_rotation);
	m_pos += m_vel;

	// ワールド行列の作成
	m_world = SimpleMath::Matrix::CreateFromQuaternion(m_rotation) * SimpleMath::Matrix::CreateTranslation(m_pos);

	// 衝突判定用の仮想オブジェクト生成
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // 境界箱の中心
	box.r = DirectX::SimpleMath::Vector3(0.5f, m_height / 2.0f, 0.5f);                        // 各半径
	SetCollision(box);

	return true;
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Render()
{
	// プレイヤーの描画
	//m_modelPlayer->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
	//	           m_world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());

	mp_bulletManager->Render();
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
