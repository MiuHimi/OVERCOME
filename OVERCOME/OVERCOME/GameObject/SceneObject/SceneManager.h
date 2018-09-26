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
	SCENE_PLAY,
	SCENE_RESULT,

	NUM_SCENES
};


// �V�[���Ǘ��N���X
class Game;
class SceneManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
private:
	// ���N���X�ւ̃|�C���^
	SceneBase* mp_scenes[NUM_SCENES];    // �o�^����Ă���V�[���̃��X�g

	SceneBase* mp_activeScene;           // �X�V���̃V�[��
	SceneBase* mp_requestedScene;        // �v�����ꂽ�V�[��

	Game* mp_game;                       // Game�t�@�C���̏����i�[


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

private:
	// �V�[����J�ڂ���
	void ChangeScene();

};
