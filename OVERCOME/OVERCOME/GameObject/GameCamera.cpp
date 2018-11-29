//////////////////////////////////////////////////////////////
// File.    GameCamera.cpp
// Summary. GameCameraClass
// Date.    2018/10/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <math.h>
#include <cmath>

#include "../pch.h"
#include "GameCamera.h"

#include "../Utility/InputManager.h"
#include "../Utility/DeviceResources.h"

#include "../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;

const float GameCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;

/// <summary>
/// コンストラクタ
/// </summary>
GameCamera::GameCamera() :m_aroundAngle(0.0f)
{
}
GameCamera::GameCamera(int windowWidth, int windowHeight)
	: m_aroundAngle(0.0f),
	m_angle(0.0f, 0.0f),
	m_angleTmp(0.0f, 0.0f),
	//m_cameraDir(0.0f, 0.0f, 0.0f),
	m_mousePos(0.0f, 0.0f),
	m_dragUnit(0.0f, 0.0f),
	m_checkMousePos(false),
	m_scrollWheelValue(0)
{
	SetWindowSize(windowWidth, windowHeight);
}
/// <summary>
/// デストラクタ
/// </summary>
GameCamera::~GameCamera()
{
}

/// <summary>
/// 更新(主にカメラの切り替えを行う)
/// </summary>
/// <param name="timer">経過時間</param>
/// <param name="player">プレイヤー情報</param>
/// <returns>終了状態</returns>
bool GameCamera::Update(DX::StepTimer const & timer, Player* player)
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

	// フラグに応じたカメラ管理
	if (cameraFlag)
	{
		/*Vector3 target(player->GetPos());
		target.y += player->GetHeight();
		RunPlayerCamera(target, player->GetDirection());*/

		/*Vector3 target(player->GetPos());
		target.y += player->GetHeight();
		MouseOperateCamera(target, player->GetDirection());*/

		DebugCamera();
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
void GameCamera::OriginPointAroundCamera()
{
	// カメラの位置設定(スタート位置)
	Vector3 eye(20.0f, 8.0f, 0.0f);

	// 注視点は(0,0,0)でカメラをY軸回転させる
	m_aroundAngle += 0.5f;
	Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_aroundAngle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));
}
/// <summary>
/// デバッグ用カメラ
/// </summary>
void GameCamera::DebugCamera()
{
	// 相対モードなら何もしない
	if (InputManager::SingletonGetInstance().GetMouseState().positionMode == Mouse::MODE_RELATIVE) return;

	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// マウスの左ボタンが押された
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		// マウスの座標を取得
		m_mousePos.x = float(InputManager::SingletonGetInstance().GetMouseState().x);
		m_mousePos.y = float(InputManager::SingletonGetInstance().GetMouseState().y);
	}
	else if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		// 現在の回転を保存
		m_angle.x = m_angleTmp.x;
		m_angle.y = m_angleTmp.y;
	}
	// マウスのボタンが押されていたらカメラを移動させる
	if (InputManager::SingletonGetInstance().GetMouseState().leftButton)
	{
		Motion(InputManager::SingletonGetInstance().GetMouseState().x, InputManager::SingletonGetInstance().GetMouseState().y);
	}

	// マウスのフォイール値を取得
	m_scrollWheelValue = InputManager::SingletonGetInstance().GetMouseState().scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		Mouse::Get().ResetScrollWheelValue();
	}

	// ビュー行列を算出する
	Matrix rotY = Matrix::CreateRotationY(m_angleTmp.y);
	Matrix rotX = Matrix::CreateRotationX(m_angleTmp.x);

	Matrix rt = rotY * rotX;

	Vector3 eye(0.0f, 5.0f, 5.0f);
	Vector3 target(0.0f, 0.0f, 0.0f);
	Vector3 up(0.0f, 1.0f, 0.0f);

	eye = Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - m_scrollWheelValue / 100);
	up = Vector3::Transform(up, rt.Invert());

	m_eyePt = eye;
	m_targetPt = target;
}

/// <summary>
/// 走っているときのプレイヤー目線カメラ
/// </summary>
/// <param name="target">注視点</param>
/// <param name="direction">プレイヤーの向いている方向</param>
void GameCamera::RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// サイン波が変動するための値
	static float wave;
	wave += 0.1f;
	// 水平方向のカメラの揺れ
	float horizontalAxis = sin(wave) * 2.0f / 500.0f;
	// 垂直方向のカメラの揺れ
	float verticalAxis = (sin(wave) + 1.0f) / 500.0f;

	// 視点設定
	Vector3 eye(/*horizontalAxis*/0.0f, /*verticalAxis*/0.0f, -0.1f);

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
void GameCamera::FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// 視点設定
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

