//////////////////////////////////////////////////////////////
// File.    MyCamera.h
// Summary. MyCameraClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

#include "Camera.h"

#include "Player.h"

class Player;
class MyCamera : public Camera
{
// メンバー変数
public:

private:
	float          m_angle;          // 回転角

// メンバー関数
public:
	// コンストラクタ
	MyCamera();
	// デストラクタ
	~MyCamera();

	// 更新(主にカメラの切り替えを行う)
	bool Update(DX::StepTimer const & timer, Player* player);

	// 原点を注視点にし、周りを周回するカメラ
	void OriginPointAroundCamera();

	// 走っているとき用カメラ
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// 後ろから追いかけるカメラ
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

private:

};