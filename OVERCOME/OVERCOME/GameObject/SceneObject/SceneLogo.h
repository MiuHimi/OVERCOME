//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"

class MatrixManager;
class SceneLogo : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
//public:

private:
	bool                                               m_toTitleMoveOnChecker;		// �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	static const int                                   FADEOUT_NEED_SECOND;			// �t�F�[�h�A�E�g���n�߂�b��

	std::unique_ptr<Obj2D>							   mp_background;				// �w�i�I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_logo;						// ���S�I�u�W�F�N�g
	std::unique_ptr<Obj2D>							   mp_fade;						// �t�F�[�h�摜�I�u�W�F�N�g

	MatrixManager*                                     mp_matrixManager;			// �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneLogo(SceneManager* sceneManager, bool isFullScreen);
	// �f�X�g���N�^
	~SceneLogo();

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
