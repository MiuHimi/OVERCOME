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
public:

private:
	bool                                   m_toTitleMoveOnChecker;                 // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool                                   m_returnToPlayChecker;                  // �v���C�V�[���ɖ߂�邩�ǂ����̃`�F�b�N
	bool                                   m_resultState;                          // ���U���g�V�[���̏��

	std::unique_ptr<GameScore>             mp_gameScore;

	const int m_scoreNumSize = 80;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureScore[3];       // �e�N�X�`���n���h��(����)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;     // �e�N�X�`���n���h��(�w�i)

	MatrixManager*                                       mp_matrixManager;        // �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneResult(SceneManager* sceneManager);
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

};
