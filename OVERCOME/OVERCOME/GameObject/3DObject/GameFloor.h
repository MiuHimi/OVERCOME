//////////////////////////////////////////////////////////////
// File.    GameFloor.h
// Summary. GameFloorClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"

#include "../../ExclusiveGameObject/CollisionBox.h"

class Game;
class MatrixManager;
class GameFloor : public CollisionBox
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelLattice;   // �i�q�������f���n���h��
	//std::unique_ptr<CollisionBox> mp_floor;         // ���Փ˔���p�I�u�W�F�N�g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameFloor();
	// �f�X�g���N�^
	~GameFloor();

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