//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SpriteFont.h>
#include "SceneBase.h"

class SceneStageSelect : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toPlayMoveOnChecker;                   // �v���C�V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool m_returnToTitleChecker;                  // �^�C�g���V�[���ɖ߂�邩�ǂ����̃`�F�b�N

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneStageSelect(SceneManager* sceneManager);
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

};
