//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../../ExclusiveGameObject/Collision2D.h"

class MatrixManager;
class SceneStageSelect : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
//public:

private:
	bool                                               m_toPlayMoveOnChecker;     // �^�C�g���V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	int												   m_selectedStage;			  // �I�����ꂽ�X�e�[�W

	// �X�e�[�W��
	enum stage
	{
		ONE,
		TWO,

		NUM
	};

	float											   m_colorAlpha;			  // ���l��ύX

	static const int                                   STAGE_ICON_SIZE;			  // �X�e�[�W�A�C�R���T�C�Y
	DirectX::SimpleMath::Vector2                       m_posStageIcon[NUM];       // �X�e�[�W�A�C�R���ʒu
	Collision2D                                        m_collideStageIcon[NUM];	  // �X�e�[�W�A�C�R���Փ˔���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureStageIcon[NUM];  // �e�N�X�`���n���h��(����)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureBackground;      // �e�N�X�`���n���h��(����)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;				  // �X�v���C�g�o�b�`

	MatrixManager*                                     mp_matrixManager;		  // �s��Ǘ��ϐ�

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneStageSelect(SceneManager* sceneManager, bool isFullScreen);
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

//private:

};
