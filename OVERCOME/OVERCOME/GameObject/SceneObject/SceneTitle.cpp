//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneTitle.h"

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
	: SceneBase(sceneManager)
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

	/*std::unique_ptr<DX::DeviceResources> dr;
	dr = std::make_unique<DX::DeviceResources>();
	dr->GetD3DDevice();*/
}
/// <summary>
/// �^�C�g���V�[���̏I������
/// </summary>
void SceneTitle::Finalize()
{
}

/// <summary>
/// �^�C�g���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneTitle::Update(DX::StepTimer const& timer)
{
	InputManager::SingletonGetInstance().Update();
	// ���͏����X�V
	//InputManager::GetInstance().Update();
	// �L�[����
	//if (InputManager::GetInstance().GetTracker().leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
	if (/*InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space)*/InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
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
	// �f�o�b�O�p
	GameDebug::SingletonGetInstance().DebugRender("SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 10.0f));
	GameDebug::SingletonGetInstance().DebugRender("SPACEkey to SceneSelectStage", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
}
