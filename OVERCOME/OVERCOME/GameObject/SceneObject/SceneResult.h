//////////////////////////////////////////////////////////////
// File.    SceneResult.h
// Summary. SceneResultClass
// Date.    2019/09/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"

#include "../2DObject/Obj2D.h"

class GameScore;
class MatrixManager;
class SceneResult : public SceneBase
{
// �����o�[�ϐ�
//public:

private:
	bool								m_toTitleMoveOnChecker;		// �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool								m_returnToPlayChecker;		// �v���C�V�[���ɖ߂�邩�ǂ����̃`�F�b�N
	bool								m_resultState;				// ���U���g�V�[���̏��

	static const int					SCORE_SIZE;					// �X�R�A�̐����̃T�C�Y

	// �X�R�A�̌�
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	DirectX::SimpleMath::Vector2		m_scoreBasePos;				// �X�R�A�̕\����ʒu

	std::unique_ptr<GameScore>			mp_gameScore;				// �X�R�A�̃|�C���^

	std::unique_ptr<Obj2D>				mp_fade;					// �t�F�[�h�摜�I�u�W�F�N�g
	std::unique_ptr<Obj2D>				mp_score;					// �X�R�A�I�u�W�F�N�g
	std::unique_ptr<Obj2D>				mp_bg;						// �w�i�I�u�W�F�N�g
	std::unique_ptr<Obj2D>				mp_resultStr;				// RESULT������I�u�W�F�N�g

	std::unique_ptr<Obj2D>				mp_gameover;				// �Q�[���I�[�o�[�摜�I�u�W�F�N�g
	std::unique_ptr<Obj2D>				mp_gameoverBG;				// �Q�[���I�[�o�[�w�i�I�u�W�F�N�g

	MatrixManager*						mp_matrixManager;			// �s��Ǘ��ϐ�

// �����o�[�֐�
public:
	// �R���X�g���N�^
	SceneResult(SceneManager* sceneManager, bool isFullScreen);
	// �f�X�g���N�^
	~SceneResult();

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
