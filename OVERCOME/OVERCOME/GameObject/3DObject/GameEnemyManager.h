//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.h
// Summary. GameEnemyManagerClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <vector>
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
	enum DANGERDIRECTION
	{
		NONE,

		DIR_FRONT,
		DIR_BACK,
		DIR_RIGHT,
		DIR_LEFT,
	};

private:
	static const int			  MAX_ENEMY = 100;			  // �ő�G��

	static const int			  MAX_SPAWN_TIME;			  // �G���o���ł���ő厞��(�t���[����)
	int							  m_spawnElapsedTime;		  // �G���o�����Ă���̌o�ߎ���(�t���[����)
	
	static const int			  RESPAWN_NEED_TIME;		  // ���X�|�[���ɕK�v�Ȏ���(�t���[����)
	int                           m_respawnTime;              // ���X�|�[������(�t���[����)

	static const float			  CONTROL_VELOCITY;			  // ���x����

	GameEnemy*					  mp_enemy[MAX_ENEMY];		  // �G�I�u�W�F�N�g
	DirectX::SimpleMath::Vector3  m_shockPos[MAX_ENEMY];	  // �G�t�F�N�g���o��ʒu
	static const float			  SMOKE_SPEED;				  // ���̏��鑬��
	static const int			  MAX_SMOKE_COUNT;			  // ���̏��鑬��
	int							  m_shockCount[MAX_ENEMY];	  // �G�t�F�N�g���o�Ă���̃J�E���g

	DANGERDIRECTION				  m_dengerousDirLR;			  // �댯����

	static const int			  BASE_LENGTH;				  // ��ƂȂ鋗��
	float						  m_compereLength[MAX_ENEMY]; // �������r
	int							  m_lengthTmp;				  // �v���C���[�Ƃ̍ŒZ�����̓G���L��

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
	bool Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, int roadType, int assaultPoint, DirectX::SimpleMath::Vector3& cameraDir);
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
	// �G�̏Փ˔�������擾
	Collision::Sphere			   GetEnemyCollide(int i) { return mp_enemy[i]->GetCollision(); }
	// �v���C���[���猩���G�̕������擾
	DANGERDIRECTION				   GetDangerDir()         { return m_dengerousDirLR; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//
	// �G�̏�Ԃ�ݒ�
	void	SetEnemyState(int i, bool flag)		{mp_enemy[i]->SetState(flag); }
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
	void CreateEnemy(int assultP, DirectX::SimpleMath::Vector3& playerPos);

	/// <summary>
	/// �G�ړ��Ǘ�
	/// </summary>
	/// <param name="timer">�o�ߎ���</param>
	/// <param name="playerPos">�v���C���[�̈ʒu</param>
	/// <param name="cameraDir">�v���C���[�̌���(�J�����̌���)</param>
	void MoveEnemy(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3& cameraDir);
	//------------------------------------------------------------------------------//

	//-----------------------------------�`��֐�-----------------------------------//
	/// <summary>
	/// ���ꉉ�o�\��
	/// </summary>
	/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
	/// <param name="world">���[���h�s��</param>
	/// <param name="drawAlpha">�����x</param>
	void DrawSmoke(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world,int &drawAlpha);
	//------------------------------------------------------------------------------//

};