//////////////////////////////////////////////////////////////
// File.    GameBulletManager.cpp
// Summary. GameBulletManagerClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/InputManager.h"
#include "GameBulletManager.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

#include "../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameBulletManager::GameBulletManager()
	: m_reloadTime(0),
	 m_shootingFlag(false)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameBulletManager::~GameBulletManager()
{
}

/// <summary>
/// ������
/// </summary>
void GameBulletManager::Initialize()
{
	for (int i = 0; i < m_maxBulletNum; i++)
	{
		GameBullet* bullet = new GameBullet(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		// �u���b�N�̏����i�[
		mp_bullet.push_back(bullet);
	}
}
/// <summary>
/// ����
/// </summary>
void GameBulletManager::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");

	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it) 
	{
		/*std::unique_ptr<Model> model;
		model = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
		(*it)->SetModel(model.get());
		SetModel(model.get());*/

		// ���f�����쐬
		std::unique_ptr<Model> model;
		model = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
		// ���f�������ꂼ��(�v�f)�ɃZ�b�g
		(*it)->SetModel(std::move(model));
		// �Փ˔���p���f���ݒ�
		SetModel(model.get());
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameBulletManager::Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3 ShootPos, DirectX::SimpleMath::Vector3 cameraDir)
{
	//InputManager::SingletonGetInstance().Update();

	if (m_shootingFlag)
	{
		m_reloadTime++;
	}

	if (m_reloadTime >= m_needReloadTime)
	{
		m_reloadTime = 0;
		m_shootingFlag = false;
	}

	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		if (!(*it)->GetState())
		{
			if (!m_shootingFlag)
			{
				if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
				//if(InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
				{
					// �c�e���������甭�ˏ���
					(*it)->SetState(true);
					(*it)->SetPos(ShootPos);
					(*it)->SetVel(cameraDir);
					m_shootingFlag = true;
					break;
				}
			}
		}
		
	}

	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		// ���˂ł���e�̍X�V
		if ((*it)->GetState())
		{
			(*it)->Update(timer);
		}
	}

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameBulletManager::Render()
{
	int i = 0;
	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		if ((*it)->GetState())
		{
			(*it)->Render();
		}

		i++;
	}
}

/// <summary>
/// �p��
/// </summary>
void GameBulletManager::Depose()
{
}
