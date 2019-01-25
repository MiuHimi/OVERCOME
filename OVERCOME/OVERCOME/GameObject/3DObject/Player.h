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

#include "../ExclusiveGameObject/CollisionBox.h"

#include "GameBulletManager.h"
#include "GameCamera.h"

class GameCamera;
class GameRoad;
class MatrixManager;
class Player : public CollisionBox
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                       // �ʒu
	DirectX::SimpleMath::Vector3             m_vel;                       // ���x(�x�N�g��)
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

	bool                                     m_restartFlag;
	int                                      m_restartTime;

	bool   m_playStartFlag;
	int    m_playStartTime;

	DirectX::SimpleMath::Vector3             m_ahead;

	DirectX::SimpleMath::Matrix              m_world;                     // ���[���h���W

	std::unique_ptr<DirectX::Model>          m_modelPlayer;               // �v���C���[���f���I�u�W�F�N�g
	//std::unique_ptr<GameBulletManager>       mp_bulletManager;
	GameBulletManager*                       mp_bulletManager;
	std::unique_ptr<GameCamera>              mp_gameCamera;               // �J�����|�C���^�[
	std::unique_ptr<GameRoad>                mp_gameRoad;                 // ���H�|�C���^
	DirectX::SimpleMath::Vector2             m_passedRoadPos;             // �ʉߍς݂̓��H���L��
	DirectX::SimpleMath::Vector2             m_nextPos;                   // ���Ɍ��������H�̍��W���L��

	bool m_velFlag;

	DirectX::SimpleMath::Vector2                         m_posRestart;               // ���X�^�[�g�ʒu
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureRestart;           // �e�N�X�`���n���h��(���X�^�[�g)

	const int                                            m_countUISize = 80;         // �����̃T�C�Y
	DirectX::SimpleMath::Vector2                         m_posCount;                 // �J�E���g�ʒu
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureCount;             // �e�N�X�`���n���h��(�J�E���g)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_texturePointer;           // �e�N�X�`���n���h��(�|�C���^�[)

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
	void Render(MatrixManager* matrixManager);
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

	// �i�s�x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetAhead() { return m_ahead; }

	/*// �e���擾
	const GameBulletManager& GetBulletManager() const
	{
		if (!mp_bulletManager)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_bulletManager;
	}
	// �e���ݒ�
	void SetBulletManager(std::unique_ptr<GameBulletManager>&& newData)
	{
		mp_bulletManager = std::move(newData);
	}*/

	GameBulletManager* GetBulletManager()
	{
		return mp_bulletManager;
	}

private:


};