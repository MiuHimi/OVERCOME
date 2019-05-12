//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"

class MatrixManager;
class SceneLogo : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool                                               m_toTitleMoveOnChecker;        // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	int                                                m_fadeoutNeedTime;             // �t�F�[�h�A�E�g���n�߂�b��
	static const int                                   m_sceneChangeNeedSecond = 4;   // �V�[���J�ڂɕK�v�ȕb��

	float                                              fadeoutCount;                  // ���l��ύX

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureBackground;           // �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureLogo;                 // �e�N�X�`���n���h��

	MatrixManager*                                     mp_matrixManager;              // �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneLogo(SceneManager* sceneManager);
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

};
