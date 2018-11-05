//////////////////////////////////////////////////////////////
// File.    GameCamera.cpp
// Summary. GameCameraClass
// Date.    2018/10/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <math.h>
#include <cmath>

#include "../pch.h"
#include "GameCamera.h"

#include "../Utility/InputManager.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;

const float GameCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;

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
	m_mousePos(0.0f, 0.0f),
	m_dragUnit(0.0f, 0.0f),
	m_scrollWheelValue(0)
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
	// �J�����̐؂�ւ������߂�t���O
	static bool cameraFlag = true;

	// B�L�[�ŃJ�����؂�ւ�(��)
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

	// �t���O�ɉ������J�����Ǘ�
	if (cameraFlag)
	{
		//Vector3 target(player->GetPos());
		//target.y += player->GetHeight();
		//RunPlayerCamera(target, player->GetDirection());

		Vector3 target(player->GetPos());
		target.y += player->GetHeight();
		MouseOperateCamera(target, player->GetDirection());
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
/// ���_�𒍎��_�ɂ��A��������񂷂�J����
/// </summary>
void GameCamera::OriginPointAroundCamera()
{
	// �J�����̈ʒu�ݒ�(�X�^�[�g�ʒu)
	Vector3 eye(20.0f, 8.0f, 0.0f);

	// �����_��(0,0,0)�ŃJ������Y����]������
	m_aroundAngle += 0.5f;
	Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_aroundAngle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));
}
/// <summary>
/// �f�o�b�O�p�J����
/// </summary>
void GameCamera::DebugCamera()
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
	Vector3 eye(/*horizontalAxis*/0.0f, /*verticalAxis*/0.0f, -0.1f);

	Matrix rotY = Matrix::CreateRotationY(direction);
	eye = Vector3::Transform(eye, rotY);
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
	Vector3 eye(0.0f, 0.3f, -4.0f);

	Matrix rotY = Matrix::CreateRotationY(direction);
	eye = Vector3::Transform(eye, rotY);
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
void GameCamera::MouseOperateCamera(DirectX::SimpleMath::Vector3 target, float direction)
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

	
	// �r���[�s����Z�o����
	Matrix rotY = Matrix::CreateRotationY(-m_angleTmp.y + direction);
	Matrix rotX = Matrix::CreateRotationX(-m_angleTmp.x);

	Matrix rt = rotY * rotX;

	Vector3 eye(0.0f, 0.0f, -0.1f);
	//Vector3 target(target);
	Vector3 up(0.0f, 1.0f, 0.0f);

	eye = Vector3::Transform(eye, rt);
	eye += target;
	up = Vector3::Transform(up, rt.Invert());

	m_eyePt = eye;
	m_targetPt = target;
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
