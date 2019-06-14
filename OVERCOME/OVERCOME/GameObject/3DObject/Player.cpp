//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "Player.h"
#include "GameRoad.h"

#include "../../Utility/InputManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int Player::SPAWNTIME = 600;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
Player::Player()
	: m_pos(0.0f, 2.0f, 0.0f), m_vel(0.0f, 0.0f, 0.0f), m_dir(0.0f, 0.0f, 0.0f),
	  m_height(0.0f), m_jumpForce(0.0f), m_gravity(0.0f), m_posTmp(0.0f, 0.0f, 0.0f),
	  m_playStartFlag(false), m_playStartTime(0),
	  m_restartFlag(false), m_restartTime(0),
	  m_spawnFlag(false), m_spawnElapsedTime(0), m_assaultPoint(0),
	  m_passedRoadPos(0.0f, 0.0f), m_nextPos(0.0f, 0.0f), m_velFlag(false),
	  m_world(SimpleMath::Matrix::Identity),
	  mp_bulletManager(nullptr), mp_gameCamera(nullptr), mp_gameRoad(nullptr),
	  m_posRestartUI(0.0f, 0.0f), m_textureRestart(nullptr),
	  m_posCountUI(0.0f, 0.0f), m_textureCount(nullptr),
	  m_texturePointer(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	delete mp_bulletManager;
	mp_bulletManager = nullptr;
}

