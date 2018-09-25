//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../../Game.h"

class SceneTitle : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toPlayMoveOnChecker;   // ���S�V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	Game* mp_game;                                // Game�t�@�C���̏����i�[
	std::unique_ptr<DirectX::SpriteFont> m_font;  // �X�v���C�g�t�H���g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneTitle(SceneManager* sceneManager);
	SceneTitle(Game* game, SceneManager* sceneManager);
	// �f�X�g���N�^
	~SceneTitle();

	// ������
	virtual void Initialize() override;
	// �X�V
	//virtual void Update(DX::StepTimer const& timer) override;
	virtual void Update(DX::StepTimer const& timer, Game* game) override;
	// �`��
	//virtual void Render() override;
	virtual void Render(DirectX::SpriteBatch* sprites, Game* game) override;
	// �`��
	virtual void Finalize() override;
};
