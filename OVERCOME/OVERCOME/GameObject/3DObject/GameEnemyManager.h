//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.h
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"

#include <vector>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "GameEnemy.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class GameEnemy;
class GameCamera;
class MatrixManager;
class Player;
class GameEnemyManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	static const int			  MAXENEMYNUM = 100;		 // �ő�G��
	static const int			  MAXALIVEDIST;				 // �ő吶������

	static const int              SPAWNTIME;                 // �G���o�����Ă����鎞��(�t���[��)
	int							  m_spawnElapsedTime;		 // �G���o�����Ă���̌o�ߎ���(�t���[��)

	static const int			  NEEDRESPAWNTIME;			 // ���X�|�[���ɕK�v�Ȏ���(�t���[��)
	int                           m_respawnTime;             // ���X�|�[������(�t���[��)
	
	GameEnemy*                    mp_enemy[MAXENEMYNUM];     // �G�Ǘ�
	DirectX::SimpleMath::Vector3  m_shockPos[MAXENEMYNUM];   // �G�t�F�N�g���o��ʒu
	int							  m_shockCount[MAXENEMYNUM]; // �G�t�F�N�g���o�Ă���̃J�E���g
	Player*                       mp_player;				 // �v���C���[�I�u�W�F�N�g
	std::unique_ptr<GameCamera>   mp_gameCamera;             // �J�����I�u�W�F�N�g

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousV;       // �e�N�X�`���n���h��(�댯�T�C��)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureSmoke;			 // �e�N�X�`���n���h��(���ꉉ�o�p��)

	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect; // �G�t�F�N�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>
								  m_batch;					 // �v���~�e�B�u�o�b�`
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; // ���̓��C�A�E�g

	enum DIRECTION
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT,

		NONE
	};
	DIRECTION                    m_dengerousDirFB;
	DIRECTION                    m_dengerousDirLR;

	double                       m_compereLength[MAXENEMYNUM]; // �������r
	int                          m_lengthTmp;                 // �v���C���[�ɍŒZ�����̓G���L��

	bool                         m_danger;                   // �G���v���C���[�ɂƂ��Ċ댯�Ȉʒu�ɂ�����t���O������

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameEnemyManager();
	// �f�X�g���N�^
	~GameEnemyManager();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer, Player* player, int playerNowRoadType, int assaultPoint);
	// �`��
	void Render(MatrixManager* matrixManager, DirectX::SimpleMath::Vector3 eyePos);
	// �p������
	void Depose();

	//-----------------------------------Getter-----------------------------------//

	int GetMaxEnemyNum()                       { return MAXENEMYNUM; }
	DirectX::SimpleMath::Vector3 GetPos(int i) { return mp_enemy[i]->GetPos(); }
	bool GetEnemyState(int i)				   { return mp_enemy[i]->GetState(); }
	Collision::Sphere GetEnemyCollide(int i)   { return mp_enemy[i]->GetCollision(); }

	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	void SetEnemyState(int i, bool flag) {mp_enemy[i]->SetState(flag); }
	//----------------------------------------------------------------------------//

	// ���ꉉ�o�ݒ�
	void ShockEnemy(int i);
	
private:
	// ���ꉉ�o
	void DrawSmoke(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world,int &drawAlpha);

};