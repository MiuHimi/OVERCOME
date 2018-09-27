//////////////////////////////////////////////////////////////
// File.    SceneManager.h
// Summary. SceneManagerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../DeviceResources.h"
#include "../../StepTimer.h"

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
	static int m_stageNum;

private:
	// ���N���X�ւ̃|�C���^
	//SceneBase* mp_scenes[NUM_SCENES];    // �o�^����Ă���V�[���̃��X�g
	SceneBase* mp_scene;                 // �V�[�����ݒ肳���

	//SceneBase* mp_activeScene;           // �X�V���̃V�[��
	//SceneBase* mp_requestedScene;        // �v�����ꂽ�V�[��

	SceneId m_nextScene;                 // ���ɍX�V����V�[��������
	bool m_requestSceneFlag;             // �V�[���J�ڂ�v�����ꂽ��t���O������


// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneManager(SceneId startSceneId);
	SceneManager(Game* game, SceneId startSceneId);
	// �f�X�g���N�^
	~SceneManager();

	// �X�V���̃V�[���̍X�V����
	void UpdateActiveScene(DX::StepTimer const& timer);
	void UpdateActiveScene(DX::StepTimer const& timer, Game* game);
	// �X�V���̃V�[���̕`�揈��
	void RenderActiveScene(DirectX::SpriteBatch* sprite, Game* game);
	// �J�ڂ������V�[����v��
	bool RequestToChangeScene(SceneId sceneId);

	// �X�e�[�W�ԍ��̐ݒ�A�擾
	static void SetStageNum(int stageID) { m_stageNum = stageID; }
	static int GetStageNum()              { return m_stageNum; }

private:
	// �V�[����J�ڂ���
	void ChangeScene(Game* game);

};
