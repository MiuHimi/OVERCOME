//////////////////////////////////////////////////////////////
// File.    GameBulletManager.h
// Summary. GameBulletManagerClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

#include <vector>

#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "GameBullet.h"

#include "../ExclusiveGameObject/CollisionSphere.h"

class GameBullet;
class MatrixManager;
class GameBulletManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	//std::vector<GameBullet*>     mp_bullet;                  // �e�Ǘ��z��
	
	static const int             m_maxBulletNum = 5;         // �ő�e��
	static const int             m_needReloadTime = 60;      // ���e���˂܂ł̎���(�t���[��)

	int                          m_reloadTime;               // ���e���U����(�t���[��)
	bool                         m_shootingFlag;             // ���ˍς݃t���O(m_reloadTime�J�E���g�J�n)

	GameBullet*                  mp_bullet[m_maxBulletNum];

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameBulletManager();
	// �f�X�g���N�^
	~GameBulletManager();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3 ShootPos, DirectX::SimpleMath::Vector3 cameraDir);
	// �`��
	void Render(MatrixManager* matrixManager);
	// �p������
	void Depose();

	//std::vector<GameBullet*> GetBullet() { return mp_bullet; }
	int GetMaxBulletNum() { return m_maxBulletNum; }
	DirectX::SimpleMath::Vector3 GetPos(int i) { return mp_bullet[i]->GetPos(); }
	bool GetBulletState(int i) { return mp_bullet[i]->GetState(); }
	void SetBulletState(int i, bool flag) { return mp_bullet[i]->SetState(flag); }
	Collision::Sphere GetBulletCollide(int i) { return mp_bullet[i]->GetCollision(); }

private:


};