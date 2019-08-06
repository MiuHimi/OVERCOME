//////////////////////////////////////////////////////////////
// File.    GameEnemy.h
// Summary. GameEnemyClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class MatrixManager;
class GameEnemy : public CollisionSphere
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:
	// �G�̎��
	enum EnemyType
	{
		NORMAL,
		POWER,
		SPEED,

		NUM
	};

	static const int						 NORMAL_ENEMY_POINT = 20; // �ʏ�G�̓��_
	static const int						 POWER_ENEMY_POINT = 30;  // �p���[�G�̓��_
	static const int						 SPEED_ENEMY_POINT = 10;  // �X�s�[�h�G�̓��_

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // �ʒu
	DirectX::SimpleMath::Vector3             m_posTmp;                // �O�t���[���̈ʒu
	DirectX::SimpleMath::Vector3             m_vel;                   // ���x
	DirectX::SimpleMath::Vector3             m_dir;                   // ����
	float									 m_size;				  // �T�C�Y
	int										 m_prob[EnemyType::NUM];  // �o���m��
	EnemyType								 m_type;				  // ���
	int										 m_hp;					  // �̗�(HP)
	static const int						 NORMAL_ENEMY_PROBABILITY;// �ʏ�G�̐����m��
	static const int						 POWER_ENEMY_PROBABILITY; // �p���[�G�̐����m��
	static const int						 SPEED_ENEMY_PROBABILITY; // �X�s�[�h�G�̐����m��

	bool                                     m_state;                 // ����
	bool									 m_shock;                 // ���@�̒e�����������^�C�~���O�̂݃t���O������
	Collision::Sphere                        m_sphere;                // �Փ˔�����

	DirectX::SimpleMath::Quaternion          m_rotaX;                 // X����]
	DirectX::SimpleMath::Quaternion          m_rotaY;                 // Y����]
	DirectX::SimpleMath::Matrix              m_world;                 // ���[���h���W

	std::unique_ptr<DirectX::Model>          mp_modelEnemy;           // �G���f���I�u�W�F�N�g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameEnemy(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model);
	// �f�X�g���N�^
	~GameEnemy();

	// ����
	void Create(int probability);

	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render(MatrixManager* matrixManager);


	//-----------------------------------Getter-----------------------------------//

	DirectX::SimpleMath::Vector3 GetPos()	{ return m_pos; }
	DirectX::SimpleMath::Vector3 GetVel()	{ return m_vel; }
	DirectX::SimpleMath::Vector3 GetDir()	{ return m_dir; }
	float GetSize()							{ return m_size; }
	DirectX::SimpleMath::Matrix GetMatrix() { return m_world; }

	bool GetState()						    { return m_state; }
	bool GetShock()						    { return m_shock; }
	EnemyType GetType()					    { return m_type; }
	int GetHP()								{ return m_hp; }
	Collision::Sphere GetCollide()		    { return m_sphere; }

	const DirectX::Model& GetModel() const
	{
		if (!mp_modelEnemy)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_modelEnemy;
	}
	//----------------------------------------------------------------------------//


	//-----------------------------------Setter-----------------------------------//

	void SetPos(DirectX::SimpleMath::Vector3 pos)			 { m_pos = pos; }
	void SetVel(DirectX::SimpleMath::Vector3 vel)			 { m_vel = vel; }
	void SetDir(DirectX::SimpleMath::Vector3 dir)			 { m_dir = dir; }

	void SetState(bool stateFlag)							 { m_state = stateFlag; }
	void SetShock(bool shockFlag)							 { m_shock = shockFlag; }
	void SetHP(int hp)										 { m_hp = hp; }
	void SetCollide(Collision::Sphere collide)				 { m_sphere = collide; }

	void SetRotateX(float angle)							 { m_rotaX = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), angle); }
	void SetRotateY(float angle)							 { m_rotaY = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), angle); }

	void SetModel(std::unique_ptr<DirectX::Model>&& newData) { mp_modelEnemy = std::move(newData); }
	//----------------------------------------------------------------------------//

private:
	// �t�H�O�̐ݒ�
	void SetFogEffectDistance(float start, float end);

};