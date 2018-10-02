//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../../Game.h"

#include "../MyCamera.h"

#include "../Player.h"

#include "../GameFloor.h"
#include "../GameRoad.h"

#include "../SkyDome.h"

#include "../GameTimer.h"
#include "../GameScore.h"

class ScenePlay : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toResultMoveOnChecker;                 // ���U���g�V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool m_returnToTitleChecker;                  // �^�C�g���V�[���ɖ߂�邩�ǂ����̃`�F�b�N

	Game* mp_game;                                // Game�t�@�C���̏����i�[
	std::unique_ptr<DirectX::SpriteFont> m_font;  // �X�v���C�g�t�H���g

	std::unique_ptr<MyCamera> mp_camera;          // �J�����I�u�W�F�N�g

	std::unique_ptr<GameFloor> mp_gameFloor;	  // �Q�[�����I�u�W�F�N�g
	std::unique_ptr<GameRoad>  mp_gameRoad;	      // �Q�[�����H�I�u�W�F�N�g

	static std::unique_ptr<Player> mp_player;     // �v���C���[�I�u�W�F�N�g

	std::unique_ptr<SkyDome> mp_skydome;          // �X�J�C�h�[���I�u�W�F�N�g

	std::unique_ptr<GameTimer> mp_gameTimer;      // �������ԃI�u�W�F�N�g
	std::unique_ptr<GameScore> mp_gameScore;      // �X�R�A�I�u�W�F�N�g

	//bool m_hitPlayerToFloorFlag = false;
	//bool m_hitPlayerToRoadFlag = false;

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	ScenePlay(SceneManager* sceneManager);
	ScenePlay(Game* game, SceneManager* sceneManager);
	// �f�X�g���N�^
	~ScenePlay();

	// ������
	virtual void Initialize() override;
	// �X�V
	//virtual void Update(DX::StepTimer const& timer) override;
	virtual void Update(DX::StepTimer const& timer, Game* game) override;
	// �`��
	//virtual void Render() override;
	virtual void Render(DirectX::SpriteBatch* sprites, Game* game) override;
	// �I��
	virtual void Finalize() override;

	static Player* GetPlayer() { return mp_player.get(); }

};
