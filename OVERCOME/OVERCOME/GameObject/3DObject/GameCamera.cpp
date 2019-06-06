//////////////////////////////////////////////////////////////
// File.    GameCamera.cpp
// Summary. GameCameraClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <math.h>
#include <cmath>

#include "../../pch.h"
#include "GameCamera.h"
#include "../SceneObject/SceneManager.h"

#include "../../Utility/InputManager.h"
#include "../../Utility/DeviceResources.h"

// usingディレクトリ
using namespace DirectX;

// constantディレクトリ
const float GameCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;
const float GameCamera::ROTATE_MAG = 300.0f;

SceneId SceneManager::m_activeScene;

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
	m_cameraDir(0.0f, 0.0f, 0.0f),
	m_mousePos(0.0f, 0.0f),
	m_dragUnit(0.0f, 0.0f),
	m_checkMousePos(false),
	m_scrollWheelValue(0),
	m_rotationX(SimpleMath::Quaternion::Identity),
	m_rotationY(SimpleMath::Quaternion::Identity),
	m_rotationTmpX(SimpleMath::Quaternion::Identity),
	m_rotationTmpY(SimpleMath::Quaternion::Identity),
	m_reformRota(SimpleMath::Quaternion::Identity)
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
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	SimpleMath::Vector3 debugPos(0.0f, 0.0f, 0.0f);
	SceneId scene = SceneManager::GetActiveScene();
	switch (scene)
	{
	case SCENE_LOGO:
		debugPos = SimpleMath::Vector3(0.0f, 5.0f, 5.0f);
		DebugCamera(debugPos);
		break;
	case SCENE_TITLE:
		debugPos = SimpleMath::Vector3(0.0f, 5.0f, 5.0f);
		DebugCamera(debugPos);
		break;
	case SCENE_SELECTSTAGE:
		debugPos = SimpleMath::Vector3(0.0f, 5.0f, 5.0f);
		DebugCamera(debugPos);
		break;
	case SCENE_PLAY:
		// マウス操作のカメラ
		target = player->GetPos();
		target.y += player->GetHeight();
		// 注視点はプレイヤーの目線の位置
		MouseOperateCamera(target, player);
		break;
	case SCENE_RESULT:
		debugPos = SimpleMath::Vector3(0.0f, 5.0f, 5.0f);
		DebugCamera(debugPos);
		break;
	}

	return true;
}

