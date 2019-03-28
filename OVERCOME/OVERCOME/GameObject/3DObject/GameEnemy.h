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
	DirectX::SimpleMath::Vector3             m_vel;                   // ���x

	bool                                     m_state;                 // ����
	Collision::Sphere                        m_sphere;                // �Փ˔�����

	DirectX::SimpleMath::Matrix              m_rotaY;                 // ��]
	DirectX::SimpleMath::Matrix              m_world;                 // ���[���h���W

	std::unique_ptr<DirectX::Model>          mp_modelEnemy;          // �e���f���I�u�W�F�N�g

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

	// �ʒu�ݒ�
	void SetPos(DirectX::SimpleMath::Vector3 pos) { m_pos = pos; }
	// ���x�x�N�g���ݒ�
	void SetVel(DirectX::SimpleMath::Vector3 vel) { m_vel = vel; }

	// ���W�擾
	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }

	// ������Ԑݒ�A�擾
	bool GetState()               { return m_state; }
	void SetState(bool stateFlag) { m_state = stateFlag; }

	// �Փ˔����Ԑݒ�A�擾
	Collision::Sphere GetCollide() { return m_sphere; }
	void SetCollide(Collision::Sphere collide) { m_sphere = collide; }
	// ��]�p��ݒ�
	void SetRotate(float angle) { m_rotaY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)); }

	// ���f�����擾
	const DirectX::Model& GetModel() const
	{
		if (!mp_modelEnemy)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_modelEnemy;
	}
	// ���f�����ݒ�
	void SetModel(std::unique_ptr<DirectX::Model>&& newData)
	{
		mp_modelEnemy = std::move(newData);
	}


private:
	// �t�H�O�̐ݒ�
	void SetFogEffectDistance(float start, float end);

};