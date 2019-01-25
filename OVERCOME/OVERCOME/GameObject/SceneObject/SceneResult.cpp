//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneResult.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"
#include "../../Utility/DrawManager.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

int GameScore::m_score;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneResult::SceneResult(SceneManager * sceneManager)
	: SceneBase(sceneManager)
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
	m_toTitleMoveOnChecker = false;

	for (int i = 0; i < 3; i++)
	{
		DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\count\\count_length.png", nullptr, m_textureScore[i].GetAddressOf());
	}

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Result\\result_background_image.png", nullptr, m_textureBackground.GetAddressOf());

	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();

	// �r���[�s��̍쐬
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float fovAngleY = XMConvertToRadians(45.0f);

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

	// ���U���g�V�[���̏�Ԃ�ݒ�
	m_resultState = SceneManager::GetResultSceneState();

	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toTitleMoveOnChecker = true;
	}

	// �L�[����
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}
	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneResult::Render()
{
	int score = GameScore::GetScore();

	// �w�i
	DrawManager::SingletonGetInstance().Draw(m_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f));

	// �X�R�A
	int hundredDigit = score / 100;
	int tenDigit = ((score % 100) / 10);
	int oneDigit = score % 10;

	/*for (int i = 0; i < 3; i++)
	{
		RECT rect;
		// �؂���ꏊ��ݒ�
		if (i = 0)
		{
			rect.top = LONG(0.0f);
			rect.left = LONG(m_scoreNumSize * hundredDigit);
			rect.right = LONG(m_scoreNumSize * hundredDigit + m_scoreNumSize);
			rect.bottom = LONG(m_scoreNumSize);
			DrawManager::SingletonGetInstance().DrawRect(m_textureScore[i].Get(), DirectX::SimpleMath::Vector2(320 + (i * float(m_scoreNumSize)), 400.0f - m_scoreNumSize / 2), &rect);

		}
		if (i = 1)
		{
			rect.top = LONG(0.0f);
			rect.left = LONG(m_scoreNumSize * tenDigit);
			rect.right = LONG(m_scoreNumSize * tenDigit + m_scoreNumSize);
			rect.bottom = LONG(m_scoreNumSize);
			DrawManager::SingletonGetInstance().DrawRect(m_textureScore[i].Get(), DirectX::SimpleMath::Vector2(320 + (i * float(m_scoreNumSize)), 400.0f - m_scoreNumSize / 2), &rect);

		}
		if (i = 2)
		{
			rect.top = LONG(0.0f);
			rect.left = LONG(m_scoreNumSize * oneDigit);
			rect.right = LONG(m_scoreNumSize * oneDigit + m_scoreNumSize);
			rect.bottom = LONG(m_scoreNumSize);
			DrawManager::SingletonGetInstance().DrawRect(m_textureScore[i].Get(), DirectX::SimpleMath::Vector2(320 + (i * float(m_scoreNumSize)), 400.0f - m_scoreNumSize / 2), &rect);

		}
		
	}*/

	RECT rect;

	rect.top = LONG(0.0f);
	rect.left = LONG(m_scoreNumSize * hundredDigit);
	rect.right = LONG(m_scoreNumSize * hundredDigit + m_scoreNumSize);
	rect.bottom = LONG(m_scoreNumSize);
	DrawManager::SingletonGetInstance().DrawRect(m_textureScore[0].Get(), DirectX::SimpleMath::Vector2(320.0f, 400.0f - m_scoreNumSize / 2), &rect);

	rect.top = LONG(0.0f);
	rect.left = LONG(m_scoreNumSize * tenDigit);
	rect.right = LONG(m_scoreNumSize * tenDigit + m_scoreNumSize);
	rect.bottom = LONG(m_scoreNumSize);
	DrawManager::SingletonGetInstance().DrawRect(m_textureScore[0].Get(), DirectX::SimpleMath::Vector2(400.0f, 400.0f - m_scoreNumSize / 2), &rect);

	rect.top = LONG(0.0f);
	rect.left = LONG(m_scoreNumSize * oneDigit);
	rect.right = LONG(m_scoreNumSize * oneDigit + m_scoreNumSize);
	rect.bottom = LONG(m_scoreNumSize);
	DrawManager::SingletonGetInstance().DrawRect(m_textureScore[0].Get(), DirectX::SimpleMath::Vector2(480.0f, 400.0f - m_scoreNumSize / 2), &rect);

	// �f�o�b�O�p
	//GameDebug::SingletonGetInstance().DebugRender("SceneResult", DirectX::SimpleMath::Vector2(20.0f, 10.0f));
	//if (m_resultState == true) GameDebug::SingletonGetInstance().DebugRender("Clear", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	//if (m_resultState == false)GameDebug::SingletonGetInstance().DebugRender("GameOver", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	//GameDebug::SingletonGetInstance().DebugRender("SPACEkey to SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	//GameDebug::SingletonGetInstance().Render();
}
