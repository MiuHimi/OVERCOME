//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include <math.h>
#include <time.h>
#include "GameEnemyManager.h"
#include "Player.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_respawnTime(0),
	  mp_player(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameEnemyManager::~GameEnemyManager()
{
	// �G�I�u�W�F�N�g�폜
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		delete mp_enemy[i];
		mp_enemy[i] = nullptr;
	}

	// �v���C���[�I�u�W�F�N�g�폜
	delete mp_player;
	mp_player = nullptr;
}

/// <summary>
/// ������
/// </summary>
void GameEnemyManager::Initialize()
{
	// ����������
	srand((unsigned int)time(NULL));
}
/// <summary>
/// ����
/// </summary>
void GameEnemyManager::Create()
{
	// �G�̏������A����
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		mp_enemy[i] = new GameEnemy(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create();
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, Player* player)
{
	m_respawnTime++;

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// �G�̏P�����Ԃ�������X�V����
		if (!player->GetEnemyTime())break;
		if (!mp_enemy[i]->GetState())
		{
			if (m_needRespawnTime < m_respawnTime && player->GetPlaying())
			{
				// �܂��o���o������o������
				mp_enemy[i]->SetState(true);

				// �J�E���g���Ȃ���
				m_respawnTime = 0;

				// �ꏊ�ݒ�
				switch (player->GetAssaultPoint())
				{
				case 1: mp_enemy[i]->SetPos(Vector3(-35.0f, 5.0f, -25.5f + float(rand() % 20 - 10))); break;
				case 2: mp_enemy[i]->SetPos(Vector3(0.0f, 15.0f, 25.5f + float(rand() % 20 - 10))); break;
				case 3: mp_enemy[i]->SetPos(Vector3(0.0f + float(rand() % 20 - 10), 3.0f, 30.0f)); break;
				case 4: mp_enemy[i]->SetPos(Vector3(-35.0f + float(rand() % 20 - 10), 5.0f, -25.5f + float(rand() % 10 - 5))); break;
				}

				// �v���C���[�Ƃ̈ʒu�̍���
				SimpleMath::Vector3 enemyPosDiff = SimpleMath::Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x), float(player->GetPos().y - mp_enemy[i]->GetPos().y), float(player->GetPos().z - mp_enemy[i]->GetPos().z));
				// �G�̌���
				SimpleMath::Vector3 enemyDir = SimpleMath::Vector3(mp_enemy[i]->GetDir());

				{
					// �x�N�g���̒��������߂�
					double lengthA = pow((enemyPosDiff.x * enemyPosDiff.x) + (enemyPosDiff.z * enemyPosDiff.z), 0.5);
					double lengthB = pow((enemyDir.x * enemyDir.x) + (enemyDir.z * enemyDir.z), 0.5);
					// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
					double cos_sita = enemyPosDiff.x * enemyDir.x + enemyPosDiff.z * enemyDir.z / (lengthA * lengthB);

					// cos�Ƃ���Ƃ����߂�
					double sita = acos(cos_sita);
					// �f�O���[�ŋ��߂�
					//sita = sita * 180.0 / double(XM_PI);

					mp_enemy[i]->SetRotateY(float(sita));
				}

				//{
				//	// �x�N�g���̒��������߂�
				//	double lengthA = pow((enemyPosDiff.y * enemyPosDiff.y) + (enemyPosDiff.z * enemyPosDiff.z), 0.5);
				//	double lengthB = pow((enemyDir.y * enemyDir.y) + (enemyDir.z * enemyDir.z), 0.5);
				//	// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
				//	double cos_sita = enemyPosDiff.y * enemyDir.y + enemyPosDiff.z * enemyDir.z / (lengthA * lengthB);
				//	// cos�Ƃ���Ƃ����߂�
				//	double sita = acos(cos_sita);
				//	mp_enemy[i]->SetRotateX(float(sita));
				//}
			}
		}
	}

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// �G�̏P�����Ԃ�������X�V����
		if (!player->GetEnemyTime())break;
		// �G�̓���
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->SetVel(Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x) / 20.0f, 
										float(player->GetPos().y - mp_enemy[i]->GetPos().y) / 20.0f, 
										float(player->GetPos().z - mp_enemy[i]->GetPos().z) / 20.0f));
			mp_enemy[i]->Update(timer);

			// �}�b�v�̒��S����50�����ꂽ�������
			float dist = (mp_enemy[i]->GetPos().x - 0.0f)*(mp_enemy[i]->GetPos().x - 0.0f) +
						 (mp_enemy[i]->GetPos().y - 0.0f)*(mp_enemy[i]->GetPos().y - 0.0f) +
						 (mp_enemy[i]->GetPos().z - 0.0f)*(mp_enemy[i]->GetPos().z - 0.0f);
			if (m_maxAliveDist * m_maxAliveDist < dist)
			{
				mp_enemy[i]->SetState(false);
			}
		}
	}

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameEnemyManager::Render(MatrixManager* matrixManager)
{
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->Render(matrixManager);
		}
	}
}

/// <summary>
/// �p��
/// </summary>
void GameEnemyManager::Depose()
{
}
