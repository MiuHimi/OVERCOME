//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.h
// Summary. GameEnemyManagerClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <vector>
#include <array>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "GameEnemy.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class GameEnemy;
class MatrixManager;
class GameEnemyManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:
	// �댯����
	enum ENEMYTYPE
	{
		SPEED_TYPE,
		NORMAL_TYPE,
		POWER_TYPE,

		MAX_TYPE
	};
	enum DANGERDIRECTION
	{
		DIR_NONE,

		DIR_FRONT,
		DIR_BACK,
		DIR_RIGHT,
		DIR_LEFT,
	};
	enum ENTRYCOUNT
	{
		NONE,

		ENTRY_X,
		DISTRIBUTE_X,
		END_X,
		ENTRY_Y,
		DISTRIBUTE_Y,
		END_Y,
		ENTRY_Z,
		DISTRIBUTE_Z,
		END_Z,
	};
	enum ENEMYHP
	{
		HP_MAX,
		HP_NORMAL_DAMAGE,
		HP_POWER_DAMAGE,
		HP_POWER_CRITICAL,
		HP_NUM,
	};
	enum ENEMYPOWER
	{
		NONE_DAMAGE,
		SPEED_ENEMY_DAMAGE,
		NORMAL_ENEMY_DAMAGE,
		POWER_ENEMY_DAMAGE,
		MAX_DAMAGE,
	};
	enum ENEMYHITEFFECT
	{
		EFFECT_FIRST,
		EFFECT_SECOND,

		MAX_EFFECT
	};
	enum MAXCREATECOUNT
	{
		DONTCREATE,
		FOUR = 4,
		SIXTEEN = 16
	};

