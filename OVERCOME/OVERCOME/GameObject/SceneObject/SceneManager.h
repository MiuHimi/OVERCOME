//////////////////////////////////////////////////////////////
// File.    SceneManager.h
// Summary. SceneManagerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"


// �N���X�̐錾
class SceneBase;

// �V�[��ID
enum SceneId
{
	SCENE_LOGO,
	SCENE_TITLE,
	SCENE_SELECTSTAGE,
	SCENE_PLAY,
	SCENE_RESULT,

	NUM_SCENES
};


// �V�[���Ǘ��N���X
class Game;
class SceneManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public :
	static int            m_stageNum;                  // �X�e�[�W�ԍ�
	static bool           m_clearSceneState;           // ���U���g�V�[���̏�Ԃ����߂�

private:
	// ���N���X�ւ̃|�C���^
	//SceneBase*            mp_scenes[NUM_SCENES];       // �o�^����Ă���V�[���̃��X�g
	SceneBase*            mp_scene;                    // �V�[�����ݒ肳���

	SceneId               m_nextScene;                 // ���ɍX�V����V�[��������
	bool                  m_requestSceneFlag;          // �V�[���J�ڂ�v�����ꂽ��t���O������

	/*SceneBase* mp_activeScene;           // �X�V���̃V�[��
													   SceneBase* mp_requestedScene;        // �v�����ꂽ�V�[��*/


// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneManager(SceneId startSceneId);
	SceneManager(Game* game, SceneId startSceneId);
	// �f�X�g���N�^
	~SceneManager();

	// �X�V���̃V�[���̍X�V����
	void UpdateActiveScene(DX::StepTimer const& timer, Game* game);
	// �X�V���̃V�[���̕`�揈��
	void RenderActiveScene();
	// �J�ڂ������V�[����v��
	bool RequestToChangeScene(SceneId sceneId);

	// �X�e�[�W�ԍ��̐ݒ�A�擾
	static void SetStageNum(int stageID)         { m_stageNum = stageID; }
	static int GetStageNum()                     { return m_stageNum; }

	// ���U���g�V�[���̏�Ԃ̐ݒ�A�擾
	static void SetResultSceneState(bool state)  { m_clearSceneState = state; }
	static bool GetResultSceneState()            { return m_clearSceneState; }

private:
	// �V�[����J�ڂ���
	void ChangeScene(Game* game);

};
