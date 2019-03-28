//////////////////////////////////////////////////////////////
// File.    GameBullet.h
// Summary. GameBulletClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class MatrixManager;
class GameBullet : public CollisionSphere
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // �ʒu
	DirectX::SimpleMath::Vector3             m_vel;                   // ���x

	bool                                     m_state;                 // ����
	Collision::Sphere                        m_sphere;                  // �Փ˔�����

	DirectX::SimpleMath::Matrix              m_world;                 // ���[���h���W

	std::unique_ptr<DirectX::Model>          mp_modelBullet;          // �e���f���I�u�W�F�N�g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameBullet(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag,DirectX::Model* model);
	// �f�X�g���N�^
	~GameBullet();

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

	DirectX::SimpleMath::Vector3 GetVel() { return m_vel; }

	// ���W�ݒ�A�擾
	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }
	//void SetPos(DirectX::SimpleMath::Vector3 pos) { m_pos = pos; }

	// ������Ԑݒ�A�擾
	bool GetState() { return m_state; }
	void SetState(bool stateFlag) { m_state = stateFlag; }

	// �Փ˔����Ԑݒ�A�擾
	Collision::Sphere GetCollide() { return m_sphere; }
	void SetCollide(Collision::Sphere collide) { m_sphere = collide; }

	// ���f�����ݒ�A�擾
	/*DirectX::Model* GetModel() { return mp_modelBullet; }
	void SetModel(DirectX::Model* model) { mp_modelBullet = model; }*/

	// ���f�����擾
	const DirectX::Model& GetModel() const
	{
		if (!mp_modelBullet)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_modelBullet;
	}
	// ���f�����ݒ�
	void SetModel(std::unique_ptr<DirectX::Model>&& newData)
	{
		mp_modelBullet = std::move(newData);
	}


private:
	// �t�H�O�̐ݒ�
	void SetFogEffectDistance(float start, float end);

};