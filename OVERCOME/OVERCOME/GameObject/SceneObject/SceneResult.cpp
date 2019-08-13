//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2019/06/12
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
	  m_scorePos(0.0f,0.0f),
	  mp_gameScore(nullptr),
	  mp_textureScore(nullptr),
	  mp_textureBackground(nullptr),
	  mp_sprite(nullptr),
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

	// �X�R�A�̕\���ʒu�̏�����
	m_scorePos = SimpleMath::Vector2(280.0f, 400.0f);

	// �X�R�A�̃|�C���^�̏�����
	mp_gameScore = std::make_unique<GameScore>();

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_length.png", nullptr, mp_textureScore.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Result\\result_background_image.png", nullptr, mp_textureBackground.GetAddressOf());

	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

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

	// ��ʃN���b�N�ŃV�[���J�ڔ���
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toTitleMoveOnChecker = true;
	}
	// �L�[���͂ł��V�[���J�ڔ���
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}

	// �V�[���J�ڂ�����������
	if (m_toTitleMoveOnChecker)
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

	// ���U���g��ʂ̕`��
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// �w�i
	RECT rectBG = { 0, 0, 800, 600 };
	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectBG, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// �X�R�A
	for (int i = 0; i < Digit::NUM; i++)
	{
		RECT rect;
		// ���ɉ����Đ؂���ʒu��ς���
		switch (i)
		{
		// 1000�̈�
		case THOUSAND:
			rect = { int(SCORE_SIZE) * thousandDigit, 0, int(SCORE_SIZE) * thousandDigit + int(SCORE_SIZE), int(SCORE_SIZE) };
			break;
		// 100�̈�
		case HUNDRED: 
			rect = { SCORE_SIZE * hundredDigit, 0, SCORE_SIZE * hundredDigit + SCORE_SIZE, SCORE_SIZE }; 
			break;
		// 10�̈�
		case TEN:
			rect = { SCORE_SIZE * tenDigit, 0, SCORE_SIZE * tenDigit + SCORE_SIZE, SCORE_SIZE };
			break;
		// 1�̈�
		case ONE:
			rect = { SCORE_SIZE * oneDigit, 0, SCORE_SIZE * oneDigit + SCORE_SIZE, SCORE_SIZE };
			break;
		// ����ȊO�̈�
		default:
			break;
		}

		mp_sprite->Draw(mp_textureScore.Get(), SimpleMath::Vector2(m_scorePos.x + float(i * SCORE_SIZE), m_scorePos.y - (float)SCORE_SIZE / 2.0f), &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
}
