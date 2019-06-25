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

#include "../../ExclusiveGameObject/CollisionMesh.h"

class MatrixManager;
class Player;
class GameMap
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> mp_modelMap;	   // �}�b�v���f��

	std::unique_ptr<CollisionMesh>  mp_collisionStage; // �R���W�������b�V��

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
	bool Update(DX::StepTimer const& timer, Player* player);
	// �`��
	void Render(MatrixManager* matrixManager);
	// �p������
	void Depose();

private:
	void SetFogEffectDistance(float start, float end);

};