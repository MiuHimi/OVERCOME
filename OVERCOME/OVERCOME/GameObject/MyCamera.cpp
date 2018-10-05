//////////////////////////////////////////////////////////////
// File.    MyCamera.cpp
// Summary. MyCameraClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// ヘッダをインクルード
#include "../pch.h"
#include "MyCamera.h"

#include "../ExclusiveGameObject/InputManager.h"

#include <math.h>
#include <cmath>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
MyCamera::MyCamera():m_angle(0.0f)
{
	mp_player = new Player();
	//mp_player = Player::GetPlayer();
	mp_player->Initialize();
}
/// <summary>
/// デストラクタ
/// </summary>
MyCamera::~MyCamera()
{
	delete mp_player;
	mp_player = nullptr;
}

/// <summary>
/// 更新
/// </summary>
bool MyCamera::Update(DX::StepTimer const & timer, Player* player)
{
	// プレイヤー情報を更新
	//mp_player->Update(timer);

	static bool cameraFlag = true;
	/*Vector3 target(player->GetPos());
	target.y += player->GetHeight();
	RunPlayerCamera(target, player->GetDirection());*/
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::B))
	{
		if (cameraFlag == true)
		{
			cameraFlag = false;
			ResetCamera();
		}
		else if (cameraFlag == false)
		{
			cameraFlag = true;
			ResetCamera();
		}
	}
	if (cameraFlag)
	{
		Vector3 target(player->GetPos());
		target.y += player->GetHeight();
		RunPlayerCamera(target, player->GetDirection());
	}
	else
	{
		Vector3 target(player->GetPos());
		target.y += player->GetHeight();
		FollowPlayerCamera(target, player->GetDirection());
	}

	return true;
}

/// <summary>
/// タイトルシーン用カメラ
/// </summary>
void MyCamera::TitleSceneCamera()
{
	// カメラの位置設定(スタート位置)
	Vector3 eye(20.0f, 8.0f, 0.0f);

	// 注視点は(0,0,0)でカメラをY軸回転させる
	m_angle += 0.5f;
	Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));
}

/// <summary>
/// ゲームシーン用カメラ
/// </summary>
void MyCamera::GameSceneCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	Vector3 eye(0.0f, 3.0f, -5.0f);

	Matrix rotY = Matrix::CreateRotationY(direction);
	eye = Vector3::Transform(eye, rotY);
	eye += target;

	// 補間する場合
	SetPositionTarget(eye, target);

	// 補間しない場合
	//m_eyePt = eye;
	//m_targetPt = target;

	/*Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));*/
}

/// <summary>
/// 走っているときのプレイヤー目線カメラ
/// </summary>
/// <param name="target">注視点</param>
/// <param name="direction">プレイヤーの向いている方向</param>
void MyCamera::RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// サイン波が変動するための値
	static float wave;
	wave += 0.01f;
	float sinWave = sin(wave + (0.5f));
	// // sin波の値 = sin(一フレームで変動する値+(複数いる場合、個数×nでそれぞれをずらすことができる)) * ふり幅(どれほどの幅で上下or左右移動するか)

	Vector3 eye(0.0f, 0.0f, -0.1f);

	/*m_count += 0.01f;
	Vector3 targetPos(target);
	targetPos.x = sin(2 * XM_PI * 2)30 + sin(XM_PI * 2 / 30 * m_count);
	if (m_count > 0.1f || m_count < -0.1f) m_count *= -1;
	targetPos.x += m_count;*/
	
	Matrix rotY = Matrix::CreateRotationY(direction);
	eye = Vector3::Transform(eye, rotY);
	eye += target;

	// 補間する場合
	//SetPositionTarget(eye, target);

	// 補間しない場合
	m_eyePt = eye;
	m_targetPt = target;
}

/// <summary>
/// 後ろから追いかけるカメラ
/// </summary>
/// <param name="target">注視点</param>
/// <param name="direction">プレイヤーの向いている方向</param>
void MyCamera::FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	Vector3 eye(0.0f, 0.3f, -4.0f);

	Matrix rotY = Matrix::CreateRotationY(direction);
	eye = Vector3::Transform(eye, rotY);
	eye += target;

	// 補間する場合
	//SetPositionTarget(eye, target);

	// 補間しない場合
	m_eyePt = eye;
	m_targetPt = target;
}
