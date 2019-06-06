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

// constant�f�B���N�g��
const float GameCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;
const float GameCamera::ROTATE_MAG = 300.0f;

SceneId SceneManager::m_activeScene;

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
GameCamera::~GameCamera()
{
}

/// <summary>
/// �X�V(��ɃJ�����̐؂�ւ����s��)
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <param name="player">�v���C���[���</param>
/// <returns>�I�����</returns>
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
		// �}�E�X����̃J����
		target = player->GetPos();
		target.y += player->GetHeight();
		// �����_�̓v���C���[�̖ڐ��̈ʒu
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
/// ���_�𒍎��_�ɂ��A��������񂷂�J����
/// </summary>
void GameCamera::OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos)
{
	// �J�����̈ʒu�ݒ�(�X�^�[�g�ʒu)
	SimpleMath::Vector3 eye(/*20.0f, 8.0f, 0.0f*/eyePos);

	// �����_��(0,0,0)�ŃJ������Y����]������
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
/// �}�E�X�Ŏ��_�ړ�����J����
/// </summary>
void GameCamera::MouseOperateCamera(DirectX::SimpleMath::Vector3 target, Player* player)
{
	RECT desktopWndRect;                         // �f�X�N�g�b�v�̃T�C�Y
	HWND desktopWnd = GetDesktopWindow();        // ���̊֐��Ńf�X�N�g�b�v�̃n���h�����擾
	GetWindowRect(desktopWnd, &desktopWndRect);  // �f�X�N�g�b�v�̃n���h�������ʂ̑傫�����擾

	RECT activeWndRect;                          // �A�N�e�B�u�ȃE�B���h�E�̃T�C�Y
	HWND activeWnd = GetActiveWindow();          // ���̊֐��ŃA�N�e�B�u�ȃE�B���h�E�̃n���h�����擾
	GetWindowRect(activeWnd, &activeWndRect);    // �A�N�e�B�u�ȃE�B���h�E�̃n���h�����炻�̉�ʂ̑傫�����擾

	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);   // �^�C�g���o�[�̍������擾

	// ���΃��[�h�Ȃ牽�����Ȃ�
	if (InputManager::SingletonGetInstance().GetMouseState().positionMode == Mouse::MODE_RELATIVE) return;
	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �}�E�X�|�C���^�[�Ɖ~�̏Փ˔���
	float distX = 400.0f - float(InputManager::SingletonGetInstance().GetMouseState().x);
	float distY = 300.0f - float(InputManager::SingletonGetInstance().GetMouseState().y);
	float distX2 = distX * distX;
	float distY2 = distY * distY;
	float r = 100.0f;
	float r2 = r * r;
	
	// �}�E�X�|�C���^�[����ʒ����ɗ����王�_�ړ����J�n����
	if (distX2 + distY2 <= r2 &&
		InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_checkMousePos = true;
	}

	if(m_checkMousePos == true)
	{
		// �x�N�g���̒��������߂�
		double lengthA = pow((m_cameraDir.x * m_cameraDir.x) + (m_cameraDir.z * m_cameraDir.z), 0.5);
		double lengthB = pow(((player->GetDir().x / 2.0f) * (player->GetDir().x / 2.0f)) +
			                 ((player->GetDir().z / 2.0f) * (player->GetDir().z / 2.0f)), 0.5);
		// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
		double cos_sita = m_cameraDir.x * (player->GetDir().x / 2.0f) + m_cameraDir.z * (player->GetDir().z / 2.0f) / (lengthA * lengthB);

		// cos�Ƃ���Ƃ����߂�
		double sita = acos(cos_sita);
		// �f�O���[�ŋ��߂�
		sita = sita * 180.0 / double(XM_PI);

		// ���̒l�Ȃ�E��]�A���̒l�Ȃ獶��]
		float s = (player->GetDir().x / 2.0f) * m_cameraDir.z -
			      (player->GetDir().z / 2.0f) * m_cameraDir.x;

		// ��]����
		// �i�s�������20�x�ȏ������Ă����琧���͈͓��ɖ߂�
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

		// ���S���W(���Βl)�ݒ�
		SimpleMath::Vector2 centerPos(400.0f, 300.0f);

		// ���S�_����̍��������߂�
		float x = InputManager::SingletonGetInstance().GetMouseState().x - centerPos.x;
		float y = InputManager::SingletonGetInstance().GetMouseState().y - centerPos.y;

		m_rotationX = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.1f, 0.0f, 0.0f), -(y / ROTATE_MAG));
		m_rotationY = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -(x / ROTATE_MAG));

		m_rotationX *= m_toScreenOutRotaX;
		m_rotationY *= m_toScreenOutRotaY;

		/*if (sita <= 90)
		{
			// �����͈͊O��������΍����̉�]
			m_rotationX = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.1f, 0.0f, 0.0f), -(y / ROTATE_MAG));
			m_rotationY = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), -(x / ROTATE_MAG));

			// �J�[�\�����E�B���h�E�O�ɍs�����Ƃ��͌��X��]���Ă���������]������
			m_rotationX *= m_toScreenOutRotaX;
			m_rotationY *= m_toScreenOutRotaY;

			// �����͈͓��ɂ������Ƃ��p�ɉ�]��ۑ�
			m_rotationTmpX = m_rotationX;
			m_rotationTmpY = m_rotationY;
		}
		else
		{
			// �����͈͓���������ۑ����Ă��������̉�]�̂ݍs��
			m_rotationX = m_rotationTmpX;
			m_rotationY = m_rotationTmpY;

			// �J�[�\�����E�B���h�E�O�ɍs�����Ƃ��͌��X��]���Ă���������]������
			m_rotationX *= m_toScreenOutRotaX;
			m_rotationY *= m_toScreenOutRotaY;
		}*/

		/*if (sita > 90.0f)
		{
			m_rotationY *= m_reformRota;
		}*/
		
		// ��ʊO�ɏo����J�[�\������ʂ̒��S�ɖ߂�
		if (InputManager::SingletonGetInstance().GetMouseState().x < 50 || InputManager::SingletonGetInstance().GetMouseState().x > 750 ||
			InputManager::SingletonGetInstance().GetMouseState().y < 50 || InputManager::SingletonGetInstance().GetMouseState().y > 550)
		{
			// ���݂̉�]��ۑ�
			m_toScreenOutRotaX = m_rotationX;
			m_toScreenOutRotaY = m_rotationY;

			// �f�X�N�g�b�v�̒l�̂��߁A�E�B���h�E���̃T�C�Y+��ʂ̔����𑫂�(Y�̓^�C�g���o�[��������)
			SetCursorPos(int(activeWndRect.left + centerPos.x), int(activeWndRect.top + centerPos.y + titlebarHeight));
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
