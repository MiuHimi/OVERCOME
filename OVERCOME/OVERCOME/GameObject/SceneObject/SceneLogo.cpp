//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneLogo.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int SceneLogo::FADEOUT_NEED_SECOND = 2;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toTitleMoveOnChecker(false),
	  mp_background(nullptr), mp_logo(nullptr), mp_fade(nullptr),
	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneLogo::~SceneLogo()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneLogo::Initialize()
{
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
	mp_background->Create(L"Resources\\Images\\black.png", nullptr);
	mp_background->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 1.0f, 1.0f);
	mp_background->SetRect(0.0f, 0.0f, mp_background->GetWidth(), mp_background->GetHeight());

	// ���S�̐���
	mp_logo = std::make_unique<Obj2D>();
	mp_logo->Create(L"Resources\\Images\\Logo\\himi_logo.png", nullptr); // �z�o�[�X�v���C�g�Ȃ�
	if (GetFullScreen())
	{
		mp_logo->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 420.0f * 1.5f, 350.0f * 1.5f, 1.0f, 1.0f);
		mp_logo->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_logo->GetWidth() * 0.5f),
											(windowHeight * 0.5f) - (mp_logo->GetHeight() * 0.5f)));
		float width = mp_logo->GetWidth() / 1.5f;
		float height = mp_logo->GetHeight() / 1.5f;
		mp_logo->SetRect(0, 0, width, height);
		mp_logo->SetScale(1.5f);
	}
	else
	{
		mp_logo->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 420.0f, 350.0f, 1.0f, 1.0f);
		// �E�B���h�E�㕔�������ǉ��ŉ��Z����
		mp_logo->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_logo->GetWidth() * 0.5f),
											(windowHeight * 0.5f) - (mp_logo->GetHeight() * 0.5f) - titlebarHeight));
		mp_logo->SetRect(0.0f, 0.0f, mp_logo->GetWidth(), mp_logo->GetHeight());
	}
	
	// �t�F�[�h�摜�̐���
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());

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
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneLogo::Finalize()
{
	// �s��Ǘ��ϐ��̍폜
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneLogo::Update(DX::StepTimer const& timer)
{
	// �t���[�����J�E���g
	static int frameCount = 0;
	frameCount++;

	// �b�����J�E���g���邽�߂Ɋ���
	int framesPerSecond = 60;
	int elapsedSecond = frameCount / framesPerSecond;

	// �t�F�[�h�A�E�g�J�n
	bool fadeOutFlag = false;
	if (elapsedSecond >= FADEOUT_NEED_SECOND)
	{
		fadeOutFlag = mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
	}

	// �t�F�[�h�A�E�g���I��������
	if (fadeOutFlag)
	{
		m_toTitleMoveOnChecker = true;
	}

	// �V�[���J��
	if (m_toTitleMoveOnChecker)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneLogo::Render()
{
	// �w�i�̕\��
	mp_background->Render();
	//���S�̕\��
	mp_logo->RenderAlphaScale();
	// �t�F�[�h�摜�̕\��
	mp_fade->RenderAlpha();
}