/// <summary>
/// 原点を注視点にし、周りを周回するカメラ
/// </summary>
void GameCamera::OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos)
{
	// カメラの位置設定(スタート位置)
	SimpleMath::Vector3 eye(/*20.0f, 8.0f, 0.0f*/eyePos);

	// 注視点は(0,0,0)でカメラをY軸回転させる
	m_aroundAngle += 0.5f;
	if (m_aroundAngle > 360)m_aroundAngle = 0.0f;
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(m_aroundAngle));
	eye = SimpleMath::Vector3::Transform(eye, rotY);
	//SetPositionTarget(eye, SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

	m_eyePt = eye;
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	m_targetPt = target;
}
/// <summary>
/// デバッグ用カメラ
/// </summary>
void GameCamera::DebugCamera(DirectX::SimpleMath::Vector3 debugPos)
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
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_angleTmp.y);
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_angleTmp.x);

	SimpleMath::Matrix rt = rotY * rotX;

	SimpleMath::Vector3 eye(debugPos);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - m_scrollWheelValue / 100);
	up = SimpleMath::Vector3::Transform(up, rt.Invert());

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
	SimpleMath::Vector3 eye(/*horizontalAxis*/0.0f, /*verticalAxis*/0.0f, -0.1f);

	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(direction);
	eye = SimpleMath::Vector3::Transform(eye, rotY);
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
	SimpleMath::Vector3 eye(0.0f, 0.3f, -4.0f);

	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(direction);
	eye = SimpleMath::Vector3::Transform(eye, rotY);
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
void GameCamera::MouseOperateCamera(DirectX::SimpleMath::Vector3 target, Player* player)
{
	RECT desktopWndRect;                         // デスクトップのサイズ
	HWND desktopWnd = GetDesktopWindow();        // この関数でデスクトップのハンドルを取得
	GetWindowRect(desktopWnd, &desktopWndRect);  // デスクトップのハンドルから画面の大きさを取得

	RECT activeWndRect;                          // アクティブなウィンドウのサイズ
	HWND activeWnd = GetActiveWindow();          // この関数でアクティブなウィンドウのハンドルを取得
	GetWindowRect(activeWnd, &activeWndRect);    // アクティブなウィンドウのハンドルからその画面の大きさを取得

	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);   // タイトルバーの高さを取得

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
		// ベクトルの長さを求める
		double lengthA = pow((m_cameraDir.x * m_cameraDir.x) + (m_cameraDir.z * m_cameraDir.z), 0.5);
		double lengthB = pow(((player->GetDir().x / 2.0f) * (player->GetDir().x / 2.0f)) +
			                 ((player->GetDir().z / 2.0f) * (player->GetDir().z / 2.0f)), 0.5);
		// 内積とベクトルの長さを使ってcosθを求める
		double cos_sita = m_cameraDir.x * (player->GetDir().x / 2.0f) + m_cameraDir.z * (player->GetDir().z / 2.0f) / (lengthA * lengthB);

		// cosθからθを求める
		double sita = acos(cos_sita);
		// デグリーで求める
		sita = sita * 180.0 / double(XM_PI);

		// 正の値なら右回転、負の値なら左回転
		float s = (player->GetDir().x / 2.0f) * m_cameraDir.z -
			      (player->GetDir().z / 2.0f) * m_cameraDir.x;

		// 回転制限
		// 進行方向より20度以上先を見ていたら制限範囲内に戻す
		/*if (s > 0 && sita > 90.0f)
		{
			m_reformRota *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), 0.01f);
		}
		else if (s < 0 && sita > 90.0f)
		{
			m_reformRota *= SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -0.01f);
		}
		else if(sita <= 90.0f)
		{
			m_reformRota = SimpleMath::Quaternion::Identity;
		}*/

		// 中心座標(相対値)設定
		SimpleMath::Vector2 centerPos(400.0f, 300.0f);

		// 中心点からの差分を求める
		float x = InputManager::SingletonGetInstance().GetMouseState().x - centerPos.x;
		float y = InputManager::SingletonGetInstance().GetMouseState().y - centerPos.y;

		m_rotationX = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.1f, 0.0f, 0.0f), -(y / ROTATE_MAG));
		m_rotationY = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -(x / ROTATE_MAG));

		m_rotationX *= m_toScreenOutRotaX;
		m_rotationY *= m_toScreenOutRotaY;

		/*if (sita <= 90)
		{
			// 矯正範囲外だったら偏差分の回転
			m_rotationX = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.1f, 0.0f, 0.0f), -(y / ROTATE_MAG));
			m_rotationY = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -(x / ROTATE_MAG));

			// カーソルがウィンドウ外に行ったときは元々回転していた分も回転させる
			m_rotationX *= m_toScreenOutRotaX;
			m_rotationY *= m_toScreenOutRotaY;

			// 矯正範囲内にいったとき用に回転を保存
			m_rotationTmpX = m_rotationX;
			m_rotationTmpY = m_rotationY;
		}
		else
		{
			// 矯正範囲内だったら保存しておいた分の回転のみ行う
			m_rotationX = m_rotationTmpX;
			m_rotationY = m_rotationTmpY;

			// カーソルがウィンドウ外に行ったときは元々回転していた分も回転させる
			m_rotationX *= m_toScreenOutRotaX;
			m_rotationY *= m_toScreenOutRotaY;
		}*/

		/*if (sita > 90.0f)
		{
			m_rotationY *= m_reformRota;
		}*/
		
		// 画面外に出たらカーソルを画面の中心に戻す
		if (InputManager::SingletonGetInstance().GetMouseState().x < 50 || InputManager::SingletonGetInstance().GetMouseState().x > 750 ||
			InputManager::SingletonGetInstance().GetMouseState().y < 50 || InputManager::SingletonGetInstance().GetMouseState().y > 550)
		{
			// 現在の回転を保存
			m_toScreenOutRotaX = m_rotationX;
			m_toScreenOutRotaY = m_rotationY;

			// デスクトップの値のため、ウィンドウ分のサイズ+画面の半分を足す(Yはタイトルバー分も足す)
			SetCursorPos(int(activeWndRect.left + centerPos.x), int(activeWndRect.top + centerPos.y + titlebarHeight));
		}
		
		// 視点、上方向設定
		SimpleMath::Vector3 eye(0.0f, 0.0f, 0.1f);
		eye = SimpleMath::Vector3::Transform(eye, (m_rotationX * m_rotationY));
		eye += target;

		// 視点、注視点決定
		m_eyePt = eye;
		m_targetPt = target;

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
		SimpleMath::Vector3 eye(0.0f, 0.0f, -0.1f);

		float defaultAngle = 180.0f;

		SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(defaultAngle));
		eye = SimpleMath::Vector3::Transform(eye, rotY);
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
