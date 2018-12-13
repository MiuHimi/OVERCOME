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
	static const float                 DEFAULT_CAMERA_DISTANCE;           // カメラの距離

	float                              m_aroundAngle;                     // 回転角

	DirectX::SimpleMath::Vector2       m_angle;                           // 現在の回転角
	DirectX::SimpleMath::Vector2       m_angleTmp;                        // 保存している回転角
	const float                        m_angleMag = 10.0f;                 // マウスによるカメラ回転の倍率 

	DirectX::SimpleMath::Vector3       m_cameraDir;                       // カメラの向きのベクトル

	DirectX::SimpleMath::Vector2       m_mousePos;                        // 二次元上の絶対値(座標)

	DirectX::SimpleMath::Vector2       m_dragUnit;                        // ドラッグされた座標
	bool                               m_checkMousePos;                   // マウスの初期位置指定用変数

	int                                m_scrollWheelValue;                // スクロールホイール値

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
	void OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos);

	// デバック用カメラ
	void DebugCamera(DirectX::SimpleMath::Vector3 debugPos);

	// 走っているとき用カメラ
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// 後ろから追いかけるカメラ
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

	// マウスで視点移動するカメラ
	void MouseOperateCamera(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 ahead);

	/// <summary>
	/// 画面サイズの設定関数
	/// </summary>
	/// <param name="windowWidth">ウインドウサイズ（幅）</param>
	/// <param name="windowHeight">ウインドウサイズ（高さ）</param>
	void SetWindowSize(int windowWidth, int windowHeight);

	// カメラの向きのベクトルを取得
	DirectX::SimpleMath::Vector3 GetCameraAngle() { return m_cameraDir; }

	// 回転角の設定、取得
	DirectX::SimpleMath::Vector2 SetAngle(DirectX::SimpleMath::Vector2 angle) { m_angleTmp = angle; }
	DirectX::SimpleMath::Vector2 GetAngle()                                   { return m_angleTmp; }

	// マウスが初期位置に配置されたらtrue
	bool GetStartPosMouse() { return m_checkMousePos; }

private:

	void Motion(int x, int y);

};
