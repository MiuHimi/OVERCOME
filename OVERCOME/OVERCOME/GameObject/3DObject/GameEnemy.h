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

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // �ʒu
	DirectX::SimpleMath::Vector3             m_posTmp;                // �O�t���[���̈ʒu
	DirectX::SimpleMath::Vector3             m_vel;                   // ���x
	DirectX::SimpleMath::Vector3             m_dir;                   // ����

	bool                                     m_state;                 // ����
	bool									 m_shock;                 // ���@�̒e�����������^�C�~���O�̂݃t���O������
	Collision::Sphere                        m_sphere;                // �Փ˔�����

	DirectX::SimpleMath::Quaternion          m_rotaX;                 // X����]
	DirectX::SimpleMath::Quaternion          m_rotaY;                 // Y����]
	DirectX::SimpleMath::Matrix              m_world;                 // ���[���h���W

	std::unique_ptr<DirectX::Model>          mp_modelEnemy;           // �e���f���I�u�W�F�N�g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameEnemy(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model);
	// �f�X�g���N�^
	~GameEnemy();

	// ����
	void Create();

	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render(MatrixManager* matrixManager);


	//-----------------------------------Getter-----------------------------------//

	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }
	DirectX::SimpleMath::Vector3 GetVel() { return m_vel; }
	DirectX::SimpleMath::Vector3 GetDir() { return m_dir; }
	DirectX::SimpleMath::Matrix GetMatrix() { return m_world; }

	bool GetState()						  { return m_state; }
	bool GetShock()						  { return m_shock; }
	Collision::Sphere GetCollide()		  { return m_sphere; }

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
	void SetCollide(Collision::Sphere collide)				 { m_sphere = collide; }

	void SetRotateX(float angle)							 { m_rotaX = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), angle); }
	void SetRotateY(float angle)							 { m_rotaY = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), angle); }

	void SetModel(std::unique_ptr<DirectX::Model>&& newData) { mp_modelEnemy = std::move(newData); }
	//----------------------------------------------------------------------------//

private:
	// �t�H�O�̐ݒ�
	void SetFogEffectDistance(float start, float end);

};