private:
	static const int			  MAX_ENEMY = 100;			  // �ő�G��
	static const int			  MAX_ENTRY_POS = 10;		  // �ő�G�o���ꏊ��

	static const int			  MAX_SPAWN_TIME;			  // �G���o���ł���ő厞��(�t���[����)
	int							  m_spawnElapsedTime;		  // �G���o�����Ă���̌o�ߎ���(�t���[����)

	static const int			  CREATE_PROBABILITY;		  // �����m��(rand()�Ƌ��Ɏg�p)
	
	bool						  m_createFillFlag;			  // �G�̐����ŏo�����x���������ꂽ��true��
	bool						  m_openChestCreateFlag[3];	  // �`�F�X�g����o��(����)������true��
	int							  m_chestCreateCount[3];	  // ���������`�F�X�g����Ăѐ��������邽�߂̃J�E���g
	int							  m_createCount;			  // �G�̐��������J�E���g
	DirectX::SimpleMath::Vector3  m_entryEnemyPosTmp;		  // �G�̏o�����ʒu

	static const int			  RESPAWN_NEED_TIME;		  // ���X�|�[���ɕK�v�Ȏ���(�t���[����)
	int                           m_respawnTime;              // ���X�|�[������(�t���[����)

	DirectX::SimpleMath::Vector3  m_entryEnemyPos[MAX_ENTRY_POS];
															  // �G�o���ʒu
	DirectX::SimpleMath::Vector3  m_entryEnemyDistribute[MAX_ENTRY_POS];
															  // �G�o���ʒu���z

	static const float			  CONTROL_NORMAL_VELOCITY;	  // ���x����(�ʏ�G�p)
	static const float			  CONTROL_POWER_VELOCITY;	  // ���x����(�p���[�G�p)
	static const float			  CONTROL_SPEED_VELOCITY;	  // ���x����(�X�s�[�h�G�p)

	GameEnemy*					  mp_enemy[MAX_ENEMY];		  // �G�I�u�W�F�N�g
	DirectX::SimpleMath::Vector3  m_hpPos[MAX_ENEMY];		  // HP��\������ʒu
	DirectX::SimpleMath::Vector3  m_shockPos[MAX_ENEMY];	  // �����o��ʒu
	static const float			  SMOKE_SPEED;				  // ���̏��鑬��
	static const int			  MAX_SMOKE_COUNT;			  // ���̏��鑬��
	ENEMYHITEFFECT				  m_hitEffectState[MAX_ENEMY];
															  // �q�b�g�G�t�F�N�g�̏��
	DirectX::SimpleMath::Vector3  m_hitPos[MAX_ENEMY];		  // �q�b�g�G�t�F�N�g���o��ʒu
	int							  m_hitAnimationCount[MAX_ENEMY];
															  // �G�t�F�N�g�A�j���[�V�����p�̃J�E���g
	int							  m_shockCount[MAX_ENEMY];	  // �G�t�F�N�g���o�Ă���̃J�E���g
	DirectX::SimpleMath::Vector3  m_pointPos[MAX_ENEMY];	  // ���_���o��ʒu
	DirectX::SimpleMath::Vector2  m_pointSize[MAX_ENEMY];	  // ���_�̑傫��

	DANGERDIRECTION				  m_dengerousDirLR;			  // �댯����

	static const int			  BASE_LENGTH;				  // ��ƂȂ鋗��
	float						  m_compereLength[MAX_ENEMY]; // �������r
	int							  m_lengthTmp;				  // �v���C���[�Ƃ̍ŒZ�����̓G���L��

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureHP[HP_NUM];		  // �e�N�X�`���n���h��(HP�o�[)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_texturePoint[MAX_TYPE];	  // �e�N�X�`���n���h��(���_)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureHitEffect[MAX_EFFECT]; // �e�N�X�`���n���h��(�q�b�g�G�t�F�N�g)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureSmoke;			  // �e�N�X�`���n���h��(���ꉉ�o�p��)

	std::unique_ptr<DirectX::AlphaTestEffect> 
								  m_batchEffect;			  // �G�t�F�N�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> 
								  m_batch;					  // �v���~�e�B�u�o�b�`
	Microsoft::WRL::ComPtr<ID3D11InputLayout> 
								  m_inputLayout;			  // ���̓��C�A�E�g

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameEnemyManager();
	// �f�X�g���N�^
	~GameEnemyManager();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// ����
	/// </summary>
	void Create();
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�o�ߎ���</param>
	/// <param name="playerPos">�v���C���[�̈ʒu</param>
	/// <param name="roadType">���̎��</param>
	/// <param name="assaultPoint">���̔ԍ�(���Ԗڂ����擾)</param>
	/// <param name="cameraDir">�v���C���[�̌���(�J�����̌���)</param>
	/// <returns>�I�����</returns>
	bool Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos,
				int roadType, int assaultPoint, 
				DirectX::SimpleMath::Vector3& cameraDir, 
				DirectX::SimpleMath::Vector3* chestEntryPos, float chestHeight, std::array<bool, 3> isChestOpen);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="matrixManager">�s��Ǘ�</param>
	/// <param name="eyePos">�v���C���[�̎��_</param>
	void Render(MatrixManager* matrixManager, DirectX::SimpleMath::Vector3 eyePos);
	/// <summary>
	/// �p������
	/// </summary>
	void Depose();

	//-----------------------------------Getter-----------------------------------//
	// �G�̍ő吔���擾
	int							   GetMaxEnemyNum()       { return MAX_ENEMY; }
	// �G�̈ʒu���擾
	DirectX::SimpleMath::Vector3   GetPos(int i)		  { return mp_enemy[i]->GetPos(); }
	// �G�̏�Ԃ��擾
	bool						   GetEnemyState(int i)   { return mp_enemy[i]->GetState(); }
	// �G�̎�ނ��擾
	GameEnemy::EnemyType		   GetEnemyType(int i)    { return mp_enemy[i]->GetType(); }
	// �G��HP���擾
	int							   GetEnemyHP(int i)	  { return mp_enemy[i]->GetHP(); }
	// �G�̏Փ˔�������擾
	Collision::Sphere			   GetEnemyCollide(int i) { return mp_enemy[i]->GetCollision(); }
	// �v���C���[���猩���G�̕������擾
	DANGERDIRECTION				   GetDangerDir()         { return m_dengerousDirLR; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//
	// �G�̏�Ԃ�ݒ�
	void	SetEnemyState(int i, bool flag)		{mp_enemy[i]->SetState(flag); }
	// �G��HP��ݒ�
	void	SetEnemyHP(int i, int hp)			{ mp_enemy[i]->SetHP(hp); }
	// �ڐG�����ݒ�
	void	SetHit(int i, bool flag, DirectX::SimpleMath::Vector3 hitPos)			{ mp_enemy[i]->SetHit(flag); m_hitPos[i] = /*mp_enemy[i]->GetPos()*/hitPos; }
	//----------------------------------------------------------------------------//

	/// <summary>
	/// ���ꉉ�o�ݒ�
	/// </summary>
	/// <param name="i">�G�̔z��</param>
	void ShockEnemy(int i);
	
private:
	//-----------------------------------�X�V�֐�-----------------------------------//
	/// <summary>
	/// ���ꉉ�o�X�V
	/// </summary>
	void UpdateSmoke();

	/// <summary>
	/// ���_���o�X�V
	/// </summary>
	void UpdatePoint();

	/// <summary>
	/// �P���\���ǂ�����Ԃ�
	/// </summary>
	/// <param name="roadType">�`�F�b�N���铹�̎��</param>
	/// <returns>true=�P���\�Afalse=�P���s��</returns>
	bool IsAssault(int roadType);

	/// <summary>
	/// �G�����Ǘ�
	/// </summary>
	/// <param name="assultP">�P���|�C���g</param>
	/// <param name="playerPos">�v���C���[�̈ʒu</param>
	void CreateEnemy(int assultP, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3* chestEntryPos, float chestHeight, std::array<bool, 3> isChestOpen);

	/// <summary>
	/// �G�ړ��Ǘ�
	/// </summary>
	/// <param name="timer">�o�ߎ���</param>
	/// <param name="playerPos">�v���C���[�̈ʒu</param>
	/// <param name="cameraDir">�v���C���[�̌���(�J�����̌���)</param>
	/// <param name="waveValue">�T�C���g�œ������p�̒l</param>
	void MoveEnemy(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3& cameraDir, float& waveValue);
	//------------------------------------------------------------------------------//

	//-----------------------------------�`��֐�-----------------------------------//
	/// <summary>
	/// HP�\��
	/// </summary>
	/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="enemyID">�G��ID(�^�C�v)</param>
	void DrawHP(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world, int enemyID);
	/// <summary>
	/// �q�b�g�G�t�F�N�g�\��
	/// </summary>
	/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="enemyEffectState">�G�̃q�b�g�G�t�F�N�g�̏��</param>
	void DrawHitEffect(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world, ENEMYHITEFFECT enemyEffectState);
	/// <summary>
	/// ���_�\��
	/// </summary>
	/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="enemyID">�G��ID(�^�C�v)</param>
	void DrawPoint(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world, int enemyID, DirectX::SimpleMath::Vector2 &size);
	/// <summary>
	/// ���ꉉ�o�\��
	/// </summary>
	/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="drawAlpha">�����x</param>
	void DrawSmoke(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world,float &drawAlpha);
	//------------------------------------------------------------------------------//

};