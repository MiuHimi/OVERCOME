//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2018/11/30
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../ExclusiveGameObject/Collision2D.h"

class MatrixManager;
class SceneStageSelect : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toPlayMoveOnChecker;                   // �v���C�V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool m_returnToTitleChecker;                  // �^�C�g���V�[���ɖ߂�邩�ǂ����̃`�F�b�N

	int selectedStage;                            // �I�����ꂽ�X�e�[�W

	int m_selectSceneID;

	enum stage
	{
		ONE,
		TWO,

		NUM
	};

	const int                                            m_stageIconSize = 80;
	DirectX::SimpleMath::Vector2                         m_posStageIcon[NUM];      // �^�C�}�[����ʒu
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureStageIcon[NUM];  // �e�N�X�`���n���h��(����)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;      // �e�N�X�`���n���h��(����)

	Collision2D                                          m_collideStageIcon[2];

	MatrixManager*                                       mp_matrixManager;      // �s��Ǘ��ϐ�

	std::unique_ptr<DirectX::SpriteBatch>                mp_sprite;

	float                                                m_color;

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneStageSelect(SceneManager* sceneManager);
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

private:
	// �I�����Ă���X�e�[�W��ǂݍ���
	void LoadStage(int stageID);

	// �I�����Ă���X�e�[�W��\��
	void ShowStage();
};