/// <summary>
/// マウスで視点移動するカメラ
/// </summary>
void GameCamera::MouseOperateCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// 相対モードなら何もしない
	if (InputManager::SingletonGetInstance().GetMouseState().positionMode == Mouse::MODE_RELATIVE) return;
	// マウスの更新
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// マウスポインターと円の衝突判定
	float distX = 400.0f - float(InputManager::SingletonGetInstance().GetMouseState().x);
	float distY = 300.0f - float(InputManager::SingletonGetInstance().GetMouseState().y);
	float distX2 = distX * distX;
	float distY2 = distY * distY;
	float r = 100.0f;
	float r2 = r * r;
	
	// マウスポインターが画面中央に来たら視点移動を開始する
	if (distX2 + distY2 <= r2 &&
		InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_checkMousePos = true;
	}

	if(m_checkMousePos == true)
	{
		// 中心座標設定(移動量を三倍にするため中心座標も三倍に)
		m_mousePos.x = 400.0f;
		m_mousePos.y = 300.0f;

		bool flag = false;

		//if (InputManager::SingletonGetInstance().GetMouseState().x < 10 || InputManager::SingletonGetInstance().GetMouseState().x > 790 ||
		//	InputManager::SingletonGetInstance().GetMouseState().y < 10 || InputManager::SingletonGetInstance().GetMouseState().y > 590)
		//{
		//	// 現在の回転を保存
		//	m_angle.x = m_angleTmp.x;
		//	m_angle.y = m_angleTmp.y;
		//	flag = true;

		//	m_mousePos.x = InputManager::SingletonGetInstance().GetMouseState().x;
		//	m_mousePos.y = InputManager::SingletonGetInstance().GetMouseState().y;

		//	SetCursorPos(400, 300);
		//}

		Vector2 mouseWhereabouts;
		mouseWhereabouts.x = float(InputManager::SingletonGetInstance().GetMouseState().x);
		mouseWhereabouts.y = float(InputManager::SingletonGetInstance().GetMouseState().y);

		if (mouseWhereabouts.x < m_mousePos.x) { mouseWhereabouts.x = (m_mousePos.x)-((m_mousePos.x - InputManager::SingletonGetInstance().GetMouseState().x) * 10); }
		if (mouseWhereabouts.x > m_mousePos.x) { mouseWhereabouts.x = (m_mousePos.x)+((InputManager::SingletonGetInstance().GetMouseState().x - m_mousePos.x) * 10); }
		if (mouseWhereabouts.y < m_mousePos.y) { mouseWhereabouts.y = (m_mousePos.y)-((m_mousePos.y - InputManager::SingletonGetInstance().GetMouseState().y) * 10); }
		if (mouseWhereabouts.y > m_mousePos.y) { mouseWhereabouts.y = (m_mousePos.y)+((InputManager::SingletonGetInstance().GetMouseState().y - m_mousePos.y) * 10); }

		// 偏差分を移動
	    Motion(int(mouseWhereabouts.x), int(mouseWhereabouts.y));

		// ビュー行列を算出する
		Matrix rotY = Matrix::CreateRotationY(-m_angleTmp.y + XMConvertToRadians(180.0f));
		Matrix rotX = Matrix::CreateRotationX(-m_angleTmp.x);
		Matrix rt = rotY * rotX;

		Vector3 eye(0.0f, 0.0f, -0.1f);
		//Vector3 target(target);
		//Vector3 up(0.0f, 1.0f, 0.0f);

		// 視点、上方向設定
		eye = Vector3::Transform(eye, rt);
		eye += target;
		//up = Vector3::Transform(up, rt.Invert());

		// 視点、注視点決定
		m_eyePt = eye;
		m_targetPt = target;

		/*//ベクトルの長さ
		float lengthX = std::abs(eye.x - target.x);
		float lengthY = std::abs(eye.y - target.y);
		float lengthZ = std::abs(eye.z - target.z);

		double length = pow((lengthX * lengthX) + (lengthY * lengthY), 0.5);

		//XY各成分を長さで割る
		Vector2D unit;
		unit.x = v.x / length;
		unit.y = v.y / length;*/

		m_cameraDir.x = (target.x - eye.x);
		m_cameraDir.y = (target.y - eye.y);
		m_cameraDir.z = (target.z - eye.z);

		m_cameraDir.Normalize();

		m_cameraDir.x *= 2.0f;
		m_cameraDir.y *= 2.0f;
		m_cameraDir.z *= 2.0f;
	}
	else
	{
		// 視点設定
		Vector3 eye(0.0f, 0.0f, -0.1f);

		Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(180.0f));
		eye = Vector3::Transform(eye, rotY);
		eye += target;

		m_eyePt = eye;
		m_targetPt = target;
	}
}

/// <summary>
/// マウスポインタの位置のドラッグ開始位置からの変位 (相対値)
/// </summary>
/// <param name="x">現在のポインタのX座標</param>
/// <param name="y">現在のポインタのY座標</param>
void GameCamera::Motion(int x, int y)
{
	// 相対的な位置を算出
	float dragPosX = (x - m_mousePos.x) * m_dragUnit.x;
	float dragPosY = (y - m_mousePos.y) * m_dragUnit.y;

	// クリックした座標と異なっていたら
	if (dragPosX != 0.0f || dragPosY != 0.0f)
	{
		// 回転量を設定
		float yAngle = dragPosX * XM_PI;
		float xAngle = dragPosY * XM_PI;

		// 現在の回転量に加える
		m_angleTmp.x = m_angle.x + xAngle;
		m_angleTmp.y = m_angle.y + yAngle;
	}
}

/// <summary>
/// 画面サイズに対する相対的なスケールに調整
/// </summary>
/// <param name="windowWidth">画面サイズ(横幅)</param>
/// <param name="windowHeight">画面サイズ(縦幅)</param>
void GameCamera::SetWindowSize(int windowWidth, int windowHeight)
{
	m_dragUnit.x = (1.0f / float(windowWidth)) / 5.0f;
	m_dragUnit.y = (1.0f / float(windowHeight)) / 5.0f;
}
