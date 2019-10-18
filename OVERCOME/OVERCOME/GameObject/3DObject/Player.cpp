//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "Player.h"
//#include "GameRoad.h"

#include <math.h>

#include "../../Utility/InputManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

#include "../../GameObject/3DObject/GameEnemyManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int Player::SPAWNTIME = 600;
const int Player::COUNT_UI_SIZE = 80;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
Player::Player()
	: m_pos(0.0f, 2.0f, 0.0f), m_vel(0.0f, 0.0f, 0.0f), m_dir(0.0f, 0.0f, 0.0f),
	  m_height(0.0f), m_jumpForce(0.0f), m_gravity(0.0f), m_posTmp(0.0f, 0.0f, 0.0f),
	  m_hp(10), m_hpBasePos{0.0f}, m_damageCount(0), m_isDamaged(false),
	  m_playStartFlag(false), m_moveStartCountDown(0),
	  m_spawnFlag(false), m_spawnElapsedTime(0),
	  m_passedRoadPos(0.0f, 0.0f), m_passingRoadPos(0.0f, 0.0f), m_nextPos(0.0f, 0.0f), m_velFlag(false),
	  m_world(SimpleMath::Matrix::Identity),
	  mp_bulletManager(nullptr), mp_gameRoad(nullptr),
	  m_isFullScreen(false),
	  m_startGudeWave(0.0f), mp_startGuide(nullptr), mp_startCount(nullptr), mp_shootPointer(nullptr), mp_dengerousSign(nullptr),
	  mp_hp(nullptr), mp_damageEffect(nullptr)
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
	m_height       = 1.75f;										// �v���C���[���g�̍���
	m_jumpForce    = 0.0f;										// �W�����v��
	m_gravity      = 0.1f;										// �d��

	m_moveStartCountDown = 180;									// �����o����܂ł̃J�E���g�_�E���p

	m_world = SimpleMath::Matrix::Identity;						// ���[���h�s��

	// ���I�u�W�F�N�g�̏�����
	//mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
	mp_bulletManager = new GameBulletManager();
	mp_bulletManager->Initialize();
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	mp_gameRoad->Create();
	
}
/// <summary>
/// ��������
/// </summary>
void Player::Create(const bool isFulleScreen)
{
	// �G�t�F�N�g�t�@�N�g���[
	//EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	//fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	//m_modelPlayer = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\player.cmo", fx);

	// �v���C���[�̍쐬
	//SetModel(m_modelPlayer.get());

	mp_bulletManager->Create();

	// �X�N���[�����[�h�擾
	m_isFullScreen = isFulleScreen;

	// �A�N�e�B�u�ȃE�B���h�E�̃T�C�Y
	RECT activeWndRect;
	// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����擾
	HWND activeWnd = GetActiveWindow();
	// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����炻�̉�ʂ̑傫�����擾
	GetWindowRect(activeWnd, &activeWndRect);

	// �E�B���h�E�̃T�C�Y���擾
	float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
	float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);
	// �^�C�g���o�[�̍������擾
	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);

	// �X�^�[�g�ē��I�u�W�F�N�g�̐���
	m_startGudeWave = 0.1f;

	mp_startGuide = std::make_unique<Obj2D>();
	mp_startGuide->Create(L"Resources\\Images\\Play\\clicktocenter.png", nullptr);
	mp_startGuide->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 450.0f, 50.0f, 1.0f, 1.0f);
	if (isFulleScreen)
	{
		mp_startGuide->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startGuide->GetWidth()*0.5f),
							 (activeWndRect.bottom - activeWndRect.top) - (mp_startGuide->GetHeight()*2.0f)));
	}
	else
	{
		mp_startGuide->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startGuide->GetWidth()*0.5f),
							 (activeWndRect.bottom - (activeWndRect.top + titlebarHeight)) - (mp_startGuide->GetHeight()*5.0f)));
	}
	mp_startGuide->SetRect(0.0f, 0.0f, mp_startGuide->GetWidth(), mp_startGuide->GetHeight());

	// �X�^�[�g�J�E���g�_�E���I�u�W�F�N�g�̐���
	mp_startCount = std::make_unique<Obj2D>();
	mp_startCount->Create(L"Resources\\Images\\ScoreCount\\count_length.png", nullptr);
	mp_startCount->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(COUNT_UI_SIZE), float(COUNT_UI_SIZE), 1.0f, 1.0f);
	if (isFulleScreen)
	{
		mp_startCount->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startCount->GetWidth()*0.5f),
												  (windowHeight*0.5f) - (mp_startCount->GetHeight()*0.5f)));
	}
	else
	{
		mp_startCount->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_startCount->GetWidth()*0.5f),
												  ((activeWndRect.bottom - (activeWndRect.top + titlebarHeight))*0.5f) - (mp_startCount->GetHeight()*0.5f)));
	}
	mp_startCount->SetRect(0.0f, 0.0f, mp_startCount->GetWidth(), mp_startCount->GetHeight());

	// ���˃|�C���^�[�I�u�W�F�N�g�̐���
	mp_shootPointer = std::make_unique<Obj2D>();
	mp_shootPointer->Create(L"Resources\\Images\\Play\\pointer.png", nullptr);
	mp_shootPointer->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 100.0f, 100.0f, 1.0f, 1.0f);
	if (isFulleScreen)
	{
		mp_shootPointer->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_shootPointer->GetWidth()*0.5f),
													(windowHeight*0.5f) + (windowHeight / 20.0f)));
	}
	else
	{
		mp_shootPointer->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_shootPointer->GetWidth()*0.5f),
													(windowHeight*0.5f) + titlebarHeight + (windowHeight / 20.0f)));
	}
	mp_shootPointer->SetRect(0.0f, 0.0f, mp_shootPointer->GetWidth(), mp_shootPointer->GetHeight());

	// �댯�T�C���I�u�W�F�N�g�̐���
	mp_dengerousSign = std::make_unique<Obj2D>();
	mp_dengerousSign->Create(L"Resources\\Images\\Play\\dangerous_signV.png", nullptr);
	mp_dengerousSign->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 50.0f, 500.0f, 1.0f, 1.0f);
	mp_dengerousSign->SetRect(0.0f, 0.0f, mp_dengerousSign->GetWidth(), mp_dengerousSign->GetHeight());

	// �_���[�W���ʃI�u�W�F�N�g�̐���
	mp_damageEffect = std::make_unique<Obj2D>();
	mp_damageEffect->Create(L"Resources\\Images\\gray.png", nullptr);
	mp_damageEffect->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_damageEffect->SetRect(0.0f, 0.0f, mp_damageEffect->GetWidth(), mp_damageEffect->GetHeight());

	// �̗̓I�u�W�F�N�g�̐���
	mp_hp = std::make_unique<Obj2D>();
	mp_hp->Create(L"Resources\\Images\\Play\\player_hp.png", nullptr);
	mp_hp->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 100.0f, 25.0f, 1.0f, 1.0f);
	mp_hp->SetRect(0.0f, 0.0f, mp_hp->GetWidth(), mp_hp->GetHeight());
	mp_hp->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_hp->GetWidth()*5.0f), windowHeight - (mp_hp->GetHeight()*2.0f)));

	// ��ʒu�ݒ�
	m_hpBasePos = mp_hp->GetPos();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool Player::Update(DX::StepTimer const & timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir)
{
	// �J�����̍X�V
	//mp_gameCamera->Update(timer, m_pos, m_height, m_dir);

	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �X�^�[�g����܂ł͏����ʒu�ŌŒ�
	if(!isPlayFlag)
	{
		// �X�^�[�g�ʒu�ŌŒ�
		if (mp_gameRoad->GetPosType(mp_gameRoad->START).x != m_pos.x ||
			mp_gameRoad->GetPosType(mp_gameRoad->START).y != m_pos.z)
		{
			m_pos.x = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.x;
			m_pos.z = mp_gameRoad->GetRoadObject((int)(mp_gameRoad->GetPosType(mp_gameRoad->START).y), (int)(mp_gameRoad->GetPosType(mp_gameRoad->START).x)).pos.z;
		}

		const int OFFSETNUM = 8;
		// ����̃}�X�̎��͔��}�X���̑��΍��W
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },		   { 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		// �ʉߍς݂̓��H���L��
		m_passedRoadPos = SimpleMath::Vector2(mp_gameRoad->GetPosType(mp_gameRoad->START).x,
											  mp_gameRoad->GetPosType(mp_gameRoad->START).y);

		// ���̍s���������(���͔��}�X�𒲂ׂ�)
		float distTmp = 0.0f;
		for (int k = 0; k < OFFSETNUM; k++)
		{
			// �}�X�ڏ�̈ʒu���W
			SimpleMath::Vector2 comparePos = SimpleMath::Vector2(m_passedRoadPos.x, m_passedRoadPos.y);
			comparePos += OFFSET[k];

			// ��ׂ���̂��Ȃ����߉�����
			if (k == 0)
			{
				// ���̍s����(�}�X�ڏ�̍��W)
				m_nextPos = comparePos;

				// ���̍s����(��)�ƍ��̈ʒu�̋��������߂�
				distTmp = (mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x - m_pos.x) +
						  (mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z - m_pos.z);
			}
			// �ŏ��ɒ��ׂ���W�ł��Ȃ��A�������݂��Ă���Ƃ���Ȃ�
			else if (k != 0 && mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).roadType != (int)mp_gameRoad->NONE)
			{
				// ���̍s����ƍ��̈ʒu�̋��������߂�
				float dist = (mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.x - m_pos.x) +
							 (mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos.z - m_pos.z);

				// dist�̂ق���������(�����Ŕ�ׂ��Ƃ���̂ق����߂�)�Ȃ�
				if (dist < distTmp)
				{
					// ���ɒʉ߂����Ƃ��낶��Ȃ���Ύ��̈ړ���Ɍ���
					if (m_passedRoadPos != comparePos) m_nextPos = comparePos;

					distTmp = dist;
				}
			}
		}
	}
	// �}�E�X�ŃN���b�N����Ă���̏���
	else
	{
		// �}�E�X�Œ��S���N���b�N���Ă���J�E���g�_�E��
		if (!m_playStartFlag)
		{
			m_moveStartCountDown--;

			// 3�b�o�߂ŃQ�[���J�n
			if (m_moveStartCountDown <= 0)
			{
				// �J�E���g�_�E���p�^�C�����Z�b�g
				m_moveStartCountDown = 180;
				// �Q�[���J�n�t���O�𗧂Ă�
				m_playStartFlag = true;
				// �ړ��J�n
				m_velFlag = true;
			}
		}
	}

	// �Q�[�����J�n����Ă���̏���
	if (m_playStartFlag)
	{
		// �}�E�X���̎擾
		CURSORINFO ci = { sizeof(CURSORINFO) };
		GetCursorInfo(&ci);
		if (ci.flags != 0)
		{
			// �}�E�X�J�[�\���̕\��
			ShowCursor(FALSE);
		}

		// �e�̍X�V
		mp_bulletManager->Update(timer, m_pos, cameraDir);

		// �v���C���[�ړ�(�x�N�g��)
		const int OFFSETNUM = 8;
		// ����̃}�X�̎��͔��}�X���̑��΍��W
		SimpleMath::Vector2 OFFSET[OFFSETNUM] =
		{
			{ -1,-1 },{ 0,-1 },{ 1,-1 },
			{ -1, 0 },		   { 1, 0 },
			{ -1, 1 },{ 0, 1 },{ 1, 1 }
		};

		SimpleMath::Vector2 nowPos = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);
		// ���̍s����Ɍ��肵���ʒu�ƌ��ݍ��W������Ȃ��߂��ʒu�ɂ�����
		if (abs(m_pos.x - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x) < 0.01f &&
			abs(m_pos.z - mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z) < 0.01f)
		{
			// ���̈ʒu�Ɍ��ݍ��W�����炷
			m_pos.x = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.x;
			m_pos.z = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos.z;

			// �ǂ̕����ɐi��ł�������ׂ�
			if (abs(m_vel.x) > abs(m_vel.z))
			{
				// X�������ɐi��ł����ꍇ
				// ���ʂ�������ʉߍς݂ɂ���
				if (m_vel.x < 0.00f)      m_passedRoadPos.x = m_nextPos.x + 1;
				else if (m_vel.x > 0.00f) m_passedRoadPos.x = m_nextPos.x - 1;
				// �����Ɏ��Ⴂ�̂ق����L�����Ă���
				m_passedRoadPos.y = m_nextPos.y;
			}
			else if (abs(m_vel.x) < abs(m_vel.z))
			{
				// Z�������ɐi��ł����ꍇ
				// ���ʂ�������ʉߍς݂ɂ���
				if (m_vel.z < 0.00f)      m_passedRoadPos.y = m_nextPos.y + 1;
				else if (m_vel.z > 0.00f) m_passedRoadPos.y = m_nextPos.y - 1;
				// �����Ɏ��Ⴂ�̂ق����L�����Ă���
				m_passedRoadPos.x = m_nextPos.x;
			}

			// ���̈ʒu�ɒ���������next����now�ɕς��Ă���(�킩��₷�����邽��)
			SimpleMath::Vector2 now = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);

			// ���̍s���������
			float distTmp = 0.0f;
			for (int k = 0; k < OFFSETNUM; k++)
			{
				SimpleMath::Vector2 pos = nowPos;
				// �ʉߒ��̓��H(ID)���L��
				m_passingRoadPos = nowPos;
				// �}�X�ڏ�̈ʒu���W
				pos += OFFSET[k];

				// ��ׂ���̂��Ȃ����ߐݒ肵�Ă���
				if (distTmp == 0.0f)
				{
					// ���̍s����o�^
					distTmp = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
							  (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);
				}

				// �������݂��Ă���Ƃ���Ȃ�
				if (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).roadType != mp_gameRoad->NONE)
				{
					// ���ׂĂ���s����ƍ��̈ʒu�̋��������߂�
					float dist = (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.x - m_pos.x) +
								 (mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z)*(mp_gameRoad->GetRoadObject((int)pos.y, (int)pos.x).pos.z - m_pos.z);

					// dist�̂ق���������(�����Ŕ�ׂ��Ƃ���̂ق����߂�)�Ȃ�
					if (dist <= distTmp)
					{
						// �ړ����Ă����}�X�̏����擾
						int pX = (int)m_passedRoadPos.x;
						int pY = (int)m_passedRoadPos.y;
						SimpleMath::Vector2 p = SimpleMath::Vector2((float)pX, (float)pY);

						// ���ɒʉ߂����Ƃ���łȂ���Ύ��̈ړ���Ɍ���
						if (p != pos && nowPos != pos)
						{
							// ���̍s���挈��
							m_nextPos = pos;
							// �ŒZ�����X�V�A���͂���Ɣ�ׂ�
							distTmp = dist;
						}
					}
				}
			}

			//if (mp_gameRoad->GetRoadObject((int)nowPos.y, (int)nowPos.x).roadType != mp_gameRoad->NONE)
			//{
			//	// ����ID�ɐݒ肵�Ă��Ȃ��Ƃ���łȂ���Έړ����J�n����
			//	m_velFlag = false;
			//}

			// �������������P���|�C���g��������
			if (mp_gameRoad->GetRoadObject((int)m_passingRoadPos.y, (int)m_passingRoadPos.x).roadType == 3)
			{
				// �ړ�����߂�
				/*m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
				m_velFlag = false;*/
				m_spawnFlag = true;

				// �J�E���g
				/*m_spawnElapsedTime++;

				// ��萔�𒴂�����
				if (m_spawnElapsedTime > SPAWNTIME)
				{
					// �J�E���g���Z�b�g
					m_spawnElapsedTime = 0;
					// �ړ����J�n����
					m_velFlag = true;
				}*/
			}
		}
		if (m_spawnFlag)
		{
			// �ړ�����߂�
			m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			m_velFlag = false;

			// �J�E���g
			m_spawnElapsedTime++;

			// ��萔�𒴂�����
			if (m_spawnElapsedTime > SPAWNTIME)
			{
				// �J�E���g���Z�b�g
				m_spawnElapsedTime = 0;
				// �ړ����J�n����
				m_velFlag = true;
				m_spawnFlag = false;
			}
		}
		// �ړ��\��������
		if(m_velFlag)
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
	}

	// �G�ƏՓ˂�����
	if (m_isDamaged)
	{
		// �J�E���g��i�߂�
		m_damageCount++;
		// 5�t���[�����Ƃɕ\���ؑ�
		if (m_damageCount % 4 == 0)
		{
			if (mp_damageEffect->GetAlpha() == 0.0f)
			{
				mp_damageEffect->SetAlpha(1.0f);
			}
			else if(mp_damageEffect->GetAlpha() == 1.0f)
			{
				mp_damageEffect->SetAlpha(0.0f);
			}
		}
		// 20�t���[���ŉ��o�I��
		if (m_damageCount > 20)
		{
			mp_damageEffect->SetAlpha(0.0f);
			m_damageCount = 0;
			m_isDamaged = false;
		}
	}
	else
	{
		m_damageCount = 0;
	}

	// HP�o�[
	if (0 < m_hp && m_hp <= 3)
	{
		mp_hp->SetRect(mp_hp->GetWidth()*2.0f, 0.0f, (mp_hp->GetWidth()*2.0f)+ mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	else if (3 < m_hp && m_hp <= 7)
	{
		mp_hp->SetRect(mp_hp->GetWidth(), 0.0f, mp_hp->GetWidth()*2.0f, mp_hp->GetHeight());
	}
	else if(7 < m_hp && m_hp <= 10)
	{
		mp_hp->SetRect(0.0f, 0.0f, mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	// HP��0�ȉ��ɂȂ�����
	if (m_hp <= 0)
	{
		// �ړ����~�߂�
		m_hp = 0;
		m_vel = SimpleMath::Vector3::Zero;
	}

	// �ړ��O�̍��W���L��
	m_posTmp = m_pos;
	// �v���C���[�ړ�(���W)
	m_pos += m_vel;
	// �ړ���̍��W�Ƃ̕΍�����ړ��������Z�o
	m_dir = m_pos - m_posTmp;
	//m_dir.Normalize();

	// �Փ˔���p�̉��z�I�u�W�F�N�g����
	Collision::Box box;
	box.c = SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // ���E���̒��S
	box.r = SimpleMath::Vector3(1.0f, m_height / 2.0f, 1.0f);                        // �e���a
	SetCollision(box);

	//--------------------2D Object Update--------------------//

	// �X�^�[�g�ē��I�u�W�F�N�g�̃t�F�[�h
	float sinWave = m_startGudeWave;
	m_startGudeWave += 0.1f;
	mp_startGuide->SetAlpha((sin(sinWave) * 2) * 0.5f);

	//--------------------------------------------------------//

	return true;
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Render(MatrixManager* matrixManager, GameEnemyManager::DANGERDIRECTION dangerDir)
{
	// �e�̕`��
	mp_bulletManager->Render(matrixManager);

	// �A�N�e�B�u�ȃE�B���h�E�̃T�C�Y
	RECT activeWndRect;
	// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����擾
	HWND activeWnd = GetActiveWindow();
	// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����炻�̉�ʂ̑傫�����擾
	GetWindowRect(activeWnd, &activeWndRect);

	// �E�B���h�E�̃T�C�Y���擾
	float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
	float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);
	// �^�C�g���o�[�̍������擾
	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);

	// �X�^�[�g�J�E���g�̕`��
	if (m_moveStartCountDown < 180 && !m_playStartFlag)
	{
		mp_startCount->SetRect(float(((m_moveStartCountDown / 60)+1) * COUNT_UI_SIZE),
							   float(0.0f),
							   float(((m_moveStartCountDown / 60)+1) * COUNT_UI_SIZE) + COUNT_UI_SIZE,
							   float(COUNT_UI_SIZE));
		mp_startCount->Render();
	}
	
	// �X�^�[�g�ē��̕\��
	if (!m_playStartFlag)
	{
		mp_startGuide->RenderAlpha();
	}
		
	// ���˃|�C���^�[�̕`��
	if (m_playStartFlag)
	{
		mp_shootPointer->Render();
	}

	// ���ߋ����̓G����ʓ��ɂ��Ȃ�������댯�T�C���\��
	if (dangerDir == GameEnemyManager::DANGERDIRECTION::DIR_LEFT)
	{
		if (m_isFullScreen)
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(float(activeWndRect.left) + mp_dengerousSign->GetWidth(),
														 (windowHeight*0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		else
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(mp_dengerousSign->GetWidth(),
														 ((windowHeight - titlebarHeight) * 0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		mp_dengerousSign->Render();
	}
	else if (dangerDir == GameEnemyManager::DANGERDIRECTION::DIR_RIGHT)
	{
		if (m_isFullScreen)
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(float(activeWndRect.right) - (mp_dengerousSign->GetWidth()*2.0f),
														 (windowHeight*0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		else
		{
			mp_dengerousSign->SetPos(SimpleMath::Vector2(float(activeWndRect.right - activeWndRect.left) - (mp_dengerousSign->GetWidth()*2.0f),
														 ((windowHeight - titlebarHeight) * 0.5f) - (mp_dengerousSign->GetHeight() * 0.5f)));
		}
		mp_dengerousSign->Render();
	}

	// HP�\��
	if (m_playStartFlag)
	{
		for (int i = 0; i < m_hp; i++)
		{
			mp_hp->SetPos(SimpleMath::Vector2(m_hpBasePos.x + float(mp_hp->GetWidth() * i), m_hpBasePos.y));
			mp_hp->Render();
		}
	}
	
	// �_���[�W���o�\��
	mp_damageEffect->RenderAlpha();
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
