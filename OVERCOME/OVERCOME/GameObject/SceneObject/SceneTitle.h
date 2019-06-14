//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

class MatrixManager;
class SceneTitle : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool                                               m_toStageSelectMoveOnChecker;
																				  // �X�e�[�W�Z���N�g�V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	float											   m_colorAlpha;			  // ���l��ύX

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureBackground;      // �e�N�X�`���n���h��(�w�i)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureTitle;		      // �e�N�X�`���n���h��(�^�C�g��)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;				  // �X�v���C�g�o�b�`

	float											   m_titleWidth;			  // �^�C�g���̕�
	float											   m_titleHeight;			  // �^�C�g���̍���
	DirectX::SimpleMath::Vector2					   m_TitlePos;				  // �^�C�g���̕\���ʒu

	MatrixManager*                                     mp_matrixManager;          // �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneTitle(SceneManager* sceneManager);
	// �f�X�g���N�^
	~SceneTitle();

	// ������
	virtual void Initialize() override;
	// �X�V
	virtual void Update(DX::StepTimer const& timer) override;
	// �`��
	virtual void Render() override;
	// �I��
	virtual void Finalize() override;
};
