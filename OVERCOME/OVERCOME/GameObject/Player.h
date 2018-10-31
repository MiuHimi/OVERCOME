//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2018/07/27
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
class Player : public CollisionBox
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                       // �ʒu
	DirectX::SimpleMath::Vector3             m_vel;                       // ���x(�x�N�g��)
	float                                    m_accel;                     // �����x
	float                                    m_direction;                 // ����(�p�x)
	DirectX::SimpleMath::Quaternion          m_rotation;                  // ��]
	float                                    m_height;                    // �v���C���[���g�̍���
	float                                    m_jumpForce;                 // �W�����v��
	float                                    m_gravity;                   // �d��
	float                                    m_fallingPower;              // ���̂܂ܗ�����Ƃ��̗�

	bool                                     m_isJump           = false;  // �W�����v�������𔻒�
	bool                                     m_collideToFloor   = false;  // ���ƐڐG�������𔻒�
	bool                                     m_collideToRoad    = false;  // ���H�ƐڐG�������𔻒�
	bool                                     m_noTouchObectFlag = false;  // ���ɂ��G�ꂸ�W�����v�����Ă��Ȃ����Ƀt���O������

	DirectX::SimpleMath::Matrix              m_world;                     // ���[���h���W

	std::unique_ptr<DirectX::Model>          m_modelPlayer;               // �v���C���[���f���I�u�W�F�N�g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player();

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

	// �v���C���[�̈ʒu���擾
	DirectX::SimpleMath::Vector3 GetPos()           { return m_pos; }
	// �v���C���[�̌������擾
	float GetDirection()                            { return m_direction; }
	// �v���C���[�̍������擾
	float GetHeight()                               { return m_height; }
	// �v���C���[���W�����v���ł��邩���擾
	bool GetJumpState()                             { return m_isJump; }
	// �v���C���[���̎擾
	Player* GetPlayer();

	// �v���C���[�̍����݂̂̈ʒu��ݒ�
	void SetHeightPos(float pos)                    { m_pos.y = pos; }
	// ���H���炻�̂܂ܗ����Ă������x��ݒ�
	void SetFallingPower(float power)               { m_fallingPower = power; }

	// �W�����v���[�V�������I��������(���߂̃t���O�ݒ�)
	void SetJumpState(bool flag)                    { m_isJump = flag; }
	// ���A���H�Ƃ̏Փ˔���̃t���O�ݒ�
	void SetFloorCollideState(bool flag)            { m_collideToFloor = flag; }
	void SetRoadCollideState(bool flag)             { m_collideToRoad = flag; }
	// ���ɂ��G�ꂸ�W�����v�����Ă��Ȃ����Ƀt���O������
	void SetNotTouchState(bool flag)                { m_noTouchObectFlag = flag; }

private:


};