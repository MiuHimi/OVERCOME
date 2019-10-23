//////////////////////////////////////////////////////////////
// File.    GameBulletManager.cpp
// Summary. GameBulletManagerClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "GameBulletManager.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

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
	for (int i = 0; i < m_maxBulletNum; i++)
	{
		delete mp_bullet[i];
		mp_bullet[i] = nullptr;
	}
}

/// <summary>
/// ������
/// </summary>
void GameBulletManager::Initialize()
{
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"ScenePlay.acb", L"ScenePlay.awb");
}
/// <summary>
/// ����
/// </summary>
void GameBulletManager::Create()
{
	for (int i = 0; i < m_maxBulletNum; i++)
	{
		mp_bullet[i] = new GameBullet(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_bullet[i]->Create();
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameBulletManager::Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3 ShootPos, DirectX::SimpleMath::Vector3 cameraDir)
{
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// ���������ォ��J�E���g�J�n
	if (m_shootingFlag)
	{
		m_reloadTime++;
	}

	// ��b�o������Ăюˌ��\��
	if (m_reloadTime >= m_needReloadTime)
	{
		m_reloadTime = 0;
		m_shootingFlag = false;
	}

	//for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	//{
	//	if (!(*it)->GetState())
	//	{
	//		if (!m_shootingFlag)
	//		{
	//			if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	//			//if(InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	//			{
	//				// �c�e���������甭�ˏ���
	//				(*it)->SetState(true);
	//				(*it)->SetPos(ShootPos);
	//				(*it)->SetVel(cameraDir);
	//				m_shootingFlag = true;
	//				break;
	//			}
	//		}
	//	}
	//}

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		if (!mp_bullet[i]->GetState())
		{
			if (!m_shootingFlag)
			{				
				if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
				{
					// �c�e���������甭�ˏ���
					mp_bullet[i]->SetState(true);
					mp_bullet[i]->SetPos(ShootPos);
					SimpleMath::Vector3 vel = cameraDir;
					vel /= 2.0f;
					mp_bullet[i]->SetVel(vel);
					m_shootingFlag = true;

					// �e����SE
					ADX2Le* adx2le = ADX2Le::GetInstance();
					adx2le->Play(2);

					break;
				}
			}
		}
	}

	//for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	//{
	//	// ���˂ł���e�̍X�V
	//	if ((*it)->GetState())
	//	{
	//		(*it)->Update(timer);
	//	}
	//}

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		if (mp_bullet[i]->GetState())
		{
			mp_bullet[i]->Update(timer);
		}
	}

	// �Փ˔�����̐ݒ�
	/*for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		(*it)->SetCollision((*it)->GetCollide());
	}*/

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameBulletManager::Render(MatrixManager* matrixManager)
{
	/*for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		if ((*it)->GetState())
		{
			(*it)->Render();
		}
	}*/

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		if (mp_bullet[i]->GetState())
		{
			mp_bullet[i]->Render(matrixManager);
		}
	}
}

/// <summary>
/// �p��
/// </summary>
void GameBulletManager::Depose()
{
}
