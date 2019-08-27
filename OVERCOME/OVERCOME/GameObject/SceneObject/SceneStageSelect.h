//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2019/08/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"

class MatrixManager;
class SceneStageSelect : public SceneBase
{
// �����o�[�ϐ�
//public:

private:
	bool                                               m_toPlayMoveOnChecker;     // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	int												   m_selectedStage;			  // �I�����ꂽ�X�e�[�W

	// �X�e�[�W��
	enum STAGE
	{
		ONE,
		TWO,

		NUM
	};

	static const int                                   STAGE_ICON_SIZE;			  // �X�e�[�W�A�C�R���T�C�Y

	std::unique_ptr<Obj2D>							   mp_background;			  // �w�i�I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_stageSelectImage;		  // StageSelect�I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_stageNum[NUM];		  // �X�e�[�W�ԍ��I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_stageFlame[NUM];		  // �X�e�[�W�ԍ��t���[���I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_fade;					  // �t�F�[�h�摜�I�u�W�F�N�g

	MatrixManager*                                     mp_matrixManager;		  // �s��Ǘ��ϐ�

// �����o�[�֐�
public:
	// �R���X�g���N�^
	SceneStageSelect(SceneManager* sceneManager, bool isFullScreen);
	// �f�X�g���N�^
	~SceneStageSelect();

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
