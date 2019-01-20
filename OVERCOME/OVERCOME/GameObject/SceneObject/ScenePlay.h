//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"

#include "../../Game.h"

#include "../Player.h"
#include "../GameCamera.h"

#include "../GameFloor.h"
#include "../GameRoad.h"
#include "../GameTarget.h"

//#include "../SkyDome.h"

#include "../GameTimer.h"
#include "../GameScore.h"

class MatrixManager;
class ScenePlay : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toResultMoveOnChecker;                 // ���U���g�V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool m_returnToTitleChecker;                  // �^�C�g���V�[���ɖ߂�邩�ǂ����̃`�F�b�N

	std::unique_ptr<GameCamera> mp_camera;        // �J�����I�u�W�F�N�g

	std::unique_ptr<GameFloor> mp_gameFloor;	  // �Q�[�����I�u�W�F�N�g
	std::unique_ptr<GameRoad>  mp_gameRoad;	      // �Q�[�����H�I�u�W�F�N�g
	std::unique_ptr<GameTarget> mp_gameTarget;    // �Q�[���W�I�I�u�W�F�N�g

	static std::unique_ptr<Player> mp_player;     // �v���C���[�I�u�W�F�N�g

	//std::unique_ptr<SkyDome> mp_skydome;          // �X�J�C�h�[���I�u�W�F�N�g

	std::unique_ptr<GameTimer> mp_gameTimer;      // �������ԃI�u�W�F�N�g
	std::unique_ptr<GameScore> mp_gameScore;      // �X�R�A�I�u�W�F�N�g

	//bool m_hitPlayerToFloorFlag = false;
	//bool m_hitPlayerToRoadFlag = false;

	float m_fadeInCount;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;        // �e�N�X�`���n���h��(�w�i)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureFadeIn;            // �e�N�X�`���n���h��(�t�F�[�h�C��)

	MatrixManager*                                       mp_matrixManager;      // �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	ScenePlay(SceneManager* sceneManager);
	// �f�X�g���N�^
	~ScenePlay();

	// ������
	virtual void Initialize() override;
	// �X�V
	virtual void Update(DX::StepTimer const& timer) override;
	// �`��
	virtual void Render() override;
	// �I��
	virtual void Finalize() override;

	static Player* GetPlayer() { return mp_player.get(); }

};
