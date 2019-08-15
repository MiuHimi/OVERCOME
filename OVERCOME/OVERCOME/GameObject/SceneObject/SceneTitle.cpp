//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneTitle::SceneTitle(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toStageSelectMoveOnChecker(false),
	  m_colorAlpha(0.0f),
	  mp_title(nullptr), mp_startBtn(nullptr), mp_fade(nullptr),
	  mp_camera(nullptr),
	  mp_modelHouse(nullptr),
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
	mp_title = std::make_unique<Obj2D>();
	mp_title->Create(L"Resources\\Images\\Title\\title_image.png", nullptr); // �z�o�[�X�v���C�g�Ȃ�
	if (GetFullScreen())
		mp_title->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 500.0f * 2.0f, 120.0f * 2.0f, 1.0f, 1.0f);
	else
		mp_title->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 500.0f, 120.0f, 1.0f, 1.0f);
	mp_title->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_title->GetWidth() * 0.5f),
										 (windowHeight * 0.5f) - (mp_title->GetHeight() * 1.5f) - titlebarHeight));
	mp_title->SetRect(0.0f, 0.0f, mp_title->GetWidth(), mp_title->GetHeight());

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
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// �ƃ��f�����쐬
	mp_modelHouse = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\house.cmo", fx);

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
	// �T�E���h�̍X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// �G�t�F�N�g�̍X�V
	mp_effectManager->Update(timer);

	// �J�����̍X�V(�^�C�g���V�[���̃J�����͒�_�J����)
	mp_camera->Update(timer, SimpleMath::Vector3(0.0f,0.0f,0.0f), 0.0f, SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	
	// �}�E�X�̍X�V
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
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
		mp_title->SetAlpha(m_colorAlpha);

		// �t�F�[�h�A�E�g
		mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// �V�[���J��
	if (m_toStageSelectMoveOnChecker && m_colorAlpha <= 0.0f && mp_fade->GetAlpha() >= 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
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

	// �G�t�F�N�g�̕`��
	mp_effectManager->Render();

	// �^�C�g���̕\��
	if (GetFullScreen())
	{
		float width = mp_title->GetWidth() * 0.5f;
		float height = mp_title->GetHeight() * 0.5f;
		mp_title->SetRect(0, 0, width, height);
		mp_title->SetScale(2.0f);
		mp_title->RenderAlphaScale();
	}
	else
	{
		mp_title->RenderAlphaScale();
	}

	// �X�^�[�g�{�^���̕\��
	mp_startBtn->RenderAlpha();

	// �t�F�[�h�摜�̕\��
	mp_fade->RenderAlpha();
}
