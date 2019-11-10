//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <math.h>

#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int SceneTitle::TITLE_STR_WIDTH = 60;
const int SceneTitle::MAX_GHOST_POS_X = 20;
const float SceneTitle::MAX_GHOST_POS_Y = 57.0f;
const float SceneTitle::MIN_GHOST_POS_Y = 43.0f;
const int SceneTitle::RESPAWN_COUNT_MIN = 180;
const int SceneTitle::RESPAWN_COUNT_MAX = 240;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneTitle::SceneTitle(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toStageSelectMoveOnChecker(false),
	  m_colorAlpha(0.0f),
	  mp_title{ nullptr }, mp_startBtn(nullptr), mp_fade(nullptr),
	  mp_camera(nullptr),
	  m_ghostPos(SimpleMath::Vector3(0.0f,0.0f,0.0f)), m_ghostDir(SceneTitle::GHOST_DIR::NONE),
	  m_spawnCount(0), m_nextSpawnCount(0),
	  mp_modelHouse(nullptr), mp_modelEnemy(nullptr),
	  mp_matrixManager(nullptr),
	  mp_effectManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneTitle::~SceneTitle()
{
}

/// <summary>
/// �^�C�g���V�[���̏���������
/// </summary>
void SceneTitle::Initialize()
{
	m_toStageSelectMoveOnChecker = false;

	// ���l�̐ݒ�(������)
	m_colorAlpha = 1.0f;

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

	// �^�C�g���̐���
	for (int i = 0; i < MAX_TITLE_STR; i++)
	{
		mp_title[i] = std::make_unique<Obj2D>();
		mp_title[i]->Create(L"Resources\\Images\\Title\\title_stitch_image.png", nullptr); // �z�o�[�X�v���C�g�Ȃ�
		if (GetFullScreen())
		{
			mp_title[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), (float)TITLE_STR_WIDTH * 2.0f, 200 * 2.0f, 1.0f, 1.0f);
			mp_title[i]->SetScale(2.0f);
		}
		else
		{
			mp_title[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), (float)TITLE_STR_WIDTH, 200.0f, 1.0f, 1.0f);
		}
		mp_title[i]->SetPos(SimpleMath::Vector2(((windowWidth * 0.5f) - ((float)TITLE_STR_WIDTH * (float)MAX_TITLE_STR/* * 0.5f*/))+ i*mp_title[i]->GetWidth(),0.0f));
		mp_title[i]->SetRect((float)i * (float)TITLE_STR_WIDTH, 0.0f, ((float)i * (float)TITLE_STR_WIDTH) + (float)TITLE_STR_WIDTH, mp_title[i]->GetHeight() * 0.5f);
	}
	
	// �X�^�[�g�{�^���̐���
	mp_startBtn = std::make_unique<Obj2D>();
	mp_startBtn->Create(L"Resources\\Images\\Title\\title_gamestart.png", L"Resources\\Images\\Title\\title_gamestart_hover.png");
	mp_startBtn->Initialize(SimpleMath::Vector2(0.0f,0.0f), 640.0f, 100.0f, 1.0f, 1.0f);
	mp_startBtn->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_startBtn->GetWidth() * 0.5f),
										   ((windowHeight - (mp_startBtn->GetHeight() * 2.0f)))));
	mp_startBtn->SetRect(0.0f, 0.0f, mp_startBtn->GetWidth(), mp_startBtn->GetHeight());

	// �t�F�[�h�摜�̐���
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	// �J�����I�u�W�F�N�g�̍쐬
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom, m_isFullScreen);

	// �H�쏉���ʒu
	m_ghostPos = SimpleMath::Vector3(-20.0f, 45.0f, 130.0f);
	m_ghostDir = GHOST_DIR::RIGHT_DIR;
	m_nextSpawnCount = 30; // �f�t�H���g�l

	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// �ƃ��f�����쐬
	mp_modelHouse = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\house.cmo", fx);
	// �G���f�����쐬
	mp_modelEnemy = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost.cmo", fx);

	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();
	
	// �r���[�s��̍쐬
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float angle = 45.0f;
	float fovAngleY = XMConvertToRadians(angle);

	// �ˉe�s����쐬
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// �s���ݒ�
	mp_matrixManager->SetViewProjection(view, projection);

	// �G�t�F�N�g�}�l�[�W���[�̏�����
	mp_effectManager = new EffectManager();
	mp_effectManager->Create();
	mp_effectManager->Initialize();
	mp_effectManager->SetRenderState(view, projection);

	// �T�E���h�Đ�
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"SceneTitle.acb", L"SceneTitle.awb");
	adx2le->Play(0);
}
/// <summary>
/// �^�C�g���V�[���̏I������
/// </summary>
void SceneTitle::Finalize()
{
	// �s��Ǘ��ϐ��̍폜
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

	// �G�t�F�N�g�Ǘ��ϐ��̍폜
	if (mp_effectManager != nullptr) {
		mp_effectManager->Finalize();
		delete mp_effectManager;
		mp_effectManager = nullptr;
	}

	// �T�E���h�̒�~
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();
}

