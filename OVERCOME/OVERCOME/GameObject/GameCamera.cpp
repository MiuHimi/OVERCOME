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

#include "../ExclusiveGameObject/InputManager.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameCamera::GameCamera() :m_angle(0.0f)
{
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
		Vector3 target(player->GetPos());
		target.y += player->GetHeight();
		RunPlayerCamera(target, player->GetDirection());
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
	m_angle += 0.5f;
	Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));
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
	static float waveV;
	waveV += 0.1f;
	// ���������̃J�����̗h��
	float horizontalAxis = sin(wave) * 2.0f / 500.0f;
	float verticalAxis = (sin(wave) + 1.0f) / 500.0f;

	// ���_�ݒ�
	Vector3 eye(horizontalAxis/*0.0f*/, /*verticalAxis*/0.0f, -0.1f);

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
