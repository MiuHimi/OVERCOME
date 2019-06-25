//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2019/06/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../../Game.h"

#include "../3DObject/GameCamera.h"

#include "../3DObject/Player.h"

#include "../3DObject/GameRoad.h"
#include "../3DObject/GameMap.h"

#include "../2DObject/GameScore.h"

//#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class ScenePlay : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool								m_toResultMoveOnChecker;   // ���U���g�V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool								m_returnToTitleChecker;    // �^�C�g���V�[���ɖ߂�邩�ǂ����̃`�F�b�N

	float								m_colorAlpha;			   // ���l��ύX

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
										mp_fade;				   // �e�N�X�`���n���h��(�t�F�[�h)
	std::unique_ptr<DirectX::SpriteBatch>
										mp_sprite;				   // �X�v���C�g�o�b�`

	std::unique_ptr<GameCamera>			mp_camera;				   // �Q�[���J�����I�u�W�F�N�g

	std::unique_ptr<Player>				mp_player;				   // �v���C���[�I�u�W�F�N�g

	std::unique_ptr<GameRoad>			mp_gameRoad;			   // ���I�u�W�F�N�g
	std::unique_ptr<GameMap>			mp_gameMap;				   // �X�e�[�W�I�u�W�F�N�g

	std::unique_ptr<GameScore>			mp_gameScore;			   // �X�R�A�I�u�W�F�N�g

	MatrixManager*						mp_matrixManager;		   // �s��Ǘ��ϐ�

	//EffectManager*					mp_effectManager;		   // �G�t�F�N�g�Ǘ��ϐ�

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

};
