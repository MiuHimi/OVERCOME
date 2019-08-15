//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/08/13
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"
#include "../3DObject/GameCamera.h"

#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class SceneTitle : public SceneBase
{
// �����o�[�ϐ�
//public:

private:
	bool                                               m_toStageSelectMoveOnChecker;
																				  // �X�e�[�W�Z���N�g�V�[���ɐi�߂邩�ǂ����̃`�F�b�N

	float											   m_fadeAlpha;				  // �t�F�[�h�p�̃��l
	float											   m_colorAlpha;			  // ���l��ύX

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureFade;			  // �e�N�X�`���n���h��(�w�i)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureTitle;		      // �e�N�X�`���n���h��(�^�C�g��)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;				  // �X�v���C�g�o�b�`

	float											   m_titleWidth;			  // �^�C�g���̕�
	float											   m_titleHeight;			  // �^�C�g���̍���
	DirectX::SimpleMath::Vector2					   m_TitlePos;				  // �^�C�g���̕\���ʒu

	std::unique_ptr<Obj2D>							   mp_startBtn;				  // �X�^�[�g�{�^���I�u�W�F�N�g

	float											   m_fadeImageWidth;		  // �t�F�[�h�摜�̕�
	float											   m_fadeImageHeight;		  // �t�F�[�h�摜�̍���
	DirectX::SimpleMath::Vector2					   m_fadeImagePos;			  // �t�F�[�h�摜�̕\���ʒu

	std::unique_ptr<GameCamera>						   mp_camera;			      // �Q�[���J�����I�u�W�F�N�g

	std::unique_ptr<DirectX::Model>					   mp_modelHouse;			  // �ƃ��f���I�u�W�F�N�g

	MatrixManager*                                     mp_matrixManager;          // �s��Ǘ��ϐ�

	EffectManager*									   mp_effectManager;		  // �G�t�F�N�g�Ǘ��ϐ�

// �����o�[�֐�
public:
	// �R���X�g���N�^
	SceneTitle(SceneManager* sceneManager, bool isFullScreen);
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

//private:

};
