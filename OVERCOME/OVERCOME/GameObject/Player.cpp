//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "Player.h"
#include "GameRoad.h"

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
Player::Player()
	: m_pos(0.0f, 2.0f, 0.0f),
	  m_vel(0.0f, 0.0f, 0.0f),
	  mp_gameCamera(nullptr),
	  mp_bulletManager(nullptr),
	  mp_gameRoad(nullptr),
	  m_restartFlag(false),
	  m_passedRoadPos(0.0f, 0.0f),
	  m_nextPos(0.0f, 0.0f),
	  m_ahead(0.0f, 0.0f, 0.0f),
	  m_velFlag(false)
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
	m_direction    = XMConvertToRadians(0.0f);                    // ����(�p�x)
	m_rotation     = DirectX::SimpleMath::Quaternion::Identity;   // ��]
	m_height       = 1.75f;                                       // �v���C���[���g�̍���
	m_jumpForce    = 0.0f;                                        // �W�����v��
	m_gravity      = 0.03f;                                       // �d��
	m_fallingPower = 0.0f;                                        // ���̂܂ܗ����鎞�̗�

	m_world = SimpleMath::Matrix::Identity;                       // ���[���h�s��

	// �v���C���[�̐i�s�x�N�g���̐ݒ�
	m_ahead = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -0.01f);
	m_ahead.Normalize();

	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
	mp_bulletManager = /*std::make_unique<GameBulletManager>();*/new GameBulletManager();
	mp_bulletManager->Initialize();
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	mp_gameRoad->Create();
	
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
	mp_gameRoad->Update(timer);

	mp_gameCamera->Update(timer,this);

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
		const int OFFSETNUM = 8;
		// ����̃}�X�̎��͔��}�X���̑��΍��W
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },{ 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};
		
		SimpleMath::Vector2 nowPos = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);
		if(abs(m_pos.x - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x) < 0.01f &&
		   abs(m_pos.z - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z) < 0.01f)
		{
			m_pos.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x;
			m_pos.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z;

			if (abs(m_vel.x) > abs(m_vel.z))
			{
				if (m_vel.x < 0.00f)
					m_passedRoadPos.x = m_nextPos.x + 1;
				else if (m_vel.x > 0.00f)
					m_passedRoadPos.x = m_nextPos.x - 1;
				m_passedRoadPos.y = m_nextPos.y;
			}
			else if (abs(m_vel.x) < abs(m_vel.z))
			{
				if (m_vel.z < 0.00f)
					m_passedRoadPos.y = m_nextPos.y + 1;
				else if (m_vel.z > 0.00f)
					m_passedRoadPos.y = m_nextPos.y - 1;
				m_passedRoadPos.x = m_nextPos.x;
			}
			
			SimpleMath::Vector2 now = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);

			// ���̍s���������
			float distTmp = 0.0f;
			for (int k = 0; k < OFFSETNUM; k++)
			{
				SimpleMath::Vector2 pos = nowPos;
				pos += OFFSET[k];

				if (distTmp == 0.0f)
				{
					// ���̍s����o�^
					distTmp = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
						(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);
				}

				if (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).roadType != 0)
				{
					float dist = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
						(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);

					if (dist <= distTmp)
					{
						int pX = (int)m_passedRoadPos.x;
						int pY = (int)m_passedRoadPos.y;
						SimpleMath::Vector2 p = SimpleMath::Vector2((float)pX, (float)pY);
						
						// ���ɒʉ߂����Ƃ��낶��Ȃ���Ύ��̈ړ���Ɍ���
						if (p != pos && 
							nowPos != pos)
						{
							m_nextPos = pos;

							distTmp = dist;

							//m_passedRoadPos = pos;
						}
					}
				}
			}
			m_velFlag = false;
		}
		else
		{
			if (!m_velFlag)
			{
				// ���̍s����ɋ߂Â��܂ō����ňړ���������
				m_vel.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x;
				m_vel.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z;

				m_vel.Normalize();
				m_vel.x /= 10.0f;
				m_vel.y = 0.0f;
				m_vel.z /= 10.0f;

				m_velFlag = true;
			}

			// �ʉߍς݂̓��H���L��
			//m_passedRoadPos = m_nextPos;
			
		}
	}
	else
	{
		const int OFFSETNUM = 8;
		// ����̃}�X�̎��͔��}�X���̑��΍��W
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },{ 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		// �}�E�X�ŃN���b�N����܂ł͏����ʒu�ŌŒ�
		if (mp_gameRoad->GetPosType(mp_gameRoad->START).x != m_pos.x ||
			mp_gameRoad->GetPosType(mp_gameRoad->START).y != m_pos.z)
		{
			m_pos.x = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.x;
			m_pos.z = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.z;
		}

		// �ʉߍς݂̓��H���L��
		m_passedRoadPos = SimpleMath::Vector2(mp_gameRoad->GetPosType(mp_gameRoad->START).x,
											  mp_gameRoad->GetPosType(mp_gameRoad->START).y);

		// ���̍s���������
		float distTmp = 0.0f;
		for (int k = 0; k < OFFSETNUM; k++)
		{
			SimpleMath::Vector2 pos = SimpleMath::Vector2(m_passedRoadPos.x, m_passedRoadPos.y);
			pos += OFFSET[k];

			if (k == 0)
			{
				m_nextPos = pos;

				// ���̍s����o�^
				distTmp = (mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x) +
					(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z);
			}

			if (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).roadType != 0)
			{
				float dist = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
					(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);

				if (dist < distTmp)
				{
					// ���ɒʉ߂����Ƃ��낶��Ȃ���Ύ��̈ړ���Ɍ���
					if (m_passedRoadPos != pos) m_nextPos = pos;

					distTmp = dist;

					// �ʉߍς݂̓��H���L��
					//m_passedRoadPos = pos;
				}
			}
		}
	}

	// ���ƐڐG����Ǝ~�܂�(�^�񒆂��N���b�N����܂�)
	/*if (m_collideToFloor)
	{
		m_restartFlag = true;
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	}*/

	
	// �v���C���[�̌����ݒ�
	if (InputManager::SingletonGetInstance().GetKeyState().A)
	{
		m_direction += XMConvertToRadians(2.0f);
	}
	if (InputManager::SingletonGetInstance().GetKeyState().D)
	{
		m_direction -= XMConvertToRadians(2.0f);
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
