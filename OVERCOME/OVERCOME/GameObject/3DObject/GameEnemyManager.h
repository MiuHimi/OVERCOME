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
	static const int              m_maxEnemyNum = 100;       // �ő�G��
	static const int              m_maxAliveDist = 50;		 // 

	static const int              m_needRespawnTime = 90;    // ���X�|�[���ɕK�v�Ȏ���(�t���[��)
	int                           m_respawnTime;             // ���X�|�[������(�t���[��)

	GameEnemy*                    mp_enemy[m_maxEnemyNum];   // �G�Ǘ�
	Player*                       mp_player;				 // �v���C���[�I�u�W�F�N�g
	std::unique_ptr<GameCamera>   mp_gameCamera;             // �J�����I�u�W�F�N�g

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousH;       // �e�N�X�`���n���h��(�댯�T�C����)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousV;       // �e�N�X�`���n���h��(�댯�T�C���c)

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

	double                       m_compereLength[m_maxEnemyNum]; // �������r
	int                          m_lengthTmp;                 // �v���C���[�ɍŒZ�����̓G���L��

	bool                         m_assault;                   // �G�ړ���

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
	bool Update(DX::StepTimer const& timer, Player* player);
	// �`��
	void Render(MatrixManager* matrixManager);
	// �p������
	void Depose();

	//-----------------------------------Getter-----------------------------------//

	int GetMaxEnemyNum()                       { return m_maxEnemyNum; }
	DirectX::SimpleMath::Vector3 GetPos(int i) { return mp_enemy[i]->GetPos(); }
	bool GetEnemyState(int i)				   { return mp_enemy[i]->GetState(); }
	Collision::Sphere GetEnemyCollide(int i)   { return mp_enemy[i]->GetCollision(); }

	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	void SetEnemyState(int i, bool flag) {mp_enemy[i]->SetState(flag); }
	//----------------------------------------------------------------------------//

private:


};