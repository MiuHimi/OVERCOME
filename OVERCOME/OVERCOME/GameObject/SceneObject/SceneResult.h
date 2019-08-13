//////////////////////////////////////////////////////////////
// File.    SceneResult.h
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"

class GameScore;
class MatrixManager;
class SceneResult : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
//public:

private:
	bool												 m_toTitleMoveOnChecker;  // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool												 m_returnToPlayChecker;   // �v���C�V�[���ɖ߂�邩�ǂ����̃`�F�b�N
	bool												 m_resultState;			  // ���U���g�V�[���̏��

	static const int									 SCORE_SIZE;			  // �X�R�A�̐����̃T�C�Y

	// �X�R�A�̌�
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	DirectX::SimpleMath::Vector2						 m_scorePos;			  // �X�R�A�̕\���ʒu

	std::unique_ptr<GameScore>							 mp_gameScore;			  // �X�R�A�̃|�C���^

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     mp_textureScore;         // �e�N�X�`���n���h��(�X�R�A)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     mp_textureBackground;    // �e�N�X�`���n���h��(�w�i)
	std::unique_ptr<DirectX::SpriteBatch>                mp_sprite;				  // �X�v���C�g�o�b�`

	MatrixManager*                                       mp_matrixManager;        // �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
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
