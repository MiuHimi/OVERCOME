//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2019/08/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int SceneStageSelect::STAGE_ICON_SIZE = 80;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toPlayMoveOnChecker(false),
	  m_selectedStage(0),
	  mp_background(nullptr), mp_stageSelectImage(nullptr),
	  mp_stageNum{nullptr}, mp_stageFlame{nullptr},
	  mp_fade(nullptr),
	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneStageSelect::Initialize()
{
	m_toPlayMoveOnChecker = false;

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

	// �w�i�̐���
	mp_background = std::make_unique<Obj2D>();
	mp_background->Create(L"Resources\\Images\\gray.png", nullptr);
	mp_background->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_background->SetRect(0.0f, 0.0f, mp_background->GetWidth(), mp_background->GetHeight());

	// StageSelect�̐���
	mp_stageSelectImage = std::make_unique<Obj2D>();
	mp_stageSelectImage->Create(L"Resources\\Images\\StageSelect\\stageselect_image.png", nullptr); // �z�o�[�X�v���C�g�Ȃ�
	mp_stageSelectImage->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 525.0f, 50.0f, 1.0f, 1.0f);
	mp_stageSelectImage->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_stageSelectImage->GetWidth() * 0.5f),
													(mp_stageSelectImage->GetHeight() * 2.0f)));
	mp_stageSelectImage->SetRect(0.0f, 0.0f, mp_stageSelectImage->GetWidth(), mp_stageSelectImage->GetHeight());

	// �X�e�[�W�ԍ��̐���
	for (int i = 0; i < STAGE::NUM; i++)
	{
		mp_stageNum[i] = std::make_unique<Obj2D>();
		mp_stageNum[i]->Create(L"Resources\\Images\\StageSelect\\stageselect_num_len.png", L"Resources\\Images\\StageSelect\\stageselect_num_len_hover.png");

		mp_stageNum[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(STAGE_ICON_SIZE), float(STAGE_ICON_SIZE), 1.0f, 1.0f);
		mp_stageNum[i]->SetPos(SimpleMath::Vector2(float(((windowWidth*0.5f) - STAGE_ICON_SIZE*1.5f) + (i*(STAGE_ICON_SIZE*1.5f))), 
												   (windowHeight * 0.5f) - (mp_stageNum[i]->GetHeight() * 0.5f)));
		mp_stageNum[i]->SetRect(float((i+1)*STAGE_ICON_SIZE), 0.0f, float((i + 1)*STAGE_ICON_SIZE + STAGE_ICON_SIZE), float(STAGE_ICON_SIZE));
	}
	// �X�e�[�W�ԍ��t���[���̐���
	for (int i = 0; i < STAGE::NUM; i++)
	{
		mp_stageFlame[i] = std::make_unique<Obj2D>();
		mp_stageFlame[i]->Create(L"Resources\\Images\\StageSelect\\stageselect_flame.png", L"Resources\\Images\\StageSelect\\stageselect_flame_hover.png");

		mp_stageFlame[i]->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(STAGE_ICON_SIZE), float(STAGE_ICON_SIZE), 1.0f, 1.0f);
		mp_stageFlame[i]->SetPos(SimpleMath::Vector2(float(((windowWidth*0.5f) - STAGE_ICON_SIZE*1.5f) + (i*(STAGE_ICON_SIZE*1.5f))), 
													 (windowHeight * 0.5f) - (mp_stageFlame[i]->GetHeight() * 0.5f)));
		mp_stageFlame[i]->SetRect(0.0f, 0.0f, float(STAGE_ICON_SIZE), float(STAGE_ICON_SIZE));
	}
	
	// �t�F�[�h�摜�̐���
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());
	mp_fade->SetAlpha(0.0f);

	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();

	// �r���[�s��̍쐬
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

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
	
	// �T�E���h�Đ�
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"SceneStageSelect.acb", L"SceneStageSelect.awb");
	adx2le->Play(0);
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneStageSelect::Finalize()
{
	// �s��Ǘ��ϐ��̍폜
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

	// �T�E���h�̒�~
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();
}

/// <summary>
/// �X�e�[�W�I���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	InputManager::SingletonGetInstance().Update();
	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �T�E���h�̍X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// �X�e�[�W�ԍ��ƃ}�E�X�J�[�\���̏Փ˔���
	SimpleMath::Vector2 mousePos = SimpleMath::Vector2((float)InputManager::SingletonGetInstance().GetMousePosX(),
		(float)InputManager::SingletonGetInstance().GetMousePosY());
	for (int i = 0; i < STAGE::NUM; i++)
	{
		SimpleMath::Vector2 btnPos = mp_stageNum[i]->GetPos();
		float btnWidth = mp_stageNum[i]->GetWidth();
		float btnHeight = mp_stageNum[i]->GetHeight();
		// �}�E�X���{�^���ɐڐG���Ă�����
		if (mp_stageNum[i]->IsCollideMouse(mousePos, btnPos, btnWidth, btnHeight))
		{
			//	�z�o�[��Ԃɂ���
			mp_stageNum[i]->SetHover(true);
			//	�����ꏊ�̃I�u�W�F�N�g�̂��ߓ��l�Ƀz�o�[��Ԃɂ���
			mp_stageFlame[i]->SetHover(true);
		}
		// �ڐG���Ă��Ȃ�������
		else
		{
			//	�z�o�[��Ԃɂ��Ȃ�
			mp_stageNum[i]->SetHover(false);
			//	�����ꏊ�̃I�u�W�F�N�g�̂��ߓ��l�Ƀz�o�[��Ԃɂ��Ȃ�
			mp_stageFlame[i]->SetHover(false);
		}
	}
	
	// �X�e�[�W�̑I��
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		for (int i = 0; i < STAGE::NUM; i++)
		{
			// �}�E�X�ƃA�C�R�����Փ˂��Ă�����
			if (mp_stageNum[i]->GetHover())
			{
				// �I�����ꂽ�X�e�[�W�ԍ����L��
				m_selectedStage = i + 1;
				// �v���C�X�e�[�W������
				//SceneManager::SetStageNum(m_selectedStage);
				// �V�[���J�ڔ���
				m_toPlayMoveOnChecker = true;
				// �T�E���h�Đ�
				adx2le->Play(1);
				break;
			}
		}
	}

	// �V�[���J�ڊJ�n
	if (m_toPlayMoveOnChecker)
	{
		for (int i = 0; i < STAGE::NUM; i++)
		{
			if (i == m_selectedStage - 1)
			{
				// �I�����ꂽ�X�e�[�W���z�o�[��Ԃɂ���
				mp_stageNum[i]->SetHover(true);
				mp_stageFlame[i]->SetHover(true);
			}
			else
			{
				// �I������Ă��Ȃ�������z�o�[��Ԃɂ��Ȃ�
				mp_stageNum[i]->SetHover(false);
				mp_stageFlame[i]->SetHover(false);
			}
		}
		
		// �t�F�[�h�A�E�g
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// �V�[���J��
	if (m_toPlayMoveOnChecker && mp_fade->GetAlpha() >= 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneStageSelect::Render()
{
	// �w�i�̕\��
	mp_background->Render();
	// StageSelect���̕\��
	mp_stageSelectImage->Render();

	// �X�e�[�W�ԍ��̕\��
	for (int i = 0; i < STAGE::NUM; i++)
	{
		mp_stageNum[i]->Render();
		mp_stageFlame[i]->Render();
	}

	// �t�F�[�h�摜�̕\��
	mp_fade->RenderAlpha();
}

