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
}
/// <summary>
/// デストラクタ
/// </summary>
MyCamera::~MyCamera()
{
}

/// <summary>
/// 更新(主にカメラの切り替えを行う)
/// </summary>
/// <param name="timer">経過時間</param>
/// <param name="player">プレイヤー情報</param>
/// <returns>終了状態</returns>
bool MyCamera::Update(DX::StepTimer const & timer, Player* player)
{
	// カメラの切り替えを決めるフラグ
	static bool cameraFlag = true;
	
	// Bキーでカメラ切り替え(仮)
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
/// 原点を注視点にし、周りを周回するカメラ
/// </summary>
void MyCamera::OriginPointAroundCamera()
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
/// 走っているときのプレイヤー目線カメラ
/// </summary>
/// <param name="target">注視点</param>
/// <param name="direction">プレイヤーの向いている方向</param>
void MyCamera::RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// サイン波が変動するための値
	static float wave;
	wave += 0.1f;
	// 水平方向のカメラの揺れ
	float horizontalAxis = sin(wave) *2.0f / 300.0f;

	Vector3 eye(horizontalAxis/*0.0f*/, 0.0f, -0.1f);
	
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
