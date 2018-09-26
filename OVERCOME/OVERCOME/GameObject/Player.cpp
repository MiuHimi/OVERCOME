//////////////////////////////////////////////////////////////
// File.    Player.cpp
// Summary. PlayerClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "Player.h"

#include "../ExclusiveGameObject/InputManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
{
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
Player::Player(Game * game) : mp_game(game),
                              m_pos(22.5f, 2.0f, 7.5f),
	                          m_vel(0.0f, 0.0f, 0.0f)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

/// <summary>
/// ����������
/// </summary>
void Player::Initialize()
{
	//m_direction = 0.0f;
	m_direction = XMConvertToRadians(180.0f);
	m_height = 1.75f;

	m_jumpForce = 0.0f;
	m_gravity = 0.03f;

	m_world = SimpleMath::Matrix::Identity;
}
/// <summary>
/// ��������
/// </summary>
void Player::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(mp_game->GetDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelPlayer = Model::CreateFromCMO(mp_game->GetDevice(), L"Resources\\Models\\player.cmo", fx);

	// �v���C���[�̍쐬
	SetGame(mp_game);
	SetModel(m_modelPlayer.get());
	
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool Player::Update(DX::StepTimer const & timer)
{
	// ���x������
	m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	
	// �v���C���[����
	if (InputManager::GetInstance().GetKeyState().Up)
	{
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, 0.2f);
	}
	if (InputManager::GetInstance().GetKeyState().Down)
	{
		m_vel = SimpleMath::Vector3(0.0f, 0.0f, -0.2f);
	}
	if (InputManager::GetInstance().GetKeyState().Left)
	{
		m_direction += XMConvertToRadians(2.0f);
	}
	if (InputManager::GetInstance().GetKeyState().Right)
	{
		m_direction += XMConvertToRadians(-2.0f);
	}

	if (InputManager::GetInstance().GetKeyState().Space && m_isJump == false ||
		InputManager::GetInstance().GetKeyState().Space && m_isCollide == true && m_pos.y >= 0.95f)
	{
		m_isJump = true;
		m_vel.y = 0.0f;
		m_jumpForce = 0.5f;
	}

	if (m_isJump == true)
	{
		m_vel.y += m_jumpForce;
		if (m_vel.y < -1.0f) m_vel.y = -1.0f;
		m_jumpForce -= m_gravity;
	}
	else if (m_isJump == false && m_isCollide == false)
	{
		m_vel.y -= m_gravity;
	}
	else if (m_isJump == true && m_isCollide == true)
	{
		m_vel.y = 0.0f;
	}

	/*if (m_isJump == false)
	{
		if (m_pos.y > 0.0f)
		{
			m_vel.y += m_jumpForce;
			m_jumpForce -= m_gravity;
		}
	}*/

	if (m_pos.y <= 0.0f)m_pos.y = 0.0f;

	// �v���C���[���ړ�
	m_rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), m_direction);
	m_vel = SimpleMath::Vector3::Transform(m_vel, m_rotation);
	m_pos += m_vel;

	// ���[���h�s��̍쐬
	m_world = SimpleMath::Matrix::CreateFromQuaternion(m_rotation) * SimpleMath::Matrix::CreateTranslation(m_pos);

	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + (m_height / 2.0f), m_pos.z);      // ���E���̒��S
	box.r = DirectX::SimpleMath::Vector3(0.5f, m_height / 2.0f, 0.5f);                        // �e���a0.5
	SetCollision(box);

	return true;
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Render(DirectX::SimpleMath::Matrix view)
{
	/*SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	trans = SimpleMath::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);*/
	//m_rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), m_direction);
	//rot = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(m_direction));

	// ���̕`��
	m_modelPlayer->Draw(mp_game->GetContext(), *mp_game->GetState(), m_world, view, mp_game->GetProjection());
}

/// <summary>
/// �폜����
/// </summary>
void Player::Depose()
{
}

Player* Player::GetPlayer()
{
	return this;
}
