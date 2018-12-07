//////////////////////////////////////////////////////////////
// File.    GameTarget.h
// Summary. GameTargetClass
// Date.    2018/11/19
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class Game;
class GameTarget : public CollisionBox
{
	// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelTarget;   // �����^���H

	static const int m_maxFloorWidth = 100;                // �t���A�̍ő啝(�ݒ�[m])
	static const int m_maxFloorHeight = 100;               // �t���A�̍ő啝(�ݒ�[m])

	static const int m_maxFloorBlock = 20;                 // �u���b�N���Ƃɕ������t���A�̕�(��)
	const int m_roadBlockSize = 5;                         // �P�ʃu���b�N�̕�(�ݒ�[m])

	int m_stageCount;

	const float m_targetRadius = 1.0f;

	struct target
	{
		DirectX::SimpleMath::Vector3       pos;           // ���H�̈ʒu
		int                                height;        // �I�̍���
		bool                               state;         // �����t���O
		int                                rotaAngle;     // ��]�p��ݒ�( (0 or 1 or 2 or 3) * 90.0f )
	};

	target m_targetObject[m_maxFloorBlock][m_maxFloorBlock];   // ���H�̏����i�[
	CollisionBox* mp_targetCollideObject[m_maxFloorBlock][m_maxFloorBlock];

	// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameTarget();
	// �f�X�g���N�^
	~GameTarget();

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

	bool GetState(int j, int i) { return m_targetObject[j][i].state; }
	void SetState(int j, int i, bool state) { m_targetObject[j][i].state = state; }
	target GetTargetObject(int j, int i) { return m_targetObject[j][i]; }
	CollisionBox* GetCollisionObject(int j, int i) { return mp_targetCollideObject[j][i]; }


private:


};