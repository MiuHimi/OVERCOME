//////////////////////////////////////////////////////////////
// File.    GameCamera.h
// Summary. GameCameraClass
// Date.    2018/10/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

#include "Camera.h"

#include "Player.h"

class Player;
class GameCamera : public Camera
{
// �����o�[�ϐ�
public:

private:
	static const float                DEFAULT_CAMERA_DISTANCE;   // �J�����̋���
	static const float                ROTATE_MAG;                // ��]�{��(100�`500���x�X�g)

	float                             m_aroundAngle;             // ��]�p

	DirectX::SimpleMath::Vector2      m_angle;                   // ���݂̉�]�p
	DirectX::SimpleMath::Vector2      m_angleTmp;                // �ۑ����Ă����]�p

	DirectX::SimpleMath::Vector3      m_cameraDir;               // �J�����̌����̃x�N�g��

	DirectX::SimpleMath::Vector2      m_mousePos;                // �񎟌���̍��W

	DirectX::SimpleMath::Vector2      m_dragUnit;                // �h���b�O���ꂽ���W
	bool                              m_checkMousePos;           // �}�E�X�̏����ʒu�w��p�ϐ�

	int                               m_scrollWheelValue;        // �X�N���[���z�C�[���l

	DirectX::SimpleMath::Quaternion   m_rotationX;               // ��](�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion   m_rotationY;               // ��](�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion   m_rotationTmpX;            // ��]�ۑ�(�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion   m_rotationTmpY;            // ��]�ۑ�(�N�H�[�^�j�I��)

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameCamera();
	GameCamera(int windowWidth, int windowHeight);
	// �f�X�g���N�^
	~GameCamera();

	// �X�V(��ɃJ�����̐؂�ւ����s��)
	bool Update(DX::StepTimer const & timer, Player* player);

	// ���_�𒍎��_�ɂ��A��������񂷂�J����
	void OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos);

	// �f�o�b�N�p�J����
	void DebugCamera(DirectX::SimpleMath::Vector3 debugPos);

	// �����Ă���Ƃ��p�J����
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// ��납��ǂ�������J����
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

	// �}�E�X�Ŏ��_�ړ�����J����
	void MouseOperateCamera(DirectX::SimpleMath::Vector3 target);

	
	// �J�����̌����̃x�N�g�����擾
	DirectX::SimpleMath::Vector3 GetCameraAngle() { return m_cameraDir; }

	// �}�E�X�������ʒu�ɔz�u���ꂽ��true
	bool GetStartPosMouse() { return m_checkMousePos; }

private:
	/// <summary>
	/// ��ʃT�C�Y�̐ݒ�֐�
	/// </summary>
	/// <param name="windowWidth">�E�C���h�E�T�C�Y�i���j</param>
	/// <param name="windowHeight">�E�C���h�E�T�C�Y�i�����j</param>
	void SetWindowSize(int windowWidth, int windowHeight);

	/// <summary>
	/// �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ� (���Βl)
	/// </summary>
	/// <param name="x">���݂̃|�C���^��X���W</param>
	/// <param name="y">���݂̃|�C���^��Y���W</param>
	void Motion(int x, int y);

};
