//////////////////////////////////////////////////////////////
// File.    GameCamera.h
// Summary. GameCameraClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "Camera.h"

#include "Player.h"

class Player;
class SceneManager;
class GameCamera : public Camera
{
// メンバー変数
public:

private:
	static const float                DEFAULT_CAMERA_DISTANCE;   // カメラの距離
	static const float                ROTATE_MAG;                // 回転倍率(100～500がベスト)

	float                             m_aroundAngle;             // 回転角

	DirectX::SimpleMath::Vector2      m_angle;                   // 現在の回転角
	DirectX::SimpleMath::Vector2      m_angleTmp;                // 保存している回転角

	DirectX::SimpleMath::Vector3      m_cameraDir;               // カメラの向きのベクトル

	DirectX::SimpleMath::Vector2      m_mousePos;                // 二次元上の座標

	DirectX::SimpleMath::Vector2      m_dragUnit;                // ドラッグされた座標
	bool                              m_checkMousePos;           // マウスの初期位置指定用変数

	int                               m_scrollWheelValue;        // スクロールホイール値

	DirectX::SimpleMath::Quaternion   m_rotationX;               // 回転(クォータニオン)
	DirectX::SimpleMath::Quaternion   m_rotationY;               // 回転(クォータニオン)
	DirectX::SimpleMath::Quaternion   m_rotationTmpX;            // 回転保存(クォータニオン)
	DirectX::SimpleMath::Quaternion   m_rotationTmpY;            // 回転保存(クォータニオン)

// メンバー関数
public:
	// コンストラクタ
	GameCamera();
	GameCamera(int windowWidth, int windowHeight);
	// デストラクタ
	~GameCamera();

	// 更新(主にカメラの切り替えを行う)
	bool Update(DX::StepTimer const & timer, Player* player);

	//-----------------------------------Getter-----------------------------------//

	// カメラの向きのベクトルを取得
	DirectX::SimpleMath::Vector3 GetCameraAngle() { return m_cameraDir; }

	// マウスが初期位置に配置されたらtrue
	bool GetStartPosMouse()                       { return m_checkMousePos; }
	//----------------------------------------------------------------------------//

private:
	/// <summary>
	/// 画面サイズの設定関数
	/// </summary>
	/// <param name="windowWidth">ウインドウサイズ（幅）</param>
	/// <param name="windowHeight">ウインドウサイズ（高さ）</param>
	void SetWindowSize(int windowWidth, int windowHeight);

	/// <summary>
	/// マウスポインタの位置のドラッグ開始位置からの変位 (相対値)
	/// </summary>
	/// <param name="x">現在のポインタのX座標</param>
	/// <param name="y">現在のポインタのY座標</param>
	void Motion(int x, int y);

	/// <summary>
	/// 原点を注視点にし、周りを周回するカメラ
	/// </summary>
	/// <param name="eyePos">注視点</param>
	void OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos);
	/// <summary>
	/// デバック用カメラ
	/// </summary>
	/// <param name="debugPos"></param>
	void DebugCamera(DirectX::SimpleMath::Vector3 debugPos);

	/// <summary>
	/// 走っているとき用カメラ
	/// </summary>
	/// <param name="target">注視点</param>
	/// <param name="direction">進行方向</param>
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	/// <summary>
	/// 後ろから追いかけるカメラ
	/// </summary>
	/// <param name="target">注視点</param>
	/// <param name="direction">進行方向</param>
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

	/// <summary>
	/// マウスで視点移動するカメラ
	/// </summary>
	/// <param name="target">注視点</param>
	void MouseOperateCamera(DirectX::SimpleMath::Vector3 target);
};
