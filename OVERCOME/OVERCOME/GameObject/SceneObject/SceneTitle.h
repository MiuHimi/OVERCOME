//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"
#include "../3DObject/GameCamera.h"

#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class SceneTitle : public SceneBase
{
// �����o�[�ϐ�
//public:

private:
	enum GHOST_DIR
	{
		NONE,
		RIGHT_DIR = 90,
		LEFT_DIR = -90
	};

	bool                                               m_toStageSelectMoveOnChecker;
																				  // �X�e�[�W�Z���N�g�V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	float											   m_colorAlpha;			  // ���l��ύX

	static const int								   MAX_TITLE_STR = 8;		  // �^�C�g��������
	std::unique_ptr<Obj2D>							   mp_title[MAX_TITLE_STR];	  // �^�C�g���I�u�W�F�N�g
	static const int								   TITLE_STR_WIDTH;			  // �^�C�g�������̕�
	std::unique_ptr<Obj2D>							   mp_startBtn;				  // �X�^�[�g�{�^���I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_fade;					  // �t�F�[�h�摜�I�u�W�F�N�g

	std::unique_ptr<GameCamera>						   mp_camera;			      // �Q�[���J�����I�u�W�F�N�g

	static const int								   MAX_GHOST_POS_X;			  // �H�삪������X���̍ő�l
	static const float								   MAX_GHOST_POS_Y;			  // �H�삪������Y���̍ő�l
	static const float								   MIN_GHOST_POS_Y;			  // �H�삪������Y���̍ŏ��l
	static const int								   RESPAWN_COUNT_MIN;		  // �H��̃��X�|�[���J�E���g�ŏ��l
	static const int								   RESPAWN_COUNT_MAX;		  // �H��̃��X�|�[���J�E���g�ő�l
	GHOST_DIR										   m_ghostDir;				  // �H��̌���
	DirectX::SimpleMath::Vector3					   m_ghostPos;				  // �H��̈ʒu
	int												   m_spawnCount;			  // ���X�|�[�����邽�߂̃J�E���g
	int												   m_nextSpawnCount;		  // �X�|�[������܂ł̃J�E���g

	std::unique_ptr<DirectX::Model>					   mp_modelHouse;			  // �ƃ��f���I�u�W�F�N�g
	std::unique_ptr<DirectX::Model>					   mp_modelEnemy;			  // �G���f���I�u�W�F�N�g

	MatrixManager*                                     mp_matrixManager;          // �s��Ǘ��ϐ�

	EffectManager*									   mp_effectManager;		  // �G�t�F�N�g�Ǘ��ϐ�

	

// �����o�[�֐�
public:
	// �R���X�g���N�^
	SceneTitle(SceneManager* sceneManager, bool isFullScreen);
	// �f�X�g���N�^
	~SceneTitle();

	// ������
	virtual void Initialize() override;
	// �X�V
	virtual void Update(DX::StepTimer const& timer) override;
	// �`��
	virtual void Render() override;
	// �I��
	virtual void Finalize() override;

//private:

};
