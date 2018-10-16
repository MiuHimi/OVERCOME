//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../../Game.h"

class SceneLogo : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toTitleMoveOnChecker;   // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	int m_count;                 // �^�C���J�E���g�ϐ�

	Game* mp_game;                                // Game�t�@�C���̏����i�[

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneLogo(SceneManager* sceneManager);
	SceneLogo(Game* game, SceneManager* sceneManager);
	// �f�X�g���N�^
	~SceneLogo();

	// ������
	virtual void Initialize() override;
	// �X�V
	//virtual void Update(DX::StepTimer const& timer) override;
	virtual void Update(DX::StepTimer const& timer, Game* game) override;
	// �`��
	//virtual void Render() override;
	virtual void Render(Game* game) override;
	// �I��
	virtual void Finalize() override;

};
