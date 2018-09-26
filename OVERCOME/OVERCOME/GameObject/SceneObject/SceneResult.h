//////////////////////////////////////////////////////////////
// File.    SceneResult.h
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../../Game.h"

class SceneResult : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toTitleMoveOnChecker;                 // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool m_returnToPlayChecker;                  // �v���C�V�[���ɖ߂�邩�ǂ����̃`�F�b�N

	Game* mp_game;                                // Game�t�@�C���̏����i�[
	std::unique_ptr<DirectX::SpriteFont> m_font;  // �X�v���C�g�t�H���g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneResult(SceneManager* sceneManager);
	SceneResult(Game* game, SceneManager* sceneManager);
	// �f�X�g���N�^
	~SceneResult();

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

};
