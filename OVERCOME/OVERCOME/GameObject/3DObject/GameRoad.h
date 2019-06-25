//////////////////////////////////////////////////////////////
// File.    GameRoad.h
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameRoad
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:
	enum PosType
	{
		START,
		GOAL,
		WAY,

		NUM
	};

private:
	static const int m_maxFloorWidth = 100;                // �t���A�̍ő啝(�ݒ�[m])
	static const int m_maxFloorHeight = 100;               // �t���A�̍ő啝(�ݒ�[m])

	static const int m_maxFloorBlock = 20;                 // �u���b�N���Ƃɕ������t���A�̕�(��)
	const int m_roadBlockSize = 5;                         // �P�ʃu���b�N�̕�(�ݒ�[m])

	DirectX::SimpleMath::Vector2 m_posStartEnd[2];         // �X�^�[�g�ʒu�ƃS�[���̈ʒu���L��

	struct road
	{
		int roadType;					  // ���̎��
		int roadNum;					  // ���̐�(�J�E���g�p)
		DirectX::SimpleMath::Vector3 pos; // �ʒu   
	};

	road m_road[m_maxFloorBlock][m_maxFloorBlock];   // ���̏����i�[

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
	void Render(MatrixManager* matrixManager);
	// �p������
	void Depose();

	int GetMaxFloorBlock() { return m_maxFloorBlock; }

	DirectX::SimpleMath::Vector2 GetPosType(int posType) { return m_posStartEnd[posType]; }

	road GetRoadObject(int j, int i) { return m_road[j][i]; }

private:
	void SetFogEffectDistance(float start, float end);

};