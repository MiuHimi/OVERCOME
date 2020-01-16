//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2019/09/06
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
public:

	// �v���C���[�̏��
	enum PLAYER_STATE
	{
		MORIBUND = 3,	// �m��
		WOUND = 7,		// ����
		NORMAL = 10		// �ʏ�
	};

private:
	DirectX::SimpleMath::Vector3             m_pos;                       // �ʒu
	DirectX::SimpleMath::Vector3             m_vel;                       // ���x(�x�N�g��)
	DirectX::SimpleMath::Vector3             m_dir;                       // �ړ�����(�x�N�g��)
	float                                    m_height;                    // �v���C���[���g�̍���
	DirectX::SimpleMath::Vector3             m_posTmp;                    // �O�t���[���̈ʒu
	int										 m_hp;						  // �̗�
	DirectX::SimpleMath::Vector2			 m_hpBasePos;				  // �̗̓o�[��ʒu
	int										 m_damageCount;				  // �_���[�W���󂯂����̌v���J�E���g
	static const int                         DAMAGE_EFFECT_COUNT;		  // �_���[�W���o�̐؂�ւ��t���[����
	static const int                         FINISH_DAMAGE_EFFECT_COUNT;  // �����o���܂łɂ����鎞��(�t���[����)
	bool									 m_isDamaged;				  // �G�ɐڐG������

	bool                                     m_playStartFlag;             // �v���C���[���i�s���o������t���O������(����)
	int                                      m_moveStartCountDown;        // �����o���܂ł̎���
	static const int                         START_COUNT_TIME;			  // �����o���܂łɂ����鎞��(�t���[����)

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
	
	bool									 m_isFullScreen;			  // �t���X�N���[�����ǂ���

	static const int                         COUNT_UI_SIZE;               // �J�E���g�_�E�������̃T�C�Y
	std::unique_ptr<Obj2D>					 mp_startCount;				  // �X�^�[�g�J�E���g�_�E���I�u�W�F�N�g
	std::unique_ptr<Obj2D>					 mp_shootPointer;			  // ���˃|�C���^�[�I�u�W�F�N�g
	std::unique_ptr<Obj2D>					 mp_dengerousSign;			  // �댯�T�C���I�u�W�F�N�g
	std::unique_ptr<Obj2D>					 mp_hp;						  // HP�I�u�W�F�N�g
	std::unique_ptr<Obj2D>					 mp_hpFrame;				  // HP�̃t���[���I�u�W�F�N�g
	std::unique_ptr<Obj2D>					 mp_damageEffect;			  // �_���[�W���ʃI�u�W�F�N�g

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
	bool Update(DX::StepTimer const& timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir, DirectX::SimpleMath::Vector3 correctPos);
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
	// HP���擾
	int GetHP()										{ return m_hp; }
	// �v���C���[�������o�������ǂ���
	bool GetPlaying()								{ return m_playStartFlag; }
	// �G���o�Ă��鎞�Ԃ��ǂ���
	bool GetEnemyTime()								{return m_spawnFlag;}
	// �ʉߒ��̓��H(ID)
	DirectX::SimpleMath::Vector2 GetPassingRoad()   { return m_passingRoadPos; }
	// �v���C���[���̎擾
	GameBulletManager* GetBulletManager()           { return mp_bulletManager; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	// �v���C���[�̍����݂̂̈ʒu��ݒ�
	void SetHeightPos(float pos)                    { m_pos.y = pos; }
	// �G�ɐڐG�������ǂ�����ݒ�
	void SetDamaged(bool flag)						{ m_isDamaged = flag; }
	//----------------------------------------------------------------------------//

	// �̗͐ݒ�
	void Damage(int damage) { m_hp -= damage; }

private:

	// �i�݂����O�̏���
	bool BeforeGoOnPlayer(const bool isClickCenter);

	// �i�ݎn�߂���̏���
	void AfterGoOnPlayer(const bool isGoOn, DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& cameraDir);

	// ���̐i�H��𒲂ׂ�
	void SearchNextRoad(const DirectX::SimpleMath::Vector2 nowPos);

	// �_���[�W�̍X�V
	void UpdateDamage();

	// HP�̍X�V
	void UpdateHP();

};