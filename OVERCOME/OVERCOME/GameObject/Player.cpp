//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "Player.h"

#include "../Utility/InputManager.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"
#include "../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
Player::Player() : m_pos(22.5f, 2.0f, 7.5f),
	               m_vel(0.0f, 0.0f, 0.0f),
	               mp_gameCamera(nullptr),
	               mp_bulletManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	//Player::Depose();
}

/// <summary>
/// ����������
/// </summary>
void Player::Initialize()
{
	// �e�ϐ�������
	m_accel        = 0.0f;                                        // �����x
	m_direction    = XMConvertToRadians(0.0f);                    // ����(�p�x)
	m_rotation     = DirectX::SimpleMath::Quaternion::Identity;   // ��]
	m_height       = 1.75f;                                       // �v���C���[���g�̍���
	m_jumpForce    = 0.0f;                                        // �W�����v��
	m_gravity      = 0.03f;                                       // �d��
	m_fallingPower = 0.0f;                                        // ���̂܂ܗ����鎞�̗�

	m_world = SimpleMath::Matrix::Identity;                       // ���[���h�s��

	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
	mp_bulletManager = /*std::make_unique<GameBulletManager>();*/new GameBulletManager();
	mp_bulletManager->Initialize();
}
/// <summary>
/// ��������
/// </summary>
void Player::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelPlayer = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\player.cmo", fx);

	// �v���C���[�̍쐬
	SetModel(m_modelPlayer.get());

	mp_bulletManager->Create();
	
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool Player::Update(DX::StepTimer const & timer)
{
	mp_gameCamera->Update(timer,this);

	m_vel = DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_accel);

	if (mp_gameCamera->GetStartPosMouse())
	{
		debugFlag = true;
	}
	if(debugFlag)
	{
		mp_bulletManager->Update(timer, m_pos, mp_gameCamera->GetCameraAngle());
	}

	// �v���C���[�ړ�(�x�N�g��)
	
	
	if (mp_gameCamera->GetStartPosMouse())
	{
		// �����x�ݒ�
		m_accel -= 0.01f;
		if (m_accel < -0.05f)
		{
			m_accel = -0.05f;
		}
		// ���x������
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, -0.1f + m_accel);
	}
	if (InputManager::SingletonGetInstance().GetKeyState().S/*mp_gameCamera->GetStartPosMouse()*/)
	{
		// �����x�ݒ�
		//m_accel += 0.001f;
		// ���x������
		m_vel = /*SimpleMath::Vector3(0.0f, 0.0f, 0.1f + m_accel);*/SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	}

	// �v���C���[�̌����ݒ�
	if (InputManager::SingletonGetInstance().GetKeyState().A)
	{
		m_direction += XMConvertToRadians(2.0f);
	}
	if (InputManager::SingletonGetInstance().GetKeyState().D)
	{
		m_direction -= XMConvertToRadians(2.0f);
	}
	
	
	if (InputManager::SingletonGetInstance().GetKeyState().Up == false &&
		InputManager::SingletonGetInstance().GetKeyState().Down == false &&
		m_vel.z < 0.1f && m_vel.z > -0.1f)
	{
		// �ړ����x������ꍇ�͒�~����
		m_vel.z = 0.0f;
	}

	// �W�����v
	if (InputManager::SingletonGetInstance().GetKeyState().W && m_isJump == false ||
		InputManager::SingletonGetInstance().GetKeyState().W && m_collideToRoad == true && m_pos.y >= 0.95f)
	{
		m_isJump = true;
		m_vel.y = 0.0f;
		m_jumpForce = 0.5f;
	}

	// �W�����v�͒���
	if (m_isJump == true && m_collideToRoad == false || m_isJump == true && m_collideToRoad == true && m_pos.y >= 0.85f)
	{
		m_vel.y += m_jumpForce;
		if (m_vel.y < -1.0f) m_vel.y = -1.0f;
		m_jumpForce -= m_gravity;
	}
	else if (m_isJump == true && m_collideToRoad == true)
	{
		m_vel.y = 0.0f;
	}

	// ���̃I�u�W�F�N�g�ɂ��G�ꂸ�A���W�����v�����Ă��Ȃ��Ƃ��̏���
	if (m_isJump == false && m_collideToRoad == false && m_collideToFloor == false && m_noTouchObectFlag == false)
	{
		m_fallingPower = 0;
		m_noTouchObectFlag = true;
	}
	if (m_noTouchObectFlag == true)
	{
		m_fallingPower -= m_gravity;
		m_vel.y += m_fallingPower;
	}

	// ���x���
	if (m_vel.y < -0.3f)
	{
		m_vel.y = -0.3f;
	}

	// �v���C���[�ړ�(���W)
	m_rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), m_direction);
	m_vel = SimpleMath::Vector3::Transform(m_vel, m_rotation);
	m_pos += m_vel;

	// ���[���h�s��̍쐬
	m_world = SimpleMath::Matrix::CreateFromQuaternion(m_rotation) * SimpleMath::Matrix::CreateTranslation(m_pos);

	// �Փ˔���p�̉��z�I�u�W�F�N�g����
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // ���E���̒��S
	box.r = DirectX::SimpleMath::Vector3(0.5f, m_height / 2.0f, 0.5f);                        // �e���a
	SetCollision(box);

	return true;
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Render()
{
	// �v���C���[�̕`��
	//m_modelPlayer->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
	//	           m_world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());

	mp_bulletManager->Render();
}

/// <summary>
/// �폜����
/// </summary>
void Player::Depose()
{
}

/// <summary>
/// �v���C���[�̏���Ԃ�
/// </summary>
/// <returns>�v���C���[���</returns>
Player* Player::GetPlayer()
{
	return this;
}
