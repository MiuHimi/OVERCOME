//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

class MatrixManager;
class SceneLogo : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool                                               m_toTitleMoveOnChecker;        // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	static const int                                   FADEOUT_NEED_SECOND;           // �t�F�[�h�A�E�g���n�߂�b��
	static const int                                   SCENE_CHANGE_NEED_SECOND;      // �V�[���J�ڂɕK�v�ȕb��

	float											   m_colorAlpha;				  // ���l��ύX

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureLogo;                // �e�N�X�`���n���h��(���S)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;					  // �X�v���C�g�o�b�`

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
