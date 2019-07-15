//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../ExclusiveGameObject/CollisionBox.h"

#include "GameRoad.h"
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
	DirectX::SimpleMath::Vector3             m_dir;                       // �ړ�����(�x�N�g��)
	float                                    m_height;                    // �v���C���[���g�̍���
	float                                    m_jumpForce;                 // �W�����v��
	float                                    m_gravity;                   // �d��
	DirectX::SimpleMath::Vector3             m_posTmp;                    // �O�t���[���̈ʒu

	bool                                     m_playStartFlag;             // �Q�[�����J�n������t���O������
	int                                      m_playStartTime;             // �Q�[�����J�n�����܂ł̎���
	bool                                     m_restartFlag;               // ���X�^�[�g�����������������t���O������
	int                                      m_restartTime;               // ���X�^�[�g����܂ł̎���

	bool flag;
	int count;

	DirectX::SimpleMath::Vector2             m_passingRoadPos;            // �ʉߍς݂̓��H(ID)���L��
	DirectX::SimpleMath::Vector2             m_passedRoadPos;             // �ʉߍς݂̓��H���L��
	DirectX::SimpleMath::Vector2             m_nextPos;                   // ���Ɍ��������H�̍��W���L��
	bool                                     m_velFlag;                   // ���̓��H�}�X�Ɍ����Ĉړ����Ȃ�t���O������

	DirectX::SimpleMath::Matrix              m_world;                     // ���[���h���W

	//std::unique_ptr<DirectX::Model>          m_modelPlayer;               // �v���C���[���f���I�u�W�F�N�g
	GameBulletManager*                       mp_bulletManager;            // �e�Ǘ��|�C���^�[
	std::unique_ptr<GameCamera>              mp_gameCamera;               // �J�����|�C���^�[
	std::unique_ptr<GameRoad>                mp_gameRoad;                 // ���H�|�C���^
	
	DirectX::SimpleMath::Vector2             m_posRestartUI;              // ���X�^�[�gUI�ʒu
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     
		                                     m_textureRestart;            // �e�N�X�`���n���h��(���X�^�[�g)

	const int                                COUNTUISIZE = 80;            // �����̃T�C�Y
	DirectX::SimpleMath::Vector2             m_posCountUI;                // �J�E���g�ʒu
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		                                     m_textureCount;              // �e�N�X�`���n���h��(�J�E���g)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     
		                                     m_texturePointer;            // �e�N�X�`���n���h��(�|�C���^�[)

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
	// �p��
	void Depose();

	//-----------------------------------Getter-----------------------------------//

	// �v���C���[�̈ʒu���擾
	DirectX::SimpleMath::Vector3 GetPos()           { return m_pos; }
	// �v���C���[�̈ړ��������擾
	DirectX::SimpleMath::Vector3 GetDir()           { return m_dir; }
	// �v���C���[�̍������擾
	float GetHeight()                               { return m_height; }
	// �v���C���[�������o�������ǂ���
	bool GetPlaying()								{ return m_playStartFlag; }

	// �ʉߒ��̓��H(ID)
	DirectX::SimpleMath::Vector2 GetPassingRoad()   { return m_passingRoadPos; }
	// �v���C���[���̎擾
	Player* GetPlayer();
	// �v���C���[���̎擾
	GameBulletManager* GetBulletManager()           { return mp_bulletManager; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	// �v���C���[�̍����݂̂̈ʒu��ݒ�
	void SetHeightPos(float pos)                    { m_pos.y = pos; }
	//----------------------------------------------------------------------------//

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

private:


};