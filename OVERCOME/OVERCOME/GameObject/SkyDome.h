//////////////////////////////////////////////////////////////
// File.    SkyDome.h
// Summary. SkyDomeClass
// Date.    2018/09/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../Game.h"


class Game;
class MatrixManager;
class SkyDome
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelSky;   // �X�J�C�h�[�����f���n���h��
	
// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SkyDome();
	// �f�X�g���N�^
	~SkyDome();

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


};