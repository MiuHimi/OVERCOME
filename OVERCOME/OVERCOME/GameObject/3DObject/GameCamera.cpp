//////////////////////////////////////////////////////////////
// File.    GameCamera.cpp
// Summary. GameCameraClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <math.h>
#include <cmath>

#include "../../pch.h"
#include "GameCamera.h"
#include "../SceneObject/SceneManager.h"

#include "../../Utility/InputManager.h"
#include "../../Utility/DeviceResources.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const float GameCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;
const float GameCamera::ROTATE_MAG = 500.0f;
const int	GameCamera::OUT_SIZE_OF_SCREEN = 50;

SceneId SceneManager::m_activeScene;

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
GameCamera::~GameCamera()
{
}

/// <summary>
/// �X�V(��ɃJ�����̐؂�ւ����s��)
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <param name="playerPos">�v���C���[���W</param>
/// <param name="playerHeight">"�v���C���[����"</param>
/// <param name="playerDir">"�v���C���[�̌���"</param>
/// <param name="isPlayFlag">"�v���C�V�[���ŏ��ɂ�����v���C���[�ړ��ۃt���O(�v���C�V�[���ȊO�͏��true)"</param>
/// <returns>�I�����</returns>
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
		// ��_�J����
		FixedPointCamera(SimpleMath::Vector3(0.0f, 50.0f, 0.0f), SimpleMath::Vector3(0.0f, 50.0f, 150.0f));
		break;
	case SCENE_SELECTSTAGE:
		debugPos = SimpleMath::Vector3(0.0f, 5.0f, 5.0f);
		DebugCamera(debugPos);
		break;
	case SCENE_PLAY:
		// �}�E�X����̃J����
		target = SimpleMath::Vector3(playerPos.x, playerPos.y + playerHeight, playerPos.z);
		// �����_�̓v���C���[�̖ڐ��̈ʒu
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
/// ���_�𒍎��_�ɂ��A��������񂷂�J����
/// </summary>
void GameCamera::OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos)
{
	// �J�����̈ʒu�ݒ�(�X�^�[�g�ʒu)
	SimpleMath::Vector3 eye(/*20.0f, 8.0f, 0.0f*/eyePos);

	// �����_��(0,0,0)�ŃJ������Y����]������
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
/// �f�o�b�O�p�J����
/// </summary>
void GameCamera::DebugCamera(DirectX::SimpleMath::Vector3 debugPos)
{
	// ���΃��[�h�Ȃ牽�����Ȃ�
	if (InputManager::SingletonGetInstance().GetMouseState().positionMode == Mouse::MODE_RELATIVE) return;

	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �}�E�X�̍��{�^���������ꂽ
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		// �}�E�X�̍��W���擾
		m_mousePos.x = float(InputManager::SingletonGetInstance().GetMouseState().x);
		m_mousePos.y = float(InputManager::SingletonGetInstance().GetMouseState().y);
	}
	else if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		// ���݂̉�]��ۑ�
		m_angle.x = m_angleTmp.x;
		m_angle.y = m_angleTmp.y;
	}
	// �}�E�X�̃{�^����������Ă�����J�������ړ�������
	if (InputManager::SingletonGetInstance().GetMouseState().leftButton)
	{
		Motion(InputManager::SingletonGetInstance().GetMouseState().x, InputManager::SingletonGetInstance().GetMouseState().y);
	}

	// �}�E�X�̃t�H�C�[���l���擾
	m_scrollWheelValue = InputManager::SingletonGetInstance().GetMouseState().scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		Mouse::Get().ResetScrollWheelValue();
	}

	// �r���[�s����Z�o����
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
/// �����Ă���Ƃ��̃v���C���[�ڐ��J����
/// </summary>
/// <param name="target">�����_</param>
/// <param name="direction">�v���C���[�̌����Ă������</param>
void GameCamera::RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// �T�C���g���ϓ����邽�߂̒l
	static float wave;
	wave += 0.1f;
	// ���������̃J�����̗h��
	float horizontalAxis = sin(wave) * 2.0f / 500.0f;
	// ���������̃J�����̗h��
	float verticalAxis = (sin(wave) + 1.0f) / 500.0f;

	// ���_�ݒ�
	SimpleMath::Vector3 eye(/*horizontalAxis*/0.0f, /*verticalAxis*/0.0f, -0.1f);

	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(direction);
	eye = SimpleMath::Vector3::Transform(eye, rotY);
	eye += target;

	// ��Ԃ���ꍇ
	//SetPositionTarget(eye, target);

	// ��Ԃ��Ȃ��ꍇ
	m_eyePt = eye;
	m_targetPt = target;
}

