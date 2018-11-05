//////////////////////////////////////////////////////////////
// File.    GameCamera.h
// Summary. GameCameraClass
// Date.    2018/10/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

#include "Camera.h"

#include "Player.h"

class Player;
class GameCamera : public Camera
{
// メンバー変数
public:

private:
	float          m_angle;          // 回転角

	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// 横回転
	float m_yAngle, m_yTmp;
	// 縦回転
	float m_xAngle, m_xTmp;

	int m_posX, m_posY;  // 二次元上の絶対値(座標)

	// ドラッグされた座標
	
	float m_sx, m_sy;

	// スクロールフォイール値
	int m_scrollWheelValue;


// メンバー関数
public:
	// コンストラクタ
	GameCamera();
	GameCamera(int windowWidth, int windowHeight);
	// デストラクタ
	~GameCamera();

	// 更新(主にカメラの切り替えを行う)
	bool Update(DX::StepTimer const & timer, Player* player);

	// 原点を注視点にし、周りを周回するカメラ
	void OriginPointAroundCamera();

	// 走っているとき用カメラ
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// 後ろから追いかけるカメラ
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

	// マウスで視点移動するカメラ
	void MouseOperateCamera();

	/// <summary>
	/// 画面サイズの設定関数
	/// </summary>
	/// <param name="windowWidth">ウインドウサイズ（幅）</param>
	/// <param name="windowHeight">ウインドウサイズ（高さ）</param>
	void SetWindowSize(int windowWidth, int windowHeight);

private:

	void Motion(int x, int y);

};
