//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class Game;
class Player : public CollisionBox
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // �ʒu
	DirectX::SimpleMath::Vector3             m_vel;                   // ���x
	float                                    m_direction;             // ����(�p�x)
	DirectX::SimpleMath::Quaternion          m_rotation;              // ��]
	float                                    m_height;                // ����
	float                                    m_jumpForce;             // �W�����v��
	float                                    m_gravity;               // �d��

	bool                                     m_isJump = false;        // �W�����v�������𔻒�
	bool                                     m_isCollide = false;     // �ڐG�������𔻒�

	DirectX::SimpleMath::Matrix              m_world;                 // ���[���h���W

	std::unique_ptr<DirectX::Model>          m_modelPlayer;           // �v���C���[���f���I�u�W�F�N�g
	//std::unique_ptr<CollisionBox>          mp_floor;                // ���Փ˔���p�I�u�W�F�N�g

	Game*                                    mp_game;                 // Game�t�@�C���̏����i�[

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	Player();
	Player(Game* game);
	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render(DirectX::SimpleMath::Matrix view);
	// �p������
	void Depose();

	// �v���C���[�̈ʒu���擾
	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }
	// �v���C���[�̌������擾
	float GetDirection() { return m_direction; }
	// �v���C���[�̍������擾
	float GetHeight() { return m_height; }
	Player* GetPlayer();

	void SetJumpState(bool flag) { m_isJump = flag; }
	//void SetCollideToRoad(bool flag) { m_isJump = flag; }
	void SetHeightPos(float pos)      { m_pos.y = pos; }
	void SetHeightVel(float vel)      { m_vel.y = vel; }
	void SetCollideState(bool flag)   { m_isCollide = flag; }

private:


};