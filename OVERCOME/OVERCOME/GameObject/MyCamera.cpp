//////////////////////////////////////////////////////////////
// File.    MyCamera.cpp
// Summary. MyCameraClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �w�b�_���C���N���[�h
#include "../pch.h"
#include "MyCamera.h"

#include "../ExclusiveGameObject/InputManager.h"

#include <math.h>
#include <cmath>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
MyCamera::MyCamera():m_angle(0.0f)
{
	mp_player = new Player();
	//mp_player = Player::GetPlayer();
	mp_player->Initialize();
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
MyCamera::~MyCamera()
{
	delete mp_player;
	mp_player = nullptr;
}

/// <summary>
/// �X�V
/// </summary>
bool MyCamera::Update(DX::StepTimer const & timer, Player* player)
{
	// �v���C���[�����X�V
	//mp_player->Update(timer);

	static bool cameraFlag = true;
	/*Vector3 target(player->GetPos());
	target.y += player->GetHeight();
	RunPlayerCamera(target, player->GetDirection());*/
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
/// �^�C�g���V�[���p�J����
/// </summary>
void MyCamera::TitleSceneCamera()
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
/// �Q�[���V�[���p�J����
/// </summary>
void MyCamera::GameSceneCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	Vector3 eye(0.0f, 3.0f, -5.0f);

	Matrix rotY = Matrix::CreateRotationY(direction);
	eye = Vector3::Transform(eye, rotY);
	eye += target;

	// ��Ԃ���ꍇ
	SetPositionTarget(eye, target);

	// ��Ԃ��Ȃ��ꍇ
	//m_eyePt = eye;
	//m_targetPt = target;

	/*Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));*/
}

/// <summary>
/// �����Ă���Ƃ��̃v���C���[�ڐ��J����
/// </summary>
/// <param name="target">�����_</param>
/// <param name="direction">�v���C���[�̌����Ă������</param>
void MyCamera::RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
	// �T�C���g���ϓ����邽�߂̒l
	static float wave;
	wave += 0.01f;
	float sinWave = sin(wave + (0.5f));
	// // sin�g�̒l = sin(��t���[���ŕϓ�����l+(��������ꍇ�A���~n�ł��ꂼ������炷���Ƃ��ł���)) * �ӂ蕝(�ǂ�قǂ̕��ŏ㉺or���E�ړ����邩)

	Vector3 eye(0.0f, 0.0f, -0.1f);

	/*m_count += 0.01f;
	Vector3 targetPos(target);
	targetPos.x = sin(2 * XM_PI * 2)30 + sin(XM_PI * 2 / 30 * m_count);
	if (m_count > 0.1f || m_count < -0.1f) m_count *= -1;
	targetPos.x += m_count;*/
	
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
void MyCamera::FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction)
{
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
