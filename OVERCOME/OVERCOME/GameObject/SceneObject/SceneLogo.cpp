//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneLogo.h"

#include "../../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(SceneManager * sceneManager)
	               : SceneBase(sceneManager)
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
	m_toTitleMoveOnChecker = false;

	m_changeSceneNeedTime = 2;
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneLogo::Finalize()
{
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneLogo::Update(DX::StepTimer const& timer)
{
	// �t���[�����J�E���g
	static int count = 0;
	count++;

	if (count / 60 >= m_changeSceneNeedTime)
	{
		m_toTitleMoveOnChecker = true;
		count = 0;
	}

	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneLogo::Render()
{
	// �f�o�b�O�p
	GameDebug::SingletonGetInstance().DebugRender("SceneLogo", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
}
