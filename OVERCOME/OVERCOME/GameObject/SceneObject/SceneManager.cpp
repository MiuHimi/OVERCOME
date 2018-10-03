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
#include "SceneStageSelect.h"
#include "ScenePlay.h"
#include "SceneResult.h"

// using�f�B���N�g��



/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="startSceneId">�J�n�V�[����ID</param>
SceneManager::SceneManager(SceneId startSceneId)
	                     : /*mp_activeScene(nullptr)
	                     , mp_requestedScene(nullptr)
	                     , */mp_scene(nullptr)
{
	// �V�[�����X�g�̏�����
	/*for (int i = 0; i < NUM_SCENES; i++)
	{
		mp_scenes[i] = nullptr;
	}*/

	// �V�[���I�u�W�F�N�g�����X�g�ɓo�^
	/*mp_scenes[SCENE_LOGO] = new SceneLogo(mp_game, this);
	mp_scenes[SCENE_TITLE] = new SceneTitle(mp_game, this);
	mp_scenes[SCENE_SELECTSTAGE] = new SceneStageSelect(mp_game, this);
	mp_scenes[SCENE_PLAY] = new ScenePlay(mp_game, this);
	mp_scenes[SCENE_RESULT] = new SceneResult(mp_game, this);*/

	// �J�n�V�[���̐ݒ�
	RequestToChangeScene(startSceneId);
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">Game�t�@�C���̏��</param>
/// <param name="startSceneId">�J�n�V�[����ID</param>
SceneManager::SceneManager(Game* game, SceneId startSceneId)
	: /*mp_activeScene(nullptr)
	, mp_requestedScene(nullptr)
	, */mp_scene(nullptr)
	, m_nextScene(SceneId::SCENE_LOGO)
	, m_requestSceneFlag(false)
{
	// �V�[�����X�g�̏�����
	/*for (int i = 0; i < NUM_SCENES; i++)
	{
		mp_scenes[i] = nullptr;
	}*/

	// �V�[���I�u�W�F�N�g�����X�g�ɓo�^
	/*mp_scenes[SCENE_LOGO] = new SceneLogo(game, this);
	mp_scenes[SCENE_TITLE] = new SceneTitle(game, this);
	mp_scenes[SCENE_SELECTSTAGE] = new SceneStageSelect(game, this);
	mp_scenes[SCENE_PLAY] = new ScenePlay(game, this);
	mp_scenes[SCENE_RESULT] = new SceneResult(game, this);*/

	// �J�n�V�[���̐ݒ�
	RequestToChangeScene(startSceneId);

	switch (startSceneId)
	{
	case SCENE_LOGO:          mp_scene = new SceneLogo(game, this);          break;
	case SCENE_TITLE:         mp_scene = new SceneTitle(game, this);         break;
	case SCENE_SELECTSTAGE:   mp_scene = new SceneStageSelect(game, this);   break;
	case SCENE_PLAY:          mp_scene = new ScenePlay(game, this);          break;
	case SCENE_RESULT:        mp_scene = new SceneResult(game, this);        break;
	}
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneManager::~SceneManager()
{
	/*mp_activeScene = nullptr;
	mp_requestedScene = nullptr;*/

	/*for (int i = 0; i < NUM_SCENES; i++)
	{
		delete mp_scenes[i];
		mp_scenes[i] = nullptr;
	}*/

	delete mp_scene;
	mp_scene = nullptr;
}

/// <summary>
/// �X�V���̃V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneManager::UpdateActiveScene(DX::StepTimer const& timer, Game* game)
{
	// �V�[���̗v������������
	//if (mp_requestedScene != nullptr)
	//{
	//	// �V�[���̕ύX
	//	ChangeScene();
	//}

	// �X�V���̃V�[������������
	//if (mp_activeScene != nullptr)
	//{
	//	// �V�[���̍X�V
	//	//mp_activeScene->Update(timer);
	//	mp_activeScene->Update(timer, game);
	//}

	// �V�[���̗v������������
	if (mp_scene != nullptr && m_requestSceneFlag == true)
	{
		// �V�[���̕ύX
		ChangeScene(game);

		// �V�[���J�ڂ̗v������
		m_requestSceneFlag = false;
	}

	// �X�V���̃V�[������������
	if (mp_scene != nullptr && m_requestSceneFlag == false)
	{
		// �V�[���̍X�V
		mp_scene->Update(timer, game);
	}

	// �V�[���̏�񂪂킩��Ȃ�������
	if(mp_scene == nullptr)
	{
		// �G���[
		throw std::range_error("Not Find Active Update Scene.");
	}
}

/// <summary>
/// �������̃V�[���̕`�揈��
/// </summary>
void SceneManager::RenderActiveScene(DirectX::SpriteBatch* sprites, Game* game)
{
	// �X�V���̃V�[������������
	/*if (mp_activeScene != nullptr)
	{
		// �V�[���̕`��
		mp_activeScene->Render(sprites, game);
	}*/

	// �X�V���̃V�[������������
	if (mp_scene != nullptr)
	{
		// �V�[���̕`��
		mp_scene->Render(sprites, game);
	}
	else if (mp_scene == nullptr)
	{
		// �X�V���̃V�[�����Ȃ�������G���[
		throw std::range_error("Not Find Active Render Scene.");
	}
}

/// <summary>
/// �V�[���̕ύX��v��
/// </summary>
/// <param name="sceneId">�ύX��v������V�[����ID</param>
/// <returns>�v�����ʂ����ꍇ true, �ʂ�Ȃ��ꍇ false</returns>
bool SceneManager::RequestToChangeScene(SceneId sceneId)
{
	/*if (sceneId >= 0 && sceneId < NUM_SCENES)
	{
		mp_requestedScene = mp_scenes[sceneId];
		return true;
	}
	else
	{
		mp_requestedScene = nullptr;
		return false;
	}*/

	// �J�ڂ������V�[����ݒ�
	m_nextScene = sceneId;

	// �V�[���J�ڔ���
	m_requestSceneFlag = true;

	return true;

}

/// <summary>
/// �����V�[���̕ύX
/// </summary>
void SceneManager::ChangeScene(Game* game)
{
	// �������̃V�[�����I��������
	/*if (mp_activeScene != nullptr)
	{
		mp_activeScene->Finalize();
	}*/
	// �v�����ꂽ�V�[�����������ɂ���
	/*mp_activeScene = mp_requestedScene;
	mp_requestedScene = nullptr;*/
	// �V���Ɋ������ɂȂ����V�[��������������
	/*if (mp_activeScene != nullptr)
	{
		mp_activeScene->Initialize();
	}*/


	// �������̃V�[�����I��������
	if (mp_scene != nullptr)
	{
		mp_scene->Finalize();
		delete mp_scene;
		mp_scene = nullptr;
	}

	switch (m_nextScene)
	{
	case SCENE_LOGO:          mp_scene = new SceneLogo(game, this);          break;
	case SCENE_TITLE:         mp_scene = new SceneTitle(game, this);         break;
	case SCENE_SELECTSTAGE:   mp_scene = new SceneStageSelect(game, this);   break;
	case SCENE_PLAY:          mp_scene = new ScenePlay(game, this);          break;
	case SCENE_RESULT:        mp_scene = new SceneResult(game, this);        break;
	}
	
	// �V���Ɋ������ɂȂ����V�[��������������
	if (mp_scene != nullptr)
	{
		mp_scene->Initialize();
	}
}