//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.h
// Summary. GameDecorateObjectClass
// Date.    2019/10/23
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <Model.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameDecorateObject
{
// �����o�[�ϐ�
public:

private:
	DirectX::SimpleMath::Matrix				m_rotationY;						// Y����]
	DirectX::SimpleMath::Matrix				m_world;						// ���[���h���W

	DirectX::SimpleMath::Vector3			m_trans[6];						// ���[���h���W

	std::unique_ptr<DirectX::Model>			mp_modelEnemyDoor[6];			// �G���o�Ă���h�A���f���I�u�W�F�N�g

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameDecorateObject();
	// �f�X�g���N�^
	~GameDecorateObject();

	/// <summary>
	/// ����
	/// </summary>
	void Create();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	/// <returns>�I�����</returns>
	bool Update(int in);
	/// <summary>
	/// �`��
	/// </summary>
	void Render(MatrixManager* matrixManager);
	/// <summary>
	/// �p������
	/// </summary>
	void Depose();

private:

};