/// <summary>
/// ��납��ǂ�������J����
/// </summary>
/// <param name="target">�����_</param>
/// <param name="direction">�v���C���[�̌����Ă������</param>
void GameCamera::FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// ���_�ݒ�
	SimpleMath::Vector3 eye(0.0f, 0.3f, -4.0f);

	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(direction);
	eye = SimpleMath::Vector3::Transform(eye, rotY);
	eye += target;

	// ��Ԃ���ꍇ
	//SetPositionTarget(eye, target);

	// ��Ԃ��Ȃ��ꍇ
	m_eyePt = eye;
	m_targetPt = target;
}

/// <summary>
/// ��_�J����
/// </summary>
/// <param name="target">�����_</param>
/// <param name="cameraPoint">�J�����̈ʒu</param>
void GameCamera::FixedPointCamera(DirectX::SimpleMath::Vector3 &target, DirectX::SimpleMath::Vector3 &cameraPoint)
{
	m_eyePt = cameraPoint;
	m_targetPt = target;
}

/// <summary>
/// ��_����̈�̃T�C�Y���̒��𗬂��悤�ɓ����J����
/// </summary>
/// <param name="basePoint">��_</param>
/// <param name="width">��(X)</param>
/// <param name="height">����(Y)</param>
/// <param name="depth">���s(Z)</param>
/// <param name="areaSize">�̈�̃T�C�Y</param>
void GameCamera::FadeCamera(DirectX::SimpleMath::Vector3 basePoint, float width, float height, float depth, float areaSize)
{
	// �J�����̏����ʒu�ݒ�
	SimpleMath::Vector3 eye(20.0f, 40.0f, 0.0f);

	// ��]
	m_aroundAngleX += 0.5f;
	m_aroundAngleY += 0.5f;
	// ��]����
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
/// �}�E�X�Ŏ��_�ړ�����J����
/// </summary>
void GameCamera::MouseOperateCamera(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 playerDirction, const bool isPlayFlag)
{
	if(isPlayFlag == true)
	{
		// �x�N�g���̒��������߂�
		double lengthA = pow((m_cameraDir.x * m_cameraDir.x) + (m_cameraDir.z * m_cameraDir.z), 0.5);
		double lengthB = pow(((playerDirction.x / 2.0f) * (playerDirction.x / 2.0f)) +
			                 ((playerDirction.z / 2.0f) * (playerDirction.z / 2.0f)), 0.5);
		// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
		double cos_sita = m_cameraDir.x * (playerDirction.x / 2.0f) + m_cameraDir.z * (playerDirction.z / 2.0f) / (lengthA * lengthB);

		// cos�Ƃ���Ƃ����߂�
		double sita = acos(cos_sita);
		// �f�O���[�ŋ��߂�
		sita = sita * 180.0 / double(XM_PI);

		// ���̒l�Ȃ�E��]�A���̒l�Ȃ獶��]
		float s = (playerDirction.x / 2.0f) * m_cameraDir.z -
			      (playerDirction.z / 2.0f) * m_cameraDir.x;

		// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����炻�̉�ʂ̑傫�����擾
		RECT activeWndRect;
		HWND activeWnd = GetActiveWindow();
		GetWindowRect(activeWnd, &activeWndRect);

		// �E�B���h�E�̃T�C�Y���擾
		float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
		float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);
		// �^�C�g���o�[�̍������擾
		int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);   

		// ���S���W�ݒ�
		SimpleMath::Vector2 centerPos;
		if(m_isFullScreen)  centerPos = SimpleMath::Vector2((windowWidth * 0.5f),(windowHeight * 0.5f));
		else				centerPos = SimpleMath::Vector2((windowWidth * 0.5f), ((windowHeight + titlebarHeight) * 0.5f));

		// ���S�_����̍��������߂�
		float x = InputManager::SingletonGetInstance().GetMouseState().x - centerPos.x;
		float y = InputManager::SingletonGetInstance().GetMouseState().y - centerPos.y;

		m_rotationX = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.1f, 0.0f, 0.0f), -(y / ROTATE_MAG));
		m_rotationY = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -(x / ROTATE_MAG));

		m_rotationX *= m_toScreenOutRotaX;
		m_rotationY *= m_toScreenOutRotaY;
		
		// ��ʊO�ɏo����J�[�\������ʂ̒��S�ɖ߂�
		if (InputManager::SingletonGetInstance().GetMouseState().x < int((activeWndRect.left - activeWndRect.left) + OUT_SIZE_OF_SCREEN) || 
			InputManager::SingletonGetInstance().GetMouseState().x > int((activeWndRect.right - activeWndRect.left) - OUT_SIZE_OF_SCREEN) ||
			InputManager::SingletonGetInstance().GetMouseState().y < int((activeWndRect.top - activeWndRect.top) + OUT_SIZE_OF_SCREEN) || 
			InputManager::SingletonGetInstance().GetMouseState().y > int((activeWndRect.bottom - activeWndRect.top) - OUT_SIZE_OF_SCREEN))
		{
			// ���݂̉�]��ۑ�
			m_toScreenOutRotaX = m_rotationX;
			m_toScreenOutRotaY = m_rotationY;

			// �X�N���[���ɉ����ĉ�ʂ̒��S�ɖ߂�
			if (m_isFullScreen) SetCursorPos(int(activeWndRect.left + centerPos.x), int(activeWndRect.top + centerPos.y));
			else				SetCursorPos(int(activeWndRect.left + centerPos.x), int(activeWndRect.top + centerPos.y + titlebarHeight));
		}
		
		// ���_�A������ݒ�
		SimpleMath::Vector3 eye(0.0f, 0.0f, 0.1f);
		eye = SimpleMath::Vector3::Transform(eye, (m_rotationX * m_rotationY));
		eye += target;

		// ���_�A�����_����
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
		// ���_�ݒ�
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
/// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ� (���Βl)
/// </summary>
/// <param name="x">���݂̃|�C���^��X���W</param>
/// <param name="y">���݂̃|�C���^��Y���W</param>
void GameCamera::Motion(int x, int y)
{
	// ���ΓI�Ȉʒu���Z�o
	float dragPosX = (x - m_mousePos.x) * m_dragUnit.x;
	float dragPosY = (y - m_mousePos.y) * m_dragUnit.y;

	// �N���b�N�������W�ƈقȂ��Ă�����
	if (dragPosX != 0.0f || dragPosY != 0.0f)
	{
		// ��]�ʂ�ݒ�
		float yAngle = dragPosX * XM_PI;
		float xAngle = dragPosY * XM_PI;

		// ���݂̉�]�ʂɉ�����
		m_angleTmp.x = m_angle.x + xAngle;
		m_angleTmp.y = m_angle.y + yAngle;
	}
}

/// <summary>
/// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
/// </summary>
/// <param name="windowWidth">��ʃT�C�Y(����)</param>
/// <param name="windowHeight">��ʃT�C�Y(�c��)</param>
void GameCamera::SetWindowSize(int windowWidth, int windowHeight)
{
	m_dragUnit.x = (1.0f / float(windowWidth)) / 5.0f;
	m_dragUnit.y = (1.0f / float(windowHeight)) / 5.0f;
}
