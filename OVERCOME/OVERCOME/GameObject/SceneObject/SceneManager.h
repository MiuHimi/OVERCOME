//////////////////////////////////////////////////////////////
// File.    SceneManager.h
// Summary. SceneManagerClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

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

class SceneBase;
// �V�[���Ǘ��N���X
class SceneManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public :
	//static int            m_stageID;                   // �X�e�[�W�ԍ�
	static bool           m_clearSceneState;           // ���U���g�V�[���̏��

	static const int      MAXSTAGE;                    // �S�X�e�[�W��

private:
	// ���N���X�ւ̃|�C���^
	SceneBase*            mp_scene;                    // �V�[�����ݒ肳���

	static SceneId        m_activeScene;               // �A�N�e�B�u�ȃV�[����ۑ�
	SceneId               m_nextScene;                 // ���ɍX�V����V�[��������
	bool                  m_requestSceneFlag;          // �V�[���J�ڂ�v�����ꂽ��t���O������

	bool                  m_gameStateFlag;             // �Q�[���̍X�V���̓t���O������

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneManager(SceneId startSceneId);
	// �f�X�g���N�^
	~SceneManager();

	// �X�V���̃V�[���̍X�V����
	void UpdateActiveScene(DX::StepTimer const& timer);
	// �X�V���̃V�[���̕`�揈��
	void RenderActiveScene();
	// �J�ڂ������V�[����v��
	bool RequestToChangeScene(SceneId sceneId);
	// �V�[�����폜
	void DeleteScene();

	//-----------------------------------Getter-----------------------------------//

	// �X�e�[�W�ԍ��̎擾
	//static int  GetStageNum()					{ return m_stageID; }

	// ���U���g�V�[���̏�Ԃ̎擾
	static bool GetResultSceneState()			{ return m_clearSceneState; }

	// �A�N�e�B�u�ȃV�[���̎擾
	static SceneId GetActiveScene()				{ return m_activeScene; }

	// �Q�[���̍X�V���t���O�̎擾
	bool GetGameState()							{ return m_gameStateFlag; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	// �X�e�[�W�ԍ��̐ݒ�A�擾
	//static void SetStageNum(int stageID)		{ m_stageID = stageID; }

	// ���U���g�V�[���̏�Ԃ̐ݒ�A�擾
	static void SetResultSceneState(bool state) { m_clearSceneState = state; }

	// �Q�[���̍X�V���t���O�̐ݒ�
	void SetGameState(bool flag)				{ m_gameStateFlag = flag; }
	//----------------------------------------------------------------------------//

private:
	// �V�[����J�ڂ���
	void ChangeScene();

};
