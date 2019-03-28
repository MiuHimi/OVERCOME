//////////////////////////////////////////////////////////////
// File.    GameMap.h
// Summary. GameMapClass
// Date.    2019/03/25
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameMap
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelMap;   // �}�b�v���f��

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameMap();
	// �f�X�g���N�^
	~GameMap();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render(MatrixManager* matrixManager);
	// �p������
	void Depose();

private:
	void SetFogEffectDistance(float start, float end);

};