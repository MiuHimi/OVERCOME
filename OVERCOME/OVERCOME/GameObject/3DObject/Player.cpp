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
#include "../../Utility/MathManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

#include "../../GameObject/3DObject/GameEnemyManager.h"


// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int Player::DAMAGE_EFFECT_COUNT = 4;
const int Player::FINISH_DAMAGE_EFFECT_COUNT = 20;
const int Player::START_COUNT_TIME = 180;
const int Player::COUNT_UI_SIZE = 80;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
Player::Player()
	: m_pos(SimpleMath::Vector3::Zero), m_vel(SimpleMath::Vector3::Zero), m_dir(SimpleMath::Vector3::Zero),
	  m_height(0.0f), m_posTmp(SimpleMath::Vector3::Zero),
	  m_hp(0), m_hpBasePos{0.0f}, m_damageCount(0), m_isDamaged(false),
	  m_playStartFlag(false), m_moveStartCountDown(0),
	  m_passedRoadPos(SimpleMath::Vector2::Zero), m_passingRoadPos(SimpleMath::Vector2::Zero), m_nextPos(SimpleMath::Vector2::Zero), m_velFlag(false),
	  m_world(SimpleMath::Matrix::Identity),
	  mp_bulletManager(nullptr), mp_gameRoad(nullptr),
	  m_isFullScreen(false),
	  mp_startCount(nullptr), mp_shootPointer(nullptr), mp_dengerousSign(nullptr),
	  mp_hp(nullptr), mp_hpFrame(nullptr), mp_damageEffect(nullptr)
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
	m_height	= 1.75f;										// �v���C���[���g�̍���
	m_hp		= 10;											// �̗�

	m_moveStartCountDown = START_COUNT_TIME;					// �����o����܂ł̃J�E���g�_�E���p

	m_world = SimpleMath::Matrix::Identity;						// ���[���h�s��

	// ���I�u�W�F�N�g�̏�����
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
	mp_damageEffect->Create(L"Resources\\Images\\Play\\damaged.png", nullptr);
	mp_damageEffect->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_damageEffect->SetRect(0.0f, 0.0f, mp_damageEffect->GetWidth(), mp_damageEffect->GetHeight());

	// �̗̓I�u�W�F�N�g�̐���
	mp_hp = std::make_unique<Obj2D>();
	mp_hp->Create(L"Resources\\Images\\Play\\player_hp.png", nullptr);
	mp_hp->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 100.0f, 25.0f, 1.0f, 1.0f);
	mp_hp->SetRect(0.0f, 0.0f, mp_hp->GetWidth(), mp_hp->GetHeight());
	mp_hp->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_hp->GetWidth()*5.0f), windowHeight - (mp_hp->GetHeight()*2.0f)));

	// �̗̘͂g�I�u�W�F�N�g�̐���
	mp_hpFrame = std::make_unique<Obj2D>();
	mp_hpFrame->Create(L"Resources\\Images\\Play\\player_hp_frame.png", nullptr);
	mp_hpFrame->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 1010.0f, 45.0f, 1.0f, 1.0f);
	mp_hpFrame->SetRect(0.0f, 0.0f, mp_hpFrame->GetWidth(), mp_hpFrame->GetHeight());
	mp_hpFrame->SetPos(SimpleMath::Vector2((windowWidth*0.5f) - (mp_hpFrame->GetWidth()*0.5f), windowHeight - (mp_hpFrame->GetHeight() + 18.0f)));

	// ��ʒu�ݒ�
	m_hpBasePos = mp_hp->GetPos();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool Player::Update(DX::StepTimer const & timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir, SimpleMath::Vector3& correctPos, const bool isAssaultedState)
{
	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �O���̓����蔻�蓙�ŋ������ꂽ�ʒu�ɐݒ�
	m_pos.y = correctPos.y;

	// �i�݂����O�̏���
	bool isGoOn = BeforeGoOnPlayer(isPlayFlag);

	// �i�ݎn�߂���̏���
	AfterGoOnPlayer(isGoOn, timer, cameraDir, isAssaultedState);

	// �_���[�W�̍X�V
	UpdateDamage();

	// HP�̍X�V
	UpdateHP();

	// �ړ��O�̍��W���L��
	m_posTmp = m_pos;
	// �v���C���[�ړ�(���W)
	m_pos += m_vel;
	// �ړ���̍��W�Ƃ̕΍�����ړ��������Z�o
	m_dir = m_pos - m_posTmp;

	// �Փ˔���p�̉��z�I�u�W�F�N�g����
	Collision::Box box;
	box.c = SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // ���E���̒��S
	box.r = SimpleMath::Vector3(1.0f, m_height / 2.0f, 1.0f);                        // �e���a
	SetCollision(box);

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
	if (m_moveStartCountDown < START_COUNT_TIME && !m_playStartFlag)
	{
		mp_startCount->SetRect(float(((m_moveStartCountDown / 60)+1) * COUNT_UI_SIZE),
							   float(0.0f),
							   float(((m_moveStartCountDown / 60)+1) * COUNT_UI_SIZE) + COUNT_UI_SIZE,
							   float(COUNT_UI_SIZE));
		mp_startCount->Render();
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
		mp_hpFrame->Render();
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
/// �i�݂����O�̏���
/// </summary>
/// <param name="isClickCenter">�i�ݏo�����߂̃t���O</param>
/// <returns>true=�v���C���[�i�s�\�Afalse=�v���C���[�i�s�s��</returns>
bool Player::BeforeGoOnPlayer(const bool isClickCenter)
{
	// �X�^�[�g����܂ł͏����ʒu�ŌŒ�
	if (!isClickCenter)
	{
		// �X�^�[�g�ʒu�ŌŒ�
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
		SearchNextRoad(m_passedRoadPos);

		// �v���C���[�i�s�s��
		m_playStartFlag = false;
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
				m_moveStartCountDown = START_COUNT_TIME;
				// �v���C���[�i�s�t���O�𗧂Ă�
				m_playStartFlag = true;
				// �ړ��J�n
				m_velFlag = true;
			}
		}
	}

	return m_playStartFlag;
}

