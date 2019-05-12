//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneTitle::SceneTitle(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  mp_matrixManager(nullptr)
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
	m_toPlayMoveOnChecker = false;

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
/// �^�C�g���V�[���̏I������
/// </summary>
void SceneTitle::Finalize()
{
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

}

/// <summary>
/// �^�C�g���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneTitle::Update(DX::StepTimer const& timer)
{
	// �}�E�X�̍X�V
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
	InputManager::SingletonGetInstance().Update();

	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
	}

}

/// <summary>
/// �^�C�g���V�[���̕`�揈��
/// </summary>
void SceneTitle::Render()
{
	GameDebug::SingletonGetInstance().DebugRender("SceneTitle", SimpleMath::Vector2(10.0f, 10.0f));
	GameDebug::SingletonGetInstance().Render();
}
