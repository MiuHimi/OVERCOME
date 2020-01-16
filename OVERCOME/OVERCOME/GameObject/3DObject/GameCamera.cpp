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

// constディレクトリ
const float GameCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;
const float GameCamera::ROTATE_MAG = 500.0f;
const int	GameCamera::OUT_SIZE_OF_SCREEN = 50;

SceneId SceneManager::m_activeScene;

/// <summary>
/// コンストラクタ
/// </summary>
GameCamera::GameCamera() :m_aroundAngleY(0.0f)
{
}
GameCamera::GameCamera(int windowWidth, int windowHeight, bool isFullScreen)
	: m_isFullScreen(isFullScreen),
	m_aroundAngleX(0.0f), m_aroundAngleY(0.0f),
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
/// <param name="playerPos">プレイヤー座標</param>
/// <param name="playerHeight">"プレイヤー高さ"</param>
/// <param name="playerDir">"プレイヤーの向き"</param>
/// <param name="isPlayFlag">"プレイシーン最初におけるプレイヤー移動可否フラグ(プレイシーン以外は常にtrue)"</param>
/// <returns>終了状態</returns>
bool GameCamera::Update(DX::StepTimer const & timer, DirectX::SimpleMath::Vector3& playerPos, float playerHeight, DirectX::SimpleMath::Vector3& playerDir, const bool isPlayFlag)
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
		// 定点カメラ
		FixedPointCamera(SimpleMath::Vector3(0.0f, 50.0f, 0.0f), SimpleMath::Vector3(0.0f, 50.0f, 150.0f));
		break;
	case SCENE_SELECTSTAGE:
		debugPos = SimpleMath::Vector3(0.0f, 5.0f, 5.0f);
		DebugCamera(debugPos);
		break;
	case SCENE_PLAY:
		// マウス操作のカメラ
		target = SimpleMath::Vector3(playerPos.x, playerPos.y + playerHeight, playerPos.z);
		// 注視点はプレイヤーの目線の位置
		MouseOperateCamera(target, playerDir, isPlayFlag);
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
	m_aroundAngleY += 0.5f;
	if (m_aroundAngleY > 360)m_aroundAngleY = 0.0f;
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(m_aroundAngleY));
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
/// 定点カメラ
/// </summary>
/// <param name="target">注視点</param>
/// <param name="cameraPoint">カメラの位置</param>
void GameCamera::FixedPointCamera(DirectX::SimpleMath::Vector3 &target, DirectX::SimpleMath::Vector3 &cameraPoint)
{
	m_eyePt = cameraPoint;
	m_targetPt = target;
}

/// <summary>
/// 基準点から領域のサイズ分の中を流れるように動くカメラ
/// </summary>
/// <param name="basePoint">基準点</param>
/// <param name="width">幅(X)</param>
/// <param name="height">高さ(Y)</param>
/// <param name="depth">奥行(Z)</param>
/// <param name="areaSize">領域のサイズ</param>
void GameCamera::FadeCamera(DirectX::SimpleMath::Vector3 basePoint, float width, float height, float depth, float areaSize)
{
	// カメラの初期位置設定
	SimpleMath::Vector3 eye(20.0f, 40.0f, 0.0f);

	// 回転
	m_aroundAngleX += 0.5f;
	m_aroundAngleY += 0.5f;
	// 回転矯正
	if (m_aroundAngleY > 360)m_aroundAngleY = 0.0f;
	if (m_aroundAngleY > 360)m_aroundAngleY = 0.0f;
	SimpleMath::Matrix rotZ = SimpleMath::Matrix::CreateRotationZ(XMConvertToRadians(m_aroundAngleX));
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(m_aroundAngleY));
	SimpleMath::Matrix rot = rotY * rotZ;
	eye = SimpleMath::Vector3::Transform(eye, rot);

	m_eyePt = eye;
	SimpleMath::Vector3 target(0.0f, 20.0f, 0.0f);
	m_targetPt = target;
}

/// <summary>
/// マウスで視点移動するカメラ
/// </summary>
void GameCamera::MouseOperateCamera(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 playerDirction, const bool isPlayFlag)
{
	if(isPlayFlag == true)
	{
		// ベクトルの長さを求める
		double lengthA = pow((m_cameraDir.x * m_cameraDir.x) + (m_cameraDir.z * m_cameraDir.z), 0.5);
		double lengthB = pow(((playerDirction.x / 2.0f) * (playerDirction.x / 2.0f)) +
			                 ((playerDirction.z / 2.0f) * (playerDirction.z / 2.0f)), 0.5);
		// 内積とベクトルの長さを使ってcosθを求める
		double cos_sita = m_cameraDir.x * (playerDirction.x / 2.0f) + m_cameraDir.z * (playerDirction.z / 2.0f) / (lengthA * lengthB);

		// cosθからθを求める
		double sita = acos(cos_sita);
		// デグリーで求める
		sita = sita * 180.0 / double(XM_PI);

		// 正の値なら右回転、負の値なら左回転
		float s = (playerDirction.x / 2.0f) * m_cameraDir.z -
			      (playerDirction.z / 2.0f) * m_cameraDir.x;

		// アクティブなウィンドウのハンドルからその画面の大きさを取得
		RECT activeWndRect;
		HWND activeWnd = GetActiveWindow();
		GetWindowRect(activeWnd, &activeWndRect);

		// ウィンドウのサイズを取得
		float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
		float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);
		// タイトルバーの高さを取得
		int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);   

		// 中心座標設定
		SimpleMath::Vector2 centerPos;
		if(m_isFullScreen)  centerPos = SimpleMath::Vector2((windowWidth * 0.5f),(windowHeight * 0.5f));
		else				centerPos = SimpleMath::Vector2((windowWidth * 0.5f), ((windowHeight + titlebarHeight) * 0.5f));

		// 中心点からの差分を求める
		float x = InputManager::SingletonGetInstance().GetMouseState().x - centerPos.x;
		float y = InputManager::SingletonGetInstance().GetMouseState().y - centerPos.y;

		m_rotationX = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.1f, 0.0f, 0.0f), -(y / ROTATE_MAG));
		m_rotationY = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -(x / ROTATE_MAG));

		m_rotationX *= m_toScreenOutRotaX;
		m_rotationY *= m_toScreenOutRotaY;
		
		// 画面外に出たらカーソルを画面の中心に戻す
		if (InputManager::SingletonGetInstance().GetMouseState().x < int((activeWndRect.left - activeWndRect.left) + OUT_SIZE_OF_SCREEN) || 
			InputManager::SingletonGetInstance().GetMouseState().x > int((activeWndRect.right - activeWndRect.left) - OUT_SIZE_OF_SCREEN) ||
			InputManager::SingletonGetInstance().GetMouseState().y < int((activeWndRect.top - activeWndRect.top) + OUT_SIZE_OF_SCREEN) || 
			InputManager::SingletonGetInstance().GetMouseState().y > int((activeWndRect.bottom - activeWndRect.top) - OUT_SIZE_OF_SCREEN))
		{
			// 現在の回転を保存
			m_toScreenOutRotaX = m_rotationX;
			m_toScreenOutRotaY = m_rotationY;

			// スクリーンに応じて画面の中心に戻す
			if (m_isFullScreen) SetCursorPos(int(activeWndRect.left + centerPos.x), int(activeWndRect.top + centerPos.y));
			else				SetCursorPos(int(activeWndRect.left + centerPos.x), int(activeWndRect.top + centerPos.y + titlebarHeight));
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