/// <summary>
/// �^�C�g���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneTitle::Update(DX::StepTimer const& timer)
{
	// �T�C���g���ϓ����邽�߂̒l
	static float ghostWave;
	ghostWave += 0.05f;

	// �T�E���h�̍X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// �G�t�F�N�g�̍X�V
	mp_effectManager->Update(timer);

	// ���C�g�̌v�Z
	/*auto SetLight = [&](IEffect* effect)
	{
		IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
		if (lights)
		{
			lights->SetAmbientLightColor(SimpleMath::Vector3(0.5f, 0.5f, 0.5f)); // �A���r�G���g�F��ݒ肷��
			lights->SetLightEnabled(0, true);                        // �P�Ԃ̃��C�gON
			lights->SetLightEnabled(1, false);                       // �Q�Ԃ̃��C�gOFF
			lights->SetLightEnabled(2, false);                       // �R�Ԃ̃��C�gOFF
			lights->SetLightDiffuseColor(0, Colors::Red);            // ���C�g�̐F
			SimpleMath::Vector3 light_dir(0.0f, 0.0f, -1);           // ���C�g�̕����x�N�g��
			light_dir.Normalize();                                   // ���C�g�̕����x�N�g���𐳋K������
			lights->SetLightDirection(0, light_dir);                 // �P�Ԃ̃��C�g�̕�����ݒ肷��
			lights->SetPerPixelLighting(true);                       // �s�N�Z���V�F�[�_�Ō��̉e�����v�Z����
		}
	};
	mp_modelEnemy->UpdateEffects(SetLight);*/

	// �H��̈ړ�
	if(m_ghostDir == GHOST_DIR::RIGHT_DIR)
		m_ghostPos.x += 0.1f;
	else if (m_ghostDir == GHOST_DIR::LEFT_DIR)
		m_ghostPos.x -= 0.1f;
	// �ӂ�ӂ킳���邽�߂ɃT�C���g���g�p
	float sinWave = sin(ghostWave) * 0.05f;
	m_ghostPos.y += sinWave;

	// �H��̕����]��
	if (m_ghostPos.x > (int)MAX_GHOST_POS_X)
	{
		m_spawnCount++;
		if (m_spawnCount > m_nextSpawnCount)
		{
			// �H��𔽑Ό�����
			m_ghostDir = GHOST_DIR::LEFT_DIR;
			// �H��̍����ݒ�
			m_ghostPos.y = (float)(MIN_GHOST_POS_Y + rand()*(MAX_GHOST_POS_Y - MIN_GHOST_POS_Y + 1) / (1 + RAND_MAX));
			// �J�E���g�����Z�b�g
			m_spawnCount = 0;
			// ���ɃX�|�[������̂ɕK�v�ȃJ�E���g��ݒ�
			m_nextSpawnCount = RESPAWN_COUNT_MIN + rand()*(RESPAWN_COUNT_MAX - RESPAWN_COUNT_MIN + 1) / (1 + RAND_MAX);
		}
	}
	else if (m_ghostPos.x < (int)-MAX_GHOST_POS_X)
	{
		m_spawnCount++;
		if (m_spawnCount > m_nextSpawnCount)
		{
			// �H��𔽑Ό�����
			m_ghostDir = GHOST_DIR::RIGHT_DIR;
			// �H��̍����ݒ�
			m_ghostPos.y = (float)(MIN_GHOST_POS_Y + rand()*(MAX_GHOST_POS_Y - MIN_GHOST_POS_Y + 1) / (1 + RAND_MAX));
			// �J�E���g�����Z�b�g
			m_spawnCount = 0;
			// ���ɃX�|�[������̂ɕK�v�ȃJ�E���g��ݒ�
			m_nextSpawnCount = RESPAWN_COUNT_MIN + rand()*(RESPAWN_COUNT_MAX - RESPAWN_COUNT_MIN + 1) / (1 + RAND_MAX);
		}
	}


	// �J�����̍X�V(�^�C�g���V�[���̃J�����͒�_�J����)
	mp_camera->Update(timer, SimpleMath::Vector3(0.0f,0.0f,0.0f), 0.0f, SimpleMath::Vector3(0.0f, 0.0f, 0.0f), true);
	
	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().Update();

	// �X�^�[�g�{�^���ƃ}�E�X�J�[�\���̏Փ˔���
	SimpleMath::Vector2 mousePos = SimpleMath::Vector2((float)InputManager::SingletonGetInstance().GetMousePosX(),
													   (float)InputManager::SingletonGetInstance().GetMousePosY());
	SimpleMath::Vector2 btnPos = mp_startBtn->GetPos();
	float btnWidth = mp_startBtn->GetWidth();
	float btnHeight = mp_startBtn->GetHeight();
	// �}�E�X���{�^���ɐڐG���Ă�����
	if (mp_startBtn->IsCollideMouse(mousePos, btnPos, btnWidth, btnHeight))
		mp_startBtn->SetHover(true);	//	�z�o�[��Ԃɂ���
	// �ڐG���Ă��Ȃ�������
	else
		mp_startBtn->SetHover(false);	// �z�o�[��Ԃɂ��Ȃ�

	// �T�C���g���ϓ����邽�߂̒l
	static float wave;
	wave += 0.01f;

	// �^�C�g�����S�̃A�j���[�V����
	for (int i = 0; i < MAX_TITLE_STR; i++)
	{
		float sinWave = (sin(wave + (i*2.0f)) - 1.0f)* 30.0f;
		mp_title[i]->SetPos(SimpleMath::Vector2(mp_title[i]->GetPos().x, sinWave));
	}

	// �V�[���J�ڂ����A���l��0�łȂ�������
	if (!m_toStageSelectMoveOnChecker && mp_fade->GetAlpha() != 0.0f)
	{
		// �t�F�[�h�C��
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_IN);
	}

	// �X�^�[�g�{�^�����N���b�N�Ńt�F�[�h�J�n
	if (mp_startBtn->GetHover() && InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toStageSelectMoveOnChecker = true;
		adx2le->Play(1);
	}
	if (m_toStageSelectMoveOnChecker)
	{
		m_colorAlpha -= 0.01f;
		if (m_colorAlpha < 0.0f) m_colorAlpha = 0.0f;

		// �N���b�N���ꂽ��z�o�[��ԂɌŒ�
		mp_startBtn->SetHover(true);
		// ���l�ݒ�
		mp_startBtn->SetAlpha(m_colorAlpha);
		for (int i = 0; i < MAX_TITLE_STR; i++)
		{
			mp_title[i]->SetAlpha(m_colorAlpha);
		}

		adx2le->SetVolume(m_colorAlpha);

		// �t�F�[�h�A�E�g
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// �V�[���J��
	if (m_toStageSelectMoveOnChecker && m_colorAlpha <= 0.0f && mp_fade->GetAlpha() >= 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// �^�C�g���V�[���̕`�揈��
/// </summary>
void SceneTitle::Render()
{
	// �r���[�s��̍쐬
	SimpleMath::Matrix view = SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), SimpleMath::Vector3::Up);
	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float angle = 45.0f;
	float fovAngleY = XMConvertToRadians(angle);

	// �ˉe�s����쐬
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);
	// �s���ݒ�
	mp_matrixManager->SetViewProjection(view, projection);

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	// �Ƃ̕`��
	mp_modelHouse->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
						world, mp_matrixManager->GetView(), mp_matrixManager->GetProjection());

	
	SimpleMath::Matrix rota = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians((float)m_ghostDir));
	world *= rota;
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_ghostPos);
	world *= trans;

	mp_modelEnemy->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
						world, mp_matrixManager->GetView(), mp_matrixManager->GetProjection());

	// �G�t�F�N�g�̕`��
	mp_effectManager->Render();

	// �^�C�g���̕\��
	if (GetFullScreen())
	{
		for (int i = 0; i < MAX_TITLE_STR; i++)
		{
			mp_title[i]->RenderAlphaScale();
		}
	}
	else
	{
		for (int i = 0; i < MAX_TITLE_STR; i++)
		{
			mp_title[i]->RenderAlphaScale();
		}
	}

	// �X�^�[�g�{�^���̕\��
	mp_startBtn->RenderAlpha();

	// �t�F�[�h�摜�̕\��
	mp_fade->RenderAlpha();
}