/// <summary>
/// ����������
/// </summary>
void Player::Initialize()
{
	// �e�ϐ�������
	m_height       = 1.75f;                                       // �v���C���[���g�̍���
	m_jumpForce    = 0.0f;                                        // �W�����v��
	m_gravity      = 0.1f;                                        // �d��

	m_world = SimpleMath::Matrix::Identity;                       // ���[���h�s��

	// ���I�u�W�F�N�g�̏�����
	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
	mp_bulletManager = new GameBulletManager();
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

	// ���X�^�[�gUI�̐ݒ�
	m_posRestartUI = SimpleMath::Vector2(175.0f, 450.0f);
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\clicktocenter.png", nullptr, m_textureRestart.GetAddressOf());

	// �J�E���g�����̐ݒ�
	m_posCountUI = SimpleMath::Vector2(360.0f, 260.0f);
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_len.png", nullptr, m_textureCount.GetAddressOf());
	
	// �|�C���^�[�̐ݒ�
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\pointer.png", nullptr, m_texturePointer.GetAddressOf());

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool Player::Update(DX::StepTimer const & timer)
{
	// �J�����̍X�V
	mp_gameCamera->Update(timer,this);

	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �}�E�X�������ʒu�ɂ��Ă���̃J�E���g�_�E��
	if (!m_playStartFlag)
	{
		if (mp_gameCamera->GetStartPosMouse())
		{
			m_playStartTime++;
		}

		if (m_playStartTime > 180)
		{
			m_playStartTime = 0;
			m_playStartFlag = true;
			// �}�E�X�J�[�\���̔�\��
			ShowCursor(FALSE);
		}
	}
	
	// �Q�[���X�^�[�g���Ă���̍s��
	if (m_playStartFlag)
	{
		// �e�̍X�V
		mp_bulletManager->Update(timer, m_pos, mp_gameCamera->GetCameraAngle());

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
			if (abs(m_pos.x - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x) < 0.01f &&
				abs(m_pos.z - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z) < 0.01f &&
				m_spawnFlag == false)
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
							}
						}
					}
				}

				if (mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).roadType == 3 && m_velFlag == true)
				{
					m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
					m_spawnFlag = true;
					m_assaultPoint = mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).rotaAngle;
				}
				if (mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).roadType != 3)
				{
					m_velFlag = false;
				}
			}
			else if (m_spawnFlag)
			{
				m_spawnElapsedTime++;
				if (m_velFlag == true && m_spawnElapsedTime > SPAWNTIME)
				{
					m_spawnElapsedTime = 0;
					m_velFlag = false;
					m_spawnFlag = false;
				}
			}
			else if(!m_velFlag)
			{
				// ���̍s����ɋ߂Â��܂ō����ňړ���������
				m_vel.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x;
				m_vel.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z;

				m_vel.Normalize();
				m_vel.x /= 10.0f;
				//if (m_vel.x > 1.0f || m_vel.x < -1.0f)m_vel.x = 0.0f;
				m_vel.y = 0.0f;
				m_vel.z /= 10.0f;
				//if (m_vel.z > 1.0f || m_vel.z < -1.0f)m_vel.z = 0.0f;

				m_velFlag = true;
			}
		}
		
		// ���X�^�[�g�̏���
		/*if (m_restartFlag)
		{
			m_restartTime++;
		}
		if (m_restartTime > 180)
		{
			SimpleMath::Vector3 pos = m_pos;

			m_pos.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x;
			m_pos.y = 1.0f;
			m_pos.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z;

			m_vel = pos - m_pos;
			m_vel.Normalize();
			m_vel.x /= 10.0f;
			m_vel.y = 0.0f;
			m_vel.z /= 10.0f;

			m_restartTime = 0;
			m_restartFlag = false;
			m_velFlag = false;
		}*/

	}

	// �X�^�[�g����܂ł͏����ʒu�ŌŒ�
	if(!mp_gameCamera->GetStartPosMouse())
	{
		const int OFFSETNUM = 8;
		// ����̃}�X�̎��͔��}�X���̑��΍��W
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },{ 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		// �����ʒu�ŌŒ�
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
				}
			}
		}
	}
	
	// �ړ��O�̍��W���L��
	m_posTmp = m_pos;
	// �v���C���[�ړ�(���W)
	m_pos += m_vel;
	// �ړ���̍��W�Ƃ̕΍�����ړ��������Z�o
	m_dir = m_pos - m_posTmp;
	//m_dir.Normalize();

	if (m_vel != SimpleMath::Vector3::Zero)
	{
		m_assaultPoint = 0;
	}

	// ���[���h�s��̍쐬
	m_world = SimpleMath::Matrix::CreateTranslation(m_pos);

	// �Փ˔���p�̉��z�I�u�W�F�N�g����
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // ���E���̒��S
	box.r = DirectX::SimpleMath::Vector3(1.0f, m_height / 2.0f, 1.0f);                        // �e���a
	SetCollision(box);

	return true;
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Render(MatrixManager* matrixManager)
{
	// �v���C���[�̕`��
	//m_modelPlayer->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
	//	           m_world, matrixManager->GetView(), matrixManager->GetProjection());

	mp_bulletManager->Render(matrixManager);

	// �X�^�[�g�J�E���g�̕`��
	if (!m_playStartFlag)
	{
		if (mp_gameCamera->GetStartPosMouse())
		{
			// �؂���ꏊ��ݒ�
			RECT rect;
			rect.top = LONG(0.0f);
			rect.left = LONG((2 * COUNTUISIZE) - (m_playStartTime/60 * COUNTUISIZE));
			rect.right = LONG(2 * COUNTUISIZE) - (m_playStartTime / 60 * COUNTUISIZE) + COUNTUISIZE;
			rect.bottom = LONG(COUNTUISIZE);

			DrawManager::SingletonGetInstance().DrawRect(m_textureCount.Get(), m_posCountUI, &rect);
		}
	}
	// ���X�^�[�g�J�E���g�̕`��
	if (m_restartFlag)
	{
		// ���ꏊ��ݒ�
		RECT rect;
		rect.top = LONG(0.0f);
		rect.left = LONG((2 * COUNTUISIZE) - (m_restartTime / 60 * COUNTUISIZE));
		rect.right = LONG(2 * COUNTUISIZE) - (m_restartTime / 60 * COUNTUISIZE) + COUNTUISIZE;
		rect.bottom = LONG(COUNTUISIZE);

		DrawManager::SingletonGetInstance().DrawRect(m_textureCount.Get(), m_posCountUI, &rect);
	}

	if (!m_playStartFlag)
		DrawManager::SingletonGetInstance().Draw(m_textureRestart.Get(), m_posRestartUI);

	// �|�C���^�[�̕`��
	if (mp_gameCamera->GetStartPosMouse() && m_playStartFlag)
	{
		DrawManager::SingletonGetInstance().Draw(m_texturePointer.Get(), SimpleMath::Vector2(350.0f, 250.0f));
	}
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
