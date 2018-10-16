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
#include "../StepTimer.h"

#include "../Game.h"


class Game;
class SkyDome
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelSky;   // �X�J�C�h�[�����f���n���h��
	
	Game* mp_game;                                // Game�t�@�C���̏����i�[

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SkyDome(Game* game);
	// �f�X�g���N�^
	~SkyDome();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer, Game* game);
	// �`��
	void Render(DirectX::SimpleMath::Matrix view);
	// �p������
	void Depose();

private:


};