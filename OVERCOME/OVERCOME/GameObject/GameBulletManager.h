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
class GameBulletManager : public CollisionSphere
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::vector<GameBullet*>     mp_bullet;                  // �e�Ǘ��z��

	static const int             m_maxBulletNum = 5;         // �ő�e��
	static const int             m_needReloadTime = 60;      // ���e���˂܂ł̎���(�t���[��)

	int                          m_reloadTime;               // ���e���U����(�t���[��)
	bool                         m_shootingFlag;             // ���ˍς݃t���O(m_reloadTime�J�E���g�J�n)

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
	void Render();
	// �p������
	void Depose();


private:


};