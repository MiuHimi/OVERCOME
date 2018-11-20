//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneResult.h"

#include "../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


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
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneResult::Finalize()
{
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneResult::Update(DX::StepTimer const& timer)
{
	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();

	// ���U���g�V�[���̏�Ԃ�ݒ�
	m_resultState = SceneManager::GetResultSceneState();

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
	MatrixManager::SingletonGetInstance().SetViewProjection(view, projection);

	// �f�o�b�O�p
	GameDebug::SingletonGetInstance().DebugRender("SceneResult", DirectX::SimpleMath::Vector2(20.0f, 10.0f));
	if (m_resultState == true) GameDebug::SingletonGetInstance().DebugRender("Clear", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	if (m_resultState == false)GameDebug::SingletonGetInstance().DebugRender("GameOver", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	GameDebug::SingletonGetInstance().DebugRender("SPACEkey to SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	GameDebug::SingletonGetInstance().Render();
}
