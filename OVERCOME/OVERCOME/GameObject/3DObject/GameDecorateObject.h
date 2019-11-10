//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.h
// Summary. GameDecorateObjectClass
// Date.    2019/10/23
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <array>
#include <Model.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameDecorateObject
{
// �����o�[�ϐ�
public:
	static const int						MAX_CHEST_NUM = 3;				// �`�F�X�g�̍ő吔

private:
	DirectX::SimpleMath::Matrix				m_world;						// ���[���h���W

	static const int						MAX_DOOR_NUM = 6;				// �h�A�̐�
	DirectX::SimpleMath::Matrix				m_doorRota[MAX_DOOR_NUM];		// �h�A�I�u�W�F�N�g�̉�]�s��
	DirectX::SimpleMath::Vector3			m_doorTrans[MAX_DOOR_NUM];		// �h�A�I�u�W�F�N�g�̈ړ��s��
	bool									m_isMoveDoor[MAX_DOOR_NUM];		// �h�A�������o�������ǂ���
	bool									m_isShowDoor[MAX_DOOR_NUM];		// �h�A��\���ł����Ԃ��ǂ���

	DirectX::SimpleMath::Matrix				m_chestRota[MAX_CHEST_NUM];		// �`�F�X�g�I�u�W�F�N�g�̉�]�s��
	DirectX::SimpleMath::Vector3			m_chestTrans[MAX_CHEST_NUM];	// �`�F�X�g�I�u�W�F�N�g�̈ړ��s��
	DirectX::SimpleMath::Vector3			m_chestTempPos[MAX_CHEST_NUM];	// �`�F�X�g�I�u�W�F�N�g�̊�{�ʒu
	float									m_chestHeight;					// �`�F�X�g�I�u�W�F�N�g�̍���
	int										m_shakeCount[MAX_CHEST_NUM];	// �`�F�X�g���U������܂ł̃J�E���g
	int										m_shakeVelCount[MAX_CHEST_NUM];	// �`�F�X�g����������ɐU�����Ă���J�E���g
	int										m_shakeNeedCount[MAX_CHEST_NUM];// �`�F�X�g���U������܂łɕK�v�ȃJ�E���g
	bool									m_isChestOpen[MAX_CHEST_NUM];	// �`�F�X�g���J���Ă��邩�ǂ���
	bool									m_isChestSound[MAX_CHEST_NUM];	// �`�F�X�g�̊J��SE���Đ��������ǂ���
	static const int						SHAKE_COUNT;					// ������ɐU������J�E���g
	static const DirectX::SimpleMath::Vector3
											SHAKE_VEL;						// �U���������

	std::unique_ptr<DirectX::Model>			mp_modelEnemyDoor[MAX_DOOR_NUM];// �G���o�Ă���h�A���f���I�u�W�F�N�g
	std::unique_ptr<DirectX::Model>			mp_modelSmallRoom;				// ���������f���I�u�W�F�N�g
	std::unique_ptr<DirectX::Model>			mp_modelEnemyChestOpen[MAX_CHEST_NUM];		// �G���o�Ă���`�F�X�g���f���I�u�W�F�N�g
	std::unique_ptr<DirectX::Model>			mp_modelEnemyChestClose[MAX_CHEST_NUM];		// �G���o�Ă���`�F�X�g���f���I�u�W�F�N�g

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

	//--------------------Getter--------------------//

	// �`�F�X�g�̈ʒu���擾
	DirectX::SimpleMath::Vector3*	GetChestPos()		{ return m_chestTrans; }

	// �`�F�X�g�̍������擾
	float							GetChestHeight()	{ return m_chestHeight; }

	// �`�F�X�g�̊J��Ԃ��擾
	std::array<bool, 3>				GetChestIsOpen()	{ return{ m_isChestOpen[0], m_isChestOpen[1], m_isChestOpen[2] }; }

	//----------------------------------------------//

private:
	/// <summary>
	/// �`��O�ɂ��郏�[���h�s��̐ݒ�
	/// </summary>
	/// <param name="rota">��]�s��</param>
	/// <param name="trans">�ړ��s��</param>
	DirectX::SimpleMath::Matrix SetWorldMatrix(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix rota, DirectX::SimpleMath::Matrix trans);

	/// <summary>
	/// �G�o���h�A�̍X�V
	/// </summary>
	/// <param name="roadID">��ID</param>
	void UpdateEnemyDoor(int roadID);

	/// <summary>
	/// �G�o���`�F�X�g�̍X�V
	/// </summary>
	/// <param name="roadID">��ID</param>
	void UpdateEnemyChest(int roadID);
};