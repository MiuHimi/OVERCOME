//////////////////////////////////////////////////////////////
// File.    MyCamera.h
// Summary. MyCameraClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

#include "Camera.h"

#include "Player.h"

class Player;
class MyCamera : public Camera
{
// メンバー変数
public:

private:
	// 回転角
	float m_angle;

	float m_count = 0.0f;

	Player* mp_player;

// メンバー関数
public:
	// コンストラクタ
	MyCamera();
	// デストラクタ
	~MyCamera();

	bool Update(DX::StepTimer const & timer, Player* player);

	// タイトルシーン用カメラ
	void TitleSceneCamera();
	// ゲームシーン用カメラ
	void GameSceneCamera(DirectX::SimpleMath::Vector3 target, float direction);

	// 走っているとき用カメラ
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// 後ろから追いかけるカメラ
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

private:

};