/// <summary>
/// �i�ݎn�߂���̏���
/// </summary>
/// <param name="isGoOn">�v���C���[������̐i�s���n�߂����ǂ���</param>
/// <param name="timer">�X�V����</param>
/// <param name="cameraDir">�J�����̌���</param>
void Player::AfterGoOnPlayer(const bool isGoOn, DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& cameraDir, const bool isAssaultedState)
{
	// �v���C���[���i�s���J�n������ȍ~�̏������s��
	if (!isGoOn)return;

	//�}�E�X�J�[�\���̔�\��
	InputManager::SingletonGetInstance().SetVisibleMouseCursor(false);

	// �e�̍X�V
	SimpleMath::Vector3 eyePos = SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height * 0.5f), m_pos.z);
	mp_bulletManager->Update(timer, eyePos, cameraDir);

	// �������A���̍s����I��
	bool isMove = ArrivalNextRoad();

	// �t���O�ɉ����Đi�s���邩�ǂ��������肷��
	SetPlayerMove(isMove, isAssaultedState);
}

/// <summary>
/// �ڎw�����ɓ���
/// </summary>
/// <returns>true=���̂܂ܐi�s�\�Afalse=�i�s��~</returns>
bool Player::ArrivalNextRoad()
{
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
			if (m_vel.x < 0.0f)      m_passedRoadPos.x = m_nextPos.x + 1;
			else if (m_vel.x > 0.0f) m_passedRoadPos.x = m_nextPos.x - 1;
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

		// ���̈ʒu�ɒ��������ߒʉߒ��̓��HID���X�V
		m_passingRoadPos = SimpleMath::Vector2(m_nextPos.x, m_nextPos.y);

		// ���̍s���������
		SearchNextRoad(m_passingRoadPos);

		// �������������P���|�C���g��������
		if (mp_gameRoad->GetRoadObject((int)m_passingRoadPos.y, (int)m_passingRoadPos.x).roadType == (int)GameRoad::RoadType::ASSAULT)
		{
			m_velFlag = false;
		}
		else
		{
			// �����łȂ���ΐi�s����
			m_velFlag = true;
		}
	}

	return m_velFlag;
}

/// <summary>
/// �t���O�ɉ����Đi�s���邩�ǂ��������肷��
/// </summary>
/// <param name="isMove">�i�s����t���O</param>
/// <param name="isAssaultedState">�G�̍U���󋵃t���O</param>
void Player::SetPlayerMove(bool isMove, const bool isAssaultedState)
{
	// �ړ��\��������
	if (isMove)
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
	else
	{
		// �ړ�����߂�
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		m_velFlag = false;

		// �G�̏P�����~�񂾂�
		if (isAssaultedState)
		{
			// �ړ����J�n����
			m_velFlag = true;
		}
	}
}

