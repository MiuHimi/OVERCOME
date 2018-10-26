//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager)
	: SceneBase(sceneManager)
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
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneStageSelect::Finalize()
{
}

/// <summary>
/// �X�e�[�W�I���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();

	static int count = 0;
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	{
		count++;
		if (count > 3)count = 0;
		SceneManager::SetStageNum(count);
	}

	// �L�[����
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Z))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneStageSelect::Render()
{
	// �f�o�b�O�p
	GameDebug::SingletonGetInstance().DebugRender("SceneStageSelect", DirectX::SimpleMath::Vector2(20.0f, 10.0f));

	if (SceneManager::GetStageNum() != 1 && SceneManager::GetStageNum() != 2)
	{
		GameDebug::SingletonGetInstance().DebugRender("StageNone", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	}
	else 
	{
		GameDebug::SingletonGetInstance().DebugIntRender("Stage", SceneManager::GetStageNum(), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
	}
}
