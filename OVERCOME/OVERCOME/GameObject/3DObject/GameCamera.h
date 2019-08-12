//////////////////////////////////////////////////////////////
// File.    GameCamera.h
// Summary. GameCameraClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "Camera.h"

#include "Player.h"

class Player;
class SceneManager;
class GameCamera : public Camera
{
// �����o�[�ϐ�
public:

private:
	static const float                DEFAULT_CAMERA_DISTANCE;   // �J�����̋���
	static const float                ROTATE_MAG;                // ��]�{��(100�`500���x�X�g)

	float                             m_aroundAngleX;            // ��]�p(�x�AX��)
	float                             m_aroundAngleY;            // ��]�p(�x�AY��)

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
	DirectX::SimpleMath::Quaternion   m_toScreenOutRotaX;        // ��ʊO�ł̉�]�ۑ�(�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion   m_toScreenOutRotaY;        // ��ʊO�ł̉�]�ۑ�(�N�H�[�^�j�I��)

	DirectX::SimpleMath::Quaternion   m_reformRota;              // �����p��](�N�H�[�^�j�I��)
	DirectX::SimpleMath::Vector2      m_mousePosDbg;             // �񎟌���̍��W

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameCamera();
	GameCamera(int windowWidth, int windowHeight);
	// �f�X�g���N�^
	~GameCamera();

	// �X�V(��ɃJ�����̐؂�ւ����s��)
	bool Update(DX::StepTimer const & timer, DirectX::SimpleMath::Vector3& playerPos, float playerHeight, DirectX::SimpleMath::Vector3& playerDir);

	//-----------------------------------Getter-----------------------------------//

	// �J�����̌����̃x�N�g�����擾
	DirectX::SimpleMath::Vector3 GetCameraAngle() { return m_cameraDir; }

	// �}�E�X�������ʒu�ɔz�u���ꂽ��true
	bool GetStartPosMouse()                       { return m_checkMousePos; }

	float GetAngle() { return m_rotationY.y; }
	//----------------------------------------------------------------------------//

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

	/// <summary>
	/// ���_�𒍎��_�ɂ��A��������񂷂�J����
	/// </summary>
	/// <param name="eyePos">�����_</param>
	void OriginPointAroundCamera(DirectX::SimpleMath::Vector3 eyePos);
	/// <summary>
	/// �f�o�b�N�p�J����
	/// </summary>
	/// <param name="debugPos"></param>
	void DebugCamera(DirectX::SimpleMath::Vector3 debugPos);

	/// <summary>
	/// �����Ă���Ƃ��p�J����
	/// </summary>
	/// <param name="target">�����_</param>
	/// <param name="direction">�i�s����</param>
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	/// <summary>
	/// ��납��ǂ�������J����
	/// </summary>
	/// <param name="target">�����_</param>
	/// <param name="direction">�i�s����</param>
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

	/// <summary>
	/// ��_�J����
	/// </summary>
	/// <param name="target">�����_</param>
	/// <param name="cameraPoint">�J�����̈ʒu</param>
	void FixedPointCamera(DirectX::SimpleMath::Vector3 &target, DirectX::SimpleMath::Vector3 &cameraPoint);

	/// <summary>
	/// ��_����̈�̃T�C�Y���̒��𗬂��悤�ɓ����J����
	/// </summary>
	/// <param name="basePoint">��_</param>
	/// <param name="width">��(X)</param>
	/// <param name="height">����(Y)</param>
	/// <param name="depth">���s(Z)</param>
	/// <param name="areaSize">�̈�̃T�C�Y</param>
	void FadeCamera(DirectX::SimpleMath::Vector3 basePoint, float width, float height, float depth, float areaSize);

	/// <summary>
	/// �}�E�X�Ŏ��_�ړ�����J����
	/// </summary>
	/// <param name="target">�����_</param>
	void MouseOperateCamera(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 playerDirction);
};
