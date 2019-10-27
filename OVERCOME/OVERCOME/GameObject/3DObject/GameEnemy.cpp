//////////////////////////////////////////////////////////////
// File.    GameEnemy.cpp
// Summary. GameEnemyClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "GameEnemy.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
//-----���v�l��100�ɂȂ�悤�ɐݒ肷��-----//
const int GameEnemy::NORMAL_ENEMY_PROBABILITY = 50;
const int GameEnemy::POWER_ENEMY_PROBABILITY = 30;
const int GameEnemy::SPEED_ENEMY_PROBABILITY = 20;
//-----------------------------------------//


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameEnemy::GameEnemy(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model)
	: m_pos(0.0f, 0.0f, 0.0f),
	  m_vel(0.0f, 0.0f, 0.0f),
	  m_dir(0.0f, 0.0f, 0.0f),
	  m_size(0.0f),
	  m_prob{ 0 }, m_type(EnemyType::NUM),m_hp(0),
	  m_rotaX(SimpleMath::Quaternion::Identity),
   	  m_rotaY(SimpleMath::Quaternion::Identity),
	  m_state(false),
	  m_isHit(false), m_shock(false),
	  m_world(DirectX::SimpleMath::Matrix::Identity),
	  mp_modelEnemy(nullptr)
{
	m_pos = pos;
	m_vel = vel;
	m_size = 1.2f;
	m_dir = SimpleMath::Vector3(0.0f, 0.0f, 1.0f);

	int probability = 100;
	for (int i = 0; i < EnemyType::NUM; i++)
	{
		// ���ꂼ��̃^�C�v�̊m����ݒ�
		switch (i)
		{
		case EnemyType::NORMAL:
			probability -= NORMAL_ENEMY_PROBABILITY;
			m_prob[i] = probability;
			break;
		case EnemyType::POWER:
			probability -= POWER_ENEMY_PROBABILITY;
			m_prob[i] = probability;
			break;
		case EnemyType::SPEED:
			probability -= SPEED_ENEMY_PROBABILITY;
			m_prob[i] = probability;
			break;
		default:
			// ���݂��Ȃ�����
			throw std::range_error("None Objects.");
			break;
		}
		// �m���ݒ�~�X
		if (probability < 0)
		{
			throw std::range_error("Set probability failure.");
		}
	}
	

	m_state = stateFlag;
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameEnemy::~GameEnemy()
{
}

void GameEnemy::Create(int probability)
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���ꂼ��̃��f�����쐬
	if (probability >= m_prob[(int)GameEnemy::SPEED] && probability < m_prob[(int)GameEnemy::POWER])
	{
		m_type = EnemyType::SPEED;
		m_hp = 1;
		mp_modelEnemy = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost_speed.cmo", fx);
	}
	else if (probability >= m_prob[(int)GameEnemy::POWER] && probability < m_prob[(int)GameEnemy::NORMAL])
	{
		m_type = EnemyType::POWER;
		m_hp = 3;
		mp_modelEnemy = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost_power.cmo", fx);
	}
	else if (probability >= m_prob[(int)GameEnemy::NORMAL])
	{
		m_type = EnemyType::NORMAL;
		m_hp = 2;
		mp_modelEnemy = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost.cmo", fx);
	}
	else
	{
		// ���݂��Ȃ�����
		throw std::range_error("None Objects.");
	}
	
	// �Փ˔���p���f���ݒ�
	Obj3D::SetModel(mp_modelEnemy.get());

	// �t�H�O�̐ݒ�
	SetFogEffectDistance(4.0f, 10.0f);

	// �Փ˔���p�I�u�W�F�N�g�ݒ�
	m_sphere.c = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_sphere.r = 0.3f;
	SetCollision(m_sphere);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameEnemy::Update(DX::StepTimer const & timer)
{
	// �ړ�
	m_pos += m_vel;

	// ��]�s����쐬
	SimpleMath::Matrix rota = SimpleMath::Matrix::CreateFromQuaternion(m_rotaX) * SimpleMath::Matrix::CreateFromQuaternion(m_rotaY);
	// ���[���h�s����쐬
	m_world = rota * SimpleMath::Matrix::CreateTranslation(m_pos);

	// �Փ˔���p�I�u�W�F�N�g�X�V
	m_sphere.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);      // ���E���̒��S
	m_sphere.r = 1.0f;                                                         // ���a
	SetCollision(m_sphere);

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameEnemy::Render(MatrixManager* matrixManager)
{
	mp_modelEnemy->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		                m_world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// �t�H�O�̃X�^�[�g�ƃG���h��ݒ�
/// </summary>
/// <param name="start">���ʂ�������n�߂鋗��</param>
/// <param name="end">���ʂ����S�ɂ����鋗��</param>
void GameEnemy::SetFogEffectDistance(float start, float end)
{
	mp_modelEnemy->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(start);
			fog->SetFogEnd(end);
			fog->SetFogColor(Colors::Black);
		}
	});
}
