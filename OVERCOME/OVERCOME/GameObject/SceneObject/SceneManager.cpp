//////////////////////////////////////////////////////////////
// File.    SceneManager.cpp
// Summary. SceneManagerClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
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
	: mp_scene(nullptr)
	, m_nextScene(SceneId::SCENE_LOGO)
	, m_requestSceneFlag(false)
	, m_gameStateFlag(true)
{
	// �J�n�V�[���̐ݒ�
	RequestToChangeScene(startSceneId);

	switch (startSceneId)
	{
	case SCENE_LOGO:          mp_scene = new SceneLogo(this);          break;
	case SCENE_TITLE:         mp_scene = new SceneTitle(this);         break;
	case SCENE_SELECTSTAGE:   mp_scene = new SceneStageSelect(this);   break;
	case SCENE_PLAY:          mp_scene = new ScenePlay(this);          break;
	case SCENE_RESULT:        mp_scene = new SceneResult(this);        break;
	}
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneManager::~SceneManager()
{
	delete mp_scene;
	mp_scene = nullptr;
}

/// <summary>
/// �X�V���̃V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneManager::UpdateActiveScene(DX::StepTimer const& timer)
{
	// �V�[���̗v������������
	if (mp_scene != nullptr && m_requestSceneFlag == true)
	{
		// �V�[���̕ύX
		ChangeScene();

		// �V�[���J�ڂ̗v������
		m_requestSceneFlag = false;
	}

	// �X�V���̃V�[������������
	if (mp_scene != nullptr && m_requestSceneFlag == false)
	{
		// �V�[���̍X�V
		mp_scene->Update(timer);
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
void SceneManager::RenderActiveScene()
{
	// �X�V���̃V�[������������
	if (mp_scene != nullptr)
	{
		// �V�[���̕`��
		mp_scene->Render();
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
	// �J�ڂ������V�[����ݒ�
	m_nextScene = sceneId;

	// �V�[���J�ڔ���
	m_requestSceneFlag = true;

	if (!m_nextScene) return false;
	return true;

}

/// <summary>
/// �V�[���̍폜
/// </summary>
void SceneManager::DeleteScene()
{
	// �������̃V�[�����I��������
	if (mp_scene != nullptr)
	{
		mp_scene->Finalize();
		delete mp_scene;
		mp_scene = nullptr;
	}
}

/// <summary>
/// �����V�[���̕ύX
/// </summary>
void SceneManager::ChangeScene()
{
	// �������̃V�[�����I��������
	if (mp_scene != nullptr)
	{
		mp_scene->Finalize();
		delete mp_scene;
		mp_scene = nullptr;
	}

	// ���̃V�[����ݒ�
	switch (m_nextScene)
	{
	case SCENE_LOGO:          mp_scene = new SceneLogo(this);          m_activeScene = SCENE_LOGO;          break;
	case SCENE_TITLE:         mp_scene = new SceneTitle(this);         m_activeScene = SCENE_TITLE;         break;
	case SCENE_SELECTSTAGE:   mp_scene = new SceneStageSelect(this);   m_activeScene = SCENE_SELECTSTAGE;   break;
	case SCENE_PLAY:          mp_scene = new ScenePlay(this);          m_activeScene = SCENE_PLAY;          break;
	case SCENE_RESULT:        mp_scene = new SceneResult(this);        m_activeScene = SCENE_RESULT;        break;
	}
	
	// �V���Ɋ������ɂȂ����V�[��������������
	if (mp_scene != nullptr)
	{
		mp_scene->Initialize();
	}
}