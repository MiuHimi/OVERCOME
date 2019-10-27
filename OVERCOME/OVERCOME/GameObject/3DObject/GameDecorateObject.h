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
	DirectX::SimpleMath::Matrix				m_world;						// ���[���h���W

	static const int						MAX_DOOR_NUM = 6;				// �h�A�̐�
	DirectX::SimpleMath::Matrix				m_doorRota[MAX_DOOR_NUM];		// �h�A�I�u�W�F�N�g�̉�]�s��
	DirectX::SimpleMath::Vector3			m_doorTrans[MAX_DOOR_NUM];		// �h�A�I�u�W�F�N�g�̈ړ��s��
	bool									m_isMoveDoor[MAX_DOOR_NUM];		// �h�A�������o�������ǂ���
	bool									m_isShowDoor[MAX_DOOR_NUM];		// �h�A��\���ł����Ԃ��ǂ���

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
	bool Update(int roadID);
	/// <summary>
	/// �`��
	/// </summary>
	void Render(MatrixManager* matrixManager);
	/// <summary>
	/// �p������
	/// </summary>
	void Depose();

private:
	/// <summary>
	/// �`��O�ɂ��郏�[���h�s��̐ݒ�
	/// </summary>
	/// <param name="rota">��]�s��</param>
	/// <param name="trans">�ړ��s��</param>
	DirectX::SimpleMath::Matrix SetWorldMatrix(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix rota, DirectX::SimpleMath::Matrix trans);

};