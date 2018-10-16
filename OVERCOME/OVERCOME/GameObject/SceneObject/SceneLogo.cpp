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
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(SceneManager* sceneManager) 
	               : SceneBase(sceneManager)
{
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(Game * game, SceneManager * sceneManager)
	               : mp_game(game)
	               , SceneBase(sceneManager)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneLogo::~SceneLogo()
{
	/*delete mp_game;
	mp_game = nullptr;*/
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneLogo::Initialize()
{
	m_toTitleMoveOnChecker = false;

	m_count = 0;
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
void SceneLogo::Update(DX::StepTimer const& timer, Game* game)
{
	m_count++;

	if (m_count == 120)
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
//void SceneLogo::Render()
//{
//}
void SceneLogo::Render(Game* game)
{
	// �f�o�b�O�p
	GameDebug::SingletonGetInstance().DebugRender("SceneLogo", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
}
