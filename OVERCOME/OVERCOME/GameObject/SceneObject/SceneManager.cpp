//////////////////////////////////////////////////////////////
// File.    SceneManager.cpp
// Summary. SceneManagerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "ScenePlay.h"
#include "SceneResult.h"

// using�f�B���N�g��



/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="startSceneId">�J�n�V�[����ID</param>
SceneManager::SceneManager(SceneId startSceneId)
	                     : mp_activeScene(nullptr)
	                     , mp_requestedScene(nullptr)
{
	// �V�[�����X�g�̏�����
	for (int i = 0; i < NUM_SCENES; i++)
	{
		mp_scenes[i] = nullptr;
	}

	// �V�[���I�u�W�F�N�g�����X�g�ɓo�^
	mp_scenes[SCENE_LOGO] = new SceneLogo(mp_game, this);
	mp_scenes[SCENE_TITLE] = new SceneTitle(mp_game, this);
	mp_scenes[SCENE_PLAY] = new ScenePlay(mp_game, this);

	// �J�n�V�[���̐ݒ�
	RequestToChangeScene(startSceneId);
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">Game�t�@�C���̏��</param>
/// <param name="startSceneId">�J�n�V�[����ID</param>
SceneManager::SceneManager(Game* game, SceneId startSceneId)
	: mp_activeScene(nullptr)
	, mp_requestedScene(nullptr)
{
	// �V�[�����X�g�̏�����
	for (int i = 0; i < NUM_SCENES; i++)
	{
		mp_scenes[i] = nullptr;
	}

	// �V�[���I�u�W�F�N�g�����X�g�ɓo�^
	mp_scenes[SCENE_LOGO] = new SceneLogo(game, this);
	mp_scenes[SCENE_TITLE] = new SceneTitle(game, this);
	mp_scenes[SCENE_PLAY] = new ScenePlay(game, this);
	mp_scenes[SCENE_RESULT] = new SceneResult(game, this);

	// �J�n�V�[���̐ݒ�
	RequestToChangeScene(startSceneId);
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneManager::~SceneManager()
{
	mp_activeScene = nullptr;
	mp_requestedScene = nullptr;

	for (int i = 0; i < NUM_SCENES; i++)
	{
		delete mp_scenes[i];
		mp_scenes[i] = nullptr;
	}
}

/// <summary>
/// �X�V���̃V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneManager::UpdateActiveScene(DX::StepTimer const& timer, Game* game)
{
	// �V�[���̗v������������
	if (mp_requestedScene != nullptr)
	{
		// �V�[���̕ύX
		ChangeScene();
	}

	// �X�V���̃V�[������������
	if (mp_activeScene != nullptr)
	{
		// �V�[���̍X�V
		//mp_activeScene->Update(timer);
		mp_activeScene->Update(timer, game);
	}
}

/// <summary>
/// �������̃V�[���̕`�揈��
/// </summary>
void SceneManager::RenderActiveScene(DirectX::SpriteBatch* sprites, Game* game)
{
	// �X�V���̃V�[������������
	if (mp_activeScene != nullptr)
	{
		// �V�[���̕`��
		mp_activeScene->Render(sprites, game);
	}
}

/// <summary>
/// �V�[���̕ύX��v��
/// </summary>
/// <param name="sceneId">�ύX��v������V�[����ID</param>
/// <returns>�v�����ʂ����ꍇ true, �ʂ�Ȃ��ꍇ false</returns>
bool SceneManager::RequestToChangeScene(SceneId sceneId)
{
	if (sceneId >= 0 && sceneId < NUM_SCENES)
	{
		mp_requestedScene = mp_scenes[sceneId];
		return true;
	}
	else
	{
		mp_requestedScene = nullptr;
		return false;
	}
}

/// <summary>
/// �����V�[���̕ύX
/// </summary>
void SceneManager::ChangeScene()
{
	// �������̃V�[�����I��������
	if (mp_activeScene != nullptr)
	{
		mp_activeScene->Finalize();
	}

	// �v�����ꂽ�V�[�����������ɂ���
	mp_activeScene = mp_requestedScene;
	mp_requestedScene = nullptr;

	// �V���Ɋ������ɂȂ����V�[��������������
	if (mp_activeScene != nullptr)
	{
		mp_activeScene->Initialize();
	}
}