/// <summary>
/// ���̍s���������(���͔��}�X�𒲂ׂ�)
/// </summary>
void Player::SearchNextRoad(const SimpleMath::Vector2 nowPos)
{
	const int OFFSETNUM = 8;
	// �v���C���[�̎���8�}�X�̑��΍��W
	SimpleMath::Vector2 OFFSET[OFFSETNUM] =
	{
		{ -1,-1 },{ 0,-1 },{ 1,-1 },
		{ -1, 0 },		   { 1, 0 },
		{ -1, 1 },{ 0, 1 },{ 1, 1 }
	};

	// ���̍s���������(���͔��}�X�𒲂ׂ�)
	float distTmp = 0.0f;
	for (int k = 0; k < OFFSETNUM; k++)
	{
		// ���ݒn�����
		SimpleMath::Vector2 comparePos = nowPos;
		// ���͔��}�X�𒲂ׂ�
		comparePos += OFFSET[k];

		// ��ׂ���̂��Ȃ����߉�����
		if (k == 0)
		{
			// ���̍s����(�}�X�ڏ�̍��W)������
			m_nextPos = comparePos;

			// ���̍s����(��)�ƍ��̈ʒu�̋��������߂�
			SimpleMath::Vector3 roadPos = mp_gameRoad->GetRoadObject((int)m_nextPos.y, (int)m_nextPos.x).pos;
			distTmp = MathManager::SingletonGetInstance().GetDistancePoints2D(m_pos.x, m_pos.z, roadPos.x, roadPos.z);
		}
		// �ŏ��ɒ��ׂ���W�Ŗ������
		else
		{
			// �������݂��Ă���Ƃ���Ȃ�ȉ��̏������s��
			if ((mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).roadType == (int)mp_gameRoad->NONE)) continue;

			// ���̍s����(��)�ƍ��̈ʒu�̋��������߂�
			SimpleMath::Vector3 roadPos = mp_gameRoad->GetRoadObject((int)comparePos.y, (int)comparePos.x).pos;
			float dist = MathManager::SingletonGetInstance().GetDistancePoints2D(m_pos.x, m_pos.z, roadPos.x, roadPos.z);

			// dist�̂ق���������(�����Ŕ�ׂ��Ƃ���̂ق����߂�)�A
			// ���ɒʉ߂����Ƃ���łȂ����
			if (dist < distTmp &&
				m_passedRoadPos != comparePos)
			{
				// ���̈ړ���Ɍ���
				m_nextPos = comparePos;
				// �ŒZ�����X�V
				distTmp = dist;
			}
		}
	}
}

/// <summary>
/// �_���[�W�̍X�V
/// </summary>
void Player::UpdateDamage()
{
	// �G�ƏՓ˂�����
	if (m_isDamaged)
	{
		// �J�E���g��i�߂�
		m_damageCount++;
		// �\���ؑ�
		if (m_damageCount % DAMAGE_EFFECT_COUNT == 0)
		{
			if (mp_damageEffect->GetAlpha() == 0.0f)
			{
				mp_damageEffect->SetAlpha(1.0f);
			}
			else if (mp_damageEffect->GetAlpha() == 1.0f)
			{
				mp_damageEffect->SetAlpha(0.0f);
			}
		}
		// ���o�I��
		if (m_damageCount > FINISH_DAMAGE_EFFECT_COUNT)
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

}

/// <summary>
/// HP�̍X�V
/// </summary>
void Player::UpdateHP()
{
	// HP�o�[
	if (m_hp <= PLAYER_STATE::MORIBUND)
	{
		mp_hp->SetRect(mp_hp->GetWidth()*2.0f, 0.0f, (mp_hp->GetWidth()*2.0f) + mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	else if (m_hp <= PLAYER_STATE::WOUND)
	{
		mp_hp->SetRect(mp_hp->GetWidth(), 0.0f, mp_hp->GetWidth() + mp_hp->GetWidth(), mp_hp->GetHeight());
	}
	else if (m_hp <= PLAYER_STATE::NORMAL)
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
}
