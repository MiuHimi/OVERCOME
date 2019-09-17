//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2019/09/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneResult.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int SceneResult::SCORE_SIZE = 80;

// �X�R�A���擾�ł���悤�ɂ���
int GameScore::m_score;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneResult::SceneResult(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toTitleMoveOnChecker(false),
	  m_returnToPlayChecker(false),
	  m_resultState(false),
	  m_scoreBasePos(0.0f,0.0f),
	  mp_gameScore(nullptr),
	  mp_fade(nullptr), mp_score(nullptr), mp_bg(nullptr), mp_resultStr(nullptr),
	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneResult::~SceneResult()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneResult::Initialize()
{
	// �V�[���J�ځA���U���g��Ԃ̃t���O�̏�����
	m_toTitleMoveOnChecker = false;
	m_returnToPlayChecker = false;
	m_resultState = false;

	// �X�R�A�̃|�C���^�̏�����
	mp_gameScore = std::make_unique<GameScore>();

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

	// �t�F�[�h�摜�̐���
	mp_fade = std::make_unique<Obj2D>();
	mp_fade->Create(L"Resources\\Images\\black.png", nullptr);
	mp_fade->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_fade->SetRect(0.0f, 0.0f, mp_fade->GetWidth(), mp_fade->GetHeight());

	// �X�R�A�̕\���ʒu�̏�����
	m_scoreBasePos = SimpleMath::Vector2(float((windowWidth * 0.5f)) - float((SCORE_SIZE)*(Digit::NUM * 0.5f)), float(windowHeight * 0.5f) - float(SCORE_SIZE * 0.5f));

	// �X�R�A�I�u�W�F�N�g�̐���
	mp_score = std::make_unique<Obj2D>();
	mp_score->Create(L"Resources\\Images\\ScoreCount\\count_length.png", nullptr);
	mp_score->Initialize(SimpleMath::Vector2(0.0f, 0.0f), float(SCORE_SIZE), float(SCORE_SIZE), 1.0f, 1.0f);
	if (m_isFullScreen)
	{
		mp_score->SetPos(SimpleMath::Vector2(m_scoreBasePos.x,
											 (activeWndRect.bottom - activeWndRect.top) - (10.0f + mp_score->GetHeight())));
	}
	else
	{
		mp_score->SetPos(SimpleMath::Vector2(m_scoreBasePos.x,
											 float(activeWndRect.bottom - (activeWndRect.top + titlebarHeight + 20.0f)) - float(10.0f + mp_score->GetHeight())));
	}
	mp_score->SetRect(0.0f, 0.0f, mp_score->GetWidth(), mp_score->GetHeight());

	// �w�i�̐���
	mp_bg = std::make_unique<Obj2D>();
	mp_bg->Create(L"Resources\\Images\\gray.png", nullptr);
	mp_bg->Initialize(SimpleMath::Vector2(0.0f, 0.0f), windowWidth, windowHeight, 0.0f, 1.0f);
	mp_bg->SetRect(0.0f, 0.0f, mp_bg->GetWidth(), mp_bg->GetHeight());

	// RESULT������̐���
	mp_resultStr = std::make_unique<Obj2D>();
	mp_resultStr->Create(L"Resources\\Images\\Result\\result_image.png", nullptr);
	mp_resultStr->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 275.0f, 50.0f, 1.0f, 1.0f);
	mp_resultStr->SetRect(0.0f, 0.0f, mp_resultStr->GetWidth(), mp_resultStr->GetHeight());
	if (m_isFullScreen)
	{
		mp_resultStr->SetScale(2.0f);
		mp_resultStr->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - ((mp_resultStr->GetWidth() * 2.0f) * 0.5f), mp_resultStr->GetHeight() * 2.0f));
	}
	else
	{
		mp_resultStr->SetPos(SimpleMath::Vector2((windowWidth * 0.5f) - (mp_resultStr->GetWidth() * 0.5f), mp_resultStr->GetHeight()));
	}

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
void SceneResult::Finalize()
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
void SceneResult::Update(DX::StepTimer const& timer)
{
	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();
	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �X�R�A�̍X�V
	mp_gameScore->Update(timer);

	// ���U���g�V�[���̏�Ԃ��擾
	m_resultState = SceneManager::GetResultSceneState();

	// ��ʃN���b�N�܂��̓L�[���͂ŃV�[���J�ڔ���
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED ||
		InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}

	bool isFinishedEffect = false;
	if (m_toTitleMoveOnChecker)
	{
		// ���l��1�łȂ�������
		if (mp_fade->GetAlpha() != 1.0f)
		{
			// �t�F�[�h�A�E�g
			isFinishedEffect = mp_fade->Fade(0.01f, Obj2D::FADE::FADE_OUT);
		}
	}

	// �V�[���J�ڂ��I��������
	if (isFinishedEffect)
	{
		// �^�C�g���ɖ߂�
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneResult::Render()
{
	// �X�R�A�̎擾
	int score = mp_gameScore->GetScore();

	// �X�R�A
	int thousandDigit = score / 1000;		   // 1000�̈�
	int hundredDigit = ((score / 100) % 10);   // 100�̈�
	int tenDigit = ((score % 100) / 10);	   // 10�̈�
	int oneDigit = score % 10;				   // 1�̈�

	// �w�i�̕\��
	mp_bg->Render();

	// RESULT������̕\��
	mp_resultStr->RenderAlphaScale();

	// �X�R�A
	for (int i = 0; i < Digit::NUM; i++)
	{
		// �\���ʒu�ݒ�
		mp_score->SetPos(SimpleMath::Vector2(m_scoreBasePos.x + float(i * SCORE_SIZE), m_scoreBasePos.y));

		RECT rect;
		// ���ɉ����Đ؂���ʒu��ς���
		switch (i)
		{
		// 1000�̈�
		case THOUSAND:
			rect = { int(SCORE_SIZE) * thousandDigit, 0, int(SCORE_SIZE) * thousandDigit + int(SCORE_SIZE), int(SCORE_SIZE) };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// 100�̈�
		case HUNDRED: 
			rect = { SCORE_SIZE * hundredDigit, 0, SCORE_SIZE * hundredDigit + SCORE_SIZE, SCORE_SIZE };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// 10�̈�
		case TEN:
			rect = { SCORE_SIZE * tenDigit, 0, SCORE_SIZE * tenDigit + SCORE_SIZE, SCORE_SIZE };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// 1�̈�
		case ONE:
			rect = { SCORE_SIZE * oneDigit, 0, SCORE_SIZE * oneDigit + SCORE_SIZE, SCORE_SIZE };
			mp_score->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
		// ����ȊO�̈�
		default:
			break;
		}

		mp_score->Render();
	}

	// �t�F�[�h�摜�̕\��
	mp_fade->RenderAlpha();
}
