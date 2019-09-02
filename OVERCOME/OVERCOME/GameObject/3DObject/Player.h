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
#include "GameEnemyManager.h"

#include "../2DObject/Obj2D.h"

class GameEnemyManager;
class GameRoad;
class MatrixManager;
class Player : public CollisionBox
{
// �����o�[�ϐ�
//public:

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

	bool                                     m_spawnFlag;                 // �G���o�Ă���t���O
	float                                    m_spawnElapsedTime;          // �G���o�����Ă���̌o�ߎ���
	static const int                         SPAWNTIME;                   // �G���o�Ă��鎞��(�t���[����)

	DirectX::SimpleMath::Vector2             m_passingRoadPos;            // �ʉߍς݂̓��H(ID)���L��
	DirectX::SimpleMath::Vector2             m_passedRoadPos;             // �ʉߍς݂̓��H���L��
	DirectX::SimpleMath::Vector2             m_nextPos;                   // ���Ɍ��������H�̍��W���L��
	bool                                     m_velFlag;                   // ���̓��H�}�X�Ɍ����Ĉړ����Ȃ�t���O������

	DirectX::SimpleMath::Matrix              m_world;                     // ���[���h���W

	//std::unique_ptr<DirectX::Model>          m_modelPlayer;               // �v���C���[���f���I�u�W�F�N�g
	GameBulletManager*                       mp_bulletManager;            // �e�Ǘ��|�C���^�[
	std::unique_ptr<GameRoad>                mp_gameRoad;                 // ���H�|�C���^
	
	std::unique_ptr<Obj2D>					 mp_startGuide;				  // �X�^�[�g�ē��I�u�W�F�N�g

	const int                                COUNTUISIZE = 80;            // �����̃T�C�Y
	DirectX::SimpleMath::Vector2             m_posCountUI;                // �J�E���g�ʒu
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		                                     m_textureCount;              // �e�N�X�`���n���h��(�J�E���g)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     
		                                     m_texturePointer;            // �e�N�X�`���n���h��(�|�C���^�[)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
											 m_textureDengerous;          // �e�N�X�`���n���h��(�댯�T�C��)

// �����o�[�֐�
public:
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize();
	// ����
	void Create(const bool isFulleScreen);
	// �X�V
	bool Update(DX::StepTimer const& timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir);
	// �`��
	void Render(MatrixManager* matrixManager, GameEnemyManager::DANGERDIRECTION dangerDir);
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
	// �G���o�Ă��鎞�Ԃ��ǂ���
	bool GetEnemyTime()								{return m_spawnFlag;}
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

//private:

};