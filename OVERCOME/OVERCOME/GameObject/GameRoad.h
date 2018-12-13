//////////////////////////////////////////////////////////////
// File.    GameRoad.h
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class GameRoad : public CollisionBox 
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:
	enum PosType
	{
		START,
		GOAL,

		NUM
	};

private:
	std::unique_ptr<DirectX::Model> m_modelRoadStraight;   // �����^���H
	std::unique_ptr<DirectX::Model> m_modelRoadStop;       // ���[�^���H
	std::unique_ptr<DirectX::Model> m_modelRoadCurve;      // �Ȑ��^���H
	std::unique_ptr<DirectX::Model> m_modelRoadBranch;     // ����^���H
	//std::unique_ptr<CollisionBox> mp_floor;              // ���H�Փ˔���p�I�u�W�F�N�g

	static const int m_maxFloorWidth = 100;                // �t���A�̍ő啝(�ݒ�[m])
	static const int m_maxFloorHeight = 100;               // �t���A�̍ő啝(�ݒ�[m])

	static const int m_maxFloorBlock = 20;                 // �u���b�N���Ƃɕ������t���A�̕�(��)
	const int m_roadBlockSize = 5;                         // �P�ʃu���b�N�̕�(�ݒ�[m])

	
	DirectX::SimpleMath::Vector2 m_posStartEnd[2];         // �X�^�[�g�ʒu�ƃS�[���̈ʒu���L��

	struct road
	{
		int roadType;                                      // ���H�̎��
		DirectX::SimpleMath::Vector3 pos;                  // ���H�̈ʒu
		int rotaAngle;                                     // ��]�p��ݒ�( (0 or 1 or 2 or 3) * 90.0f )
	};

	road m_roadObject[m_maxFloorBlock][m_maxFloorBlock];   // ���H�̏����i�[
	CollisionBox* mp_roadCollideObject[m_maxFloorBlock][m_maxFloorBlock];

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameRoad();
	// �f�X�g���N�^
	~GameRoad();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render();
	// �p������
	void Depose();

	int GetMaxFloorBlock() { return m_maxFloorBlock; }

	DirectX::SimpleMath::Vector2 GetPosType(int posType) { return m_posStartEnd[posType]; }

	road GetRoadObject(int j, int i) { return m_roadObject[j][i]; }
	CollisionBox* GetCollisionObject(int j, int i) { return mp_roadCollideObject[j][i]; }


private:
	void SetFogEffectDistance(float start, float end);

};