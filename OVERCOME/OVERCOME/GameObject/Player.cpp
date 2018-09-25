//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "Player.h"

#include "../ExclusiveGameObject/InputManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
{
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
Player::Player(Game * game) : mp_game(game),
                              m_pos(22.5f, 2.0f, 7.5f),
	                          m_vel(0.0f, 0.0f, 0.0f)
{
}
/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize()
{
	//m_direction = 0.0f;
	m_direction = XMConvertToRadians(180.0f);
	m_height = 1.75f;

	m_jumpForce = 0.0f;
	m_gravity = 0.03f;

	m_world = SimpleMath::Matrix::Identity;
}
/// <summary>
/// 生成処理
/// </summary>
void Player::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(mp_game->GetDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelPlayer = Model::CreateFromCMO(mp_game->GetDevice(), L"Resources\\Models\\player.cmo", fx);

	// プレイヤーの作成
	SetGame(mp_game);
	SetModel(m_modelPlayer.get());
	
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool Player::Update(DX::StepTimer const & timer)
{
	// 速度初期化
	m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	
	// プレイヤー操作
	if (InputManager::GetInstance().GetKeyState().Up)
	{
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.2f);
	}
	if (InputManager::GetInstance().GetKeyState().Down)
	{
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, -0.2f);
	}
	if (InputManager::GetInstance().GetKeyState().Left)
	{
		m_direction += XMConvertToRadians(2.0f);
	}
	if (InputManager::GetInstance().GetKeyState().Right)
	{
		m_direction += XMConvertToRadians(-2.0f);
	}

	if (InputManager::GetInstance().GetKeyState().Space && m_isJump == false ||
		InputManager::GetInstance().GetKeyState().Space && m_isCollide == true && m_pos.y >= 0.95f)
	{
		m_isJump = true;
		m_vel.y = 0.0f;
		m_jumpForce = 0.5f;
	}

	if (m_isJump == true)
	{
		m_vel.y += m_jumpForce;
		if (m_vel.y < -1.0f) m_vel.y = -1.0f;
		m_jumpForce -= m_gravity;
	}
	else if (m_isJump == false && m_isCollide == false)
	{
		m_vel.y -= m_gravity;
	}
	else if (m_isJump == true && m_isCollide == true)
	{
		m_vel.y = 0.0f;
	}

	/*if (m_isJump == false)
	{
		if (m_pos.y > 0.0f)
		{
			m_vel.y += m_jumpForce;
			m_jumpForce -= m_gravity;
		}
	}*/

	if (m_pos.y <= 0.0f)m_pos.y = 0.0f;

	// プレイヤーを移動
	m_rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), m_direction);
	m_vel = SimpleMath::Vector3::Transform(m_vel, m_rotation);
	m_pos += m_vel;

	// ワールド行列の作成
	m_world = SimpleMath::Matrix::CreateFromQuaternion(m_rotation) * SimpleMath::Matrix::CreateTranslation(m_pos);

	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // 境界箱の中心
	box.r = DirectX::SimpleMath::Vector3(0.5f, m_height / 2.0f, 0.5f);                        // 各半径0.5
	SetCollision(box);

	return true;
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Render(DirectX::SimpleMath::Matrix view)
{
	/*SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	trans = SimpleMath::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);*/
	//m_rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), m_direction);
	//rot = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(m_direction));

	// 床の描画
	m_modelPlayer->Draw(mp_game->GetContext(), *mp_game->GetState(), m_world, view, mp_game->GetProjection());
}

/// <summary>
/// 削除処理
/// </summary>
void Player::Depose()
{
}

Player* Player::GetPlayer()
{
	return this;
}
