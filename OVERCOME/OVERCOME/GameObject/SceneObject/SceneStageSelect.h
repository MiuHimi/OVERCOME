//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2018/11/30
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <vector>
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../GameCamera.h"

class SceneStageSelect : public SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	bool m_toPlayMoveOnChecker;                   // �v���C�V�[���ɐi�߂邩�ǂ����̃`�F�b�N
	bool m_returnToTitleChecker;                  // �^�C�g���V�[���ɖ߂�邩�ǂ����̃`�F�b�N

	std::unique_ptr<DirectX::Model> m_modelRoadStraight;   // �����^���H
	std::unique_ptr<DirectX::Model> m_modelRoadStop;       // ���[�^���H
	std::unique_ptr<DirectX::Model> m_modelRoadCurve;      // �Ȑ��^���H
	std::unique_ptr<DirectX::Model> m_modelRoadBranch;     // ����^���H

	static const int m_maxFloorWidth = 100;                // �t���A�̍ő啝(�ݒ�[m])
	static const int m_maxFloorHeight = 100;               // �t���A�̍ő啝(�ݒ�[m])

	static const int m_maxFloorBlock = 20;                 // �u���b�N���Ƃɕ������t���A�̕�(��)
	const int m_roadBlockSize = 5;                         // �P�ʃu���b�N�̕�(�ݒ�[m])

	struct road
	{
		int roadType;                                      // ���H�̎��
		DirectX::SimpleMath::Vector3 pos;                  // ���H�̈ʒu
		int rotaAngle;                                     // ��]�p��ݒ�( (0 or 1 or 2 or 3) * 90.0f )
	};

	//road m_roadObject[m_maxFloorBlock][m_maxFloorBlock];   // ���H�̏����i�[
	std::vector<std::vector<road>> m_roadObject;

	std::unique_ptr<GameCamera> mp_camera;        // �J�����I�u�W�F�N�g

	bool m_showFlag;                                        // 
	int m_selectSceneID;

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
