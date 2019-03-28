//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

//#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class SceneTitle : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool              m_toPlayMoveOnChecker;              // ���S�V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureBackground;   // �e�N�X�`���n���h��(�w�i)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureTitle;        // �e�N�X�`���n���h��(�^�C�g��)

	//EffectManager*	                                   mp_effectManager;      // �G�t�F�N�g�Ǘ��ϐ�
	MatrixManager*                                     mp_matrixManager;      // �s��Ǘ��ϐ�

	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;

	float                                              m_color;

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
	// �`��
	virtual void Finalize() override;
};
