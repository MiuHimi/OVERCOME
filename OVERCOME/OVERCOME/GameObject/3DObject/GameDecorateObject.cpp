//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.cpp
// Summary. GameDecorateObjectClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "GameDecorateObject.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;

// static�f�B���N�g��
const int GameDecorateObject::SHAKE_COUNT = 5;
const SimpleMath::Vector3 GameDecorateObject::SHAKE_VEL = SimpleMath::Vector3(0.2f, 0.0f, 0.0f);


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameDecorateObject::GameDecorateObject()
	: m_world(SimpleMath::Matrix::Identity),
	  m_doorRota{ SimpleMath::Matrix::Identity }, m_doorTrans{ SimpleMath::Vector3::Zero }, m_isMoveDoor{ false }, m_isShowDoor{ false },
	  m_chestRota{ SimpleMath::Matrix::Identity }, m_chestTrans{ SimpleMath::Vector3::Zero }, m_chestTempPos{ SimpleMath::Vector3::Zero }, 
	  m_shakeCount{ 0 }, m_shakeVelCount{ 0 }, m_shakeNeedCount{ 0 }, m_isChestOpen{ false },
	  mp_modelEnemyDoor {nullptr}, mp_modelSmallRoom(nullptr),
	  mp_modelEnemyChestOpen{nullptr}, mp_modelEnemyChestClose{ nullptr }
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameDecorateObject::~GameDecorateObject()
{
}

/// <summary>
/// ����
/// </summary>
void GameDecorateObject::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");

	for (int i = 0; i < MAX_DOOR_NUM; i++)
	{
		// �G�o���h�A���f���̓ǂݍ���
		mp_modelEnemyDoor[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\wood_door.cmo", fx);
		// ���f���Ƀt�H�O��������
		mp_modelEnemyDoor[i]->UpdateEffects([&](IEffect* effect)
		{
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(4.0f);
				fog->SetFogEnd(10.0f);
				fog->SetFogColor(Colors::Black);
			}
		});
	}

	// �������f���̓ǂݍ���
	mp_modelSmallRoom = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\small_room.cmo", fx);
	// ���f���Ƀt�H�O��������
	mp_modelSmallRoom->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(15.0f);
			fog->SetFogEnd(20.0f);
			fog->SetFogColor(Colors::Black);
		}
	});

	for (int i = 0; i < 3; i++)
	{
		// �`�F�X�g���f���̓ǂݍ���
		mp_modelEnemyChestOpen[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost_chest_open.cmo", fx);
		mp_modelEnemyChestClose[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost_chest_close.cmo", fx);

		// ���f���Ƀt�H�O��������
		mp_modelEnemyChestOpen[i]->UpdateEffects([&](IEffect* effect)
		{
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(15.0f);
				fog->SetFogEnd(20.0f);
				fog->SetFogColor(Colors::Black);
			}
		});
		// ���f���Ƀt�H�O��������
		mp_modelEnemyChestClose[i]->UpdateEffects([&](IEffect* effect)
		{
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(15.0f);
				fog->SetFogEnd(20.0f);
				fog->SetFogColor(Colors::Black);
			}
		});
	}
}
/// <summary>
/// ������
/// </summary>
void GameDecorateObject::Initialize()
{
	for (int i = 0; i < MAX_DOOR_NUM; i++)
	{
		m_doorTrans[0] = SimpleMath::Vector3(25.0f, 0.0f, -27.0f);
		m_doorTrans[1] = SimpleMath::Vector3(0.0f, 0.0f, -28.0f);
		m_doorTrans[2] = SimpleMath::Vector3(-3.0f, 0.0f, -23.0f);
		m_doorTrans[3] = SimpleMath::Vector3(7.0f, 0.0f, 25.0f);
		m_doorTrans[4] = SimpleMath::Vector3(-13.0f, 0.0f, 23.0f);
		m_doorTrans[5] = SimpleMath::Vector3(-14.0f, 0.0f, 23.0f);

		m_isShowDoor[i] = true;
	}

	for (int i = 0; i < 3; i++)
	{
		m_chestTrans[0] = SimpleMath::Vector3(-1.5f, 2.0f, -37.5f);
		m_chestTrans[1] = SimpleMath::Vector3(-16.0f, 2.0f, -27.5f);
		m_chestTrans[2] = SimpleMath::Vector3(-9.0f, 2.0f, -37.5f);
		m_chestRota[0] = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(0.0f));
		m_chestRota[1] = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
		m_chestRota[2] = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(45.0f));
		m_chestTempPos[0] = m_chestTrans[0];
		m_chestTempPos[1] = m_chestTrans[1];
		m_chestTempPos[2] = m_chestTrans[2];

		m_shakeNeedCount[0] = 100;
		m_shakeNeedCount[1] = 140;
		m_shakeNeedCount[2] = 190;
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>�I�����</returns>
bool GameDecorateObject::Update(int roadID)
{
	// �G�o���h�A�̍X�V
	UpdateEnemyDoor(roadID);

	// �G�o���`�F�X�g�̍X�V
	UpdateEnemyChest(roadID);

	/*if (roadID != 32) return false;*/

	// �`�F�X�g�̍X�V
	/*for (int i = 0; i < 3; i++)
	{
		// �U������J�E���g
		m_shakeCount[i]++;

		if (m_shakeCount[i] / m_shakeNeedCount[i] >= 1)
		{
			// �U�������p�J�E���g
			m_shakeVelCount[i]++;

			// �ϐ�������
			SimpleMath::Quaternion rotation =
				SimpleMath::Quaternion::Identity;
			SimpleMath::Vector3 vect = SimpleMath::Vector3::Zero;

			switch (i)
			{
			case 0:
				// �U�����邽�߂ɕK�v�Ȋp�x�̌v�Z
				rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), 0.0f);
				// �U������������Z�o
				vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
				break;
			case 1:
				// �U�����邽�߂ɕK�v�Ȋp�x�̌v�Z
				rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(90.0f));
				// �U������������Z�o
				vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
				break;
			case 2:
				// �U�����邽�߂ɕK�v�Ȋp�x�̌v�Z
				rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(45.0f));
				// �U������������Z�o
				vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
				break;
			}

			if (m_shakeVelCount[i] / SHAKE_COUNT == 0 || m_shakeVelCount[i] / SHAKE_COUNT == 2)
			{
				// �������ĉE���ɐU��
				m_chestTrans[i] = m_chestTempPos[i] + vect;
			}
			else if (m_shakeVelCount[i] / SHAKE_COUNT == 1 || m_shakeVelCount[i] / SHAKE_COUNT == 3)
			{
				// �������č����ɐU��
				m_chestTrans[i] = m_chestTempPos[i] + -vect;
			}
			else if (m_shakeVelCount[i] / SHAKE_COUNT >= 4)
			{
				if (m_shakeVelCount[i] / SHAKE_COUNT >= 8)
				{
					// ���I�[�v��
					m_isChestOpen[i] = true;
				}
				if (m_shakeVelCount[i] / SHAKE_COUNT >= 12)
				{
					// �J�E���g���Z�b�g
					m_shakeCount[i] = 0;
					m_shakeVelCount[i] = 0;
					// ���W�����̈ʒu��
					m_chestTrans[i] = m_chestTempPos[i];
					// ���J���܂ł̎��Ԃ��v�Z
					m_shakeNeedCount[i] = 60 + (int)(rand()*(100 - 60 + 1.0) / (1.0 + RAND_MAX));

					// ���N���[�Y
					m_isChestOpen[i] = false;
				}
			}
		}
	}*/

	return true;
}

/// <summary>
/// �`��
/// </summary>
void GameDecorateObject::Render(MatrixManager * matrixManager)
{
	// �h�A�̕`��
	for (int i = 0; i < MAX_DOOR_NUM; i++)
	{
		switch (i)
		{
		case 0:
			m_world = SetWorldMatrix(m_world, 
				SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(m_doorTrans[0]));
			break;
		case 1:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(m_doorTrans[1]));
			break;
		case 2:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f)), SimpleMath::Matrix::CreateTranslation(m_doorTrans[2]));
			break;
		case 3:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f)), SimpleMath::Matrix::CreateTranslation(m_doorTrans[3]));
			break;
		case 4:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(m_doorTrans[4]));
			break;
		case 5:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f)), SimpleMath::Matrix::CreateTranslation(m_doorTrans[5]));
			break;
		default:
			break;
		}

		// �\���ł����ԂȂ�
		if (m_isShowDoor[i])
		{
			// �h�A�̕`��
			mp_modelEnemyDoor[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
				m_world, matrixManager->GetView(), matrixManager->GetProjection());
		}
	}

	// �������̕`��
	m_world = SetWorldMatrix(m_world,
		SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-8.5f, 0.0f, -32.0f)));
	mp_modelSmallRoom->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		m_world, matrixManager->GetView(), matrixManager->GetProjection());

	// �`�F�X�g�̕`��
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			m_world = SetWorldMatrix(m_world,
				m_chestRota[0], SimpleMath::Matrix::CreateTranslation(m_chestTrans[0]));
			break;
		case 1:
			m_world = SetWorldMatrix(m_world,
				m_chestRota[1], SimpleMath::Matrix::CreateTranslation(m_chestTrans[1]));
			break;
		case 2:
			m_world = SetWorldMatrix(m_world,
				m_chestRota[2], SimpleMath::Matrix::CreateTranslation(m_chestTrans[2]));
			break;
		default:
			break;
		}

		// �`�F�X�g�̕`��
		if(m_isChestOpen[i])
		mp_modelEnemyChestOpen[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
			m_world, matrixManager->GetView(), matrixManager->GetProjection());
		else
			mp_modelEnemyChestClose[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
				m_world, matrixManager->GetView(), matrixManager->GetProjection());
	}
}

/// <summary>
/// �p������
/// </summary>
void GameDecorateObject::Depose()
{
}

/// <summary>
/// �`��O�ɂ��郏�[���h�s��̐ݒ�
/// </summary>
/// <param name="rota">��]�s��</param>
/// <param name="trans">�ړ��s��</param>
SimpleMath::Matrix GameDecorateObject::SetWorldMatrix(SimpleMath::Matrix& world, SimpleMath::Matrix rota, SimpleMath::Matrix trans)
{
	world = SimpleMath::Matrix::Identity;
	world *= rota;
	world *= trans;

	return world;
}

/// <summary>
/// �G�o���h�A�̍X�V
/// </summary>
/// <param name="roadID">��ID</param>
void GameDecorateObject::UpdateEnemyDoor(int roadID)
{
	if (roadID == 31 && m_doorTrans[0].x < 35.0f)
	{
		// SE���Ȃ��ĂȂ�������
		if (!m_isMoveDoor[0])
		{
			// �h�A���J��SE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[0] = true;
		}
		// �h�A�𓮂���
		m_doorTrans[0].x += 0.1f;
	}
	else
		m_isShowDoor[0] = true;
	if (roadID == 32 && m_doorTrans[1].x < 5.0f)
	{
		// SE���Ȃ��ĂȂ�������
		if (!m_isMoveDoor[1])
		{
			// �h�A���J��SE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[1] = true;
		}
		// �h�A�𓮂���
		m_doorTrans[1].x += 0.1f;
	}
	else
		m_isShowDoor[1] = true;
	if (roadID == 32 && m_doorTrans[2].z < -15.0f)
	{
		// SE���Ȃ��ĂȂ�������
		if (!m_isMoveDoor[2])
		{
			// �h�A���J��SE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[2] = true;
		}
		// �h�A�𓮂���
		m_doorTrans[2].z += 0.1f;
	}
	else
		m_isShowDoor[2] = true;
	if (roadID == 33 && m_doorTrans[3].z < 35.0f)
	{
		// SE���Ȃ��ĂȂ�������
		if (!m_isMoveDoor[3])
		{
			// �h�A���J��SE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[3] = true;
		}
		// �h�A�𓮂���
		m_doorTrans[3].z += 0.1f;
	}
	else
		m_isShowDoor[3] = true;
	if (roadID == 34 && m_doorTrans[4].x < -6.0f)
	{
		// SE���Ȃ��ĂȂ�������
		if (!m_isMoveDoor[4])
		{
			// �h�A���J��SE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[4] = true;
		}
		// �h�A�𓮂���
		m_doorTrans[4].x += 0.1f;
	}
	else
		m_isShowDoor[4] = true;
	if (roadID == 34 && m_doorTrans[5].x > -25.0f)
	{
		// SE���Ȃ��ĂȂ�������
		if (!m_isMoveDoor[5])
		{
			// �h�A���J��SE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[5] = true;
		}
		// �h�A�𓮂���
		m_doorTrans[5].x -= 0.1f;
		m_isShowDoor[5] = true;
	}
	else if (roadID != 34 && m_doorTrans[5].x > -25.0f)
	{
		m_isShowDoor[5] = true;
	}
	else
		m_isShowDoor[5] = false;
}

/// <summary>
/// �G�o���`�F�X�g�̍X�V
/// </summary>
/// <param name="roadID">��ID</param>
void GameDecorateObject::UpdateEnemyChest(int roadID)
{
	if (roadID != 32) return;

	// �`�F�X�g�̍X�V
	for (int i = 0; i < 3; i++)
	{
		// �U������J�E���g
		m_shakeCount[i]++;

		// �܂��U�����J�n���Ă͂����Ȃ��J�E���g��������
		if (m_shakeCount[i] / m_shakeNeedCount[i] < 1)
		{
			continue;
		}

		// �U�������p�J�E���g
		m_shakeVelCount[i]++;

		// �ϐ�������
		SimpleMath::Quaternion rotation = SimpleMath::Quaternion::Identity;
		SimpleMath::Vector3 vect = SimpleMath::Vector3::Zero;

		// �U���p�̒l�̌v�Z
		switch (i)
		{
		case 0:
			// �U�����邽�߂ɕK�v�Ȋp�x�̌v�Z
			rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), 0.0f);
			// �U������������Z�o
			vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
			break;
		case 1:
			// �U�����邽�߂ɕK�v�Ȋp�x�̌v�Z
			rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(90.0f));
			// �U������������Z�o
			vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
			break;
		case 2:
			// �U�����邽�߂ɕK�v�Ȋp�x�̌v�Z
			rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(45.0f));
			// �U������������Z�o
			vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
			break;
		}

		// �U���֌W�̏���
		if (m_shakeVelCount[i] / SHAKE_COUNT == 0 || m_shakeVelCount[i] / SHAKE_COUNT == 2)
		{
			// �������ĉE���ɐU��
			m_chestTrans[i] = m_chestTempPos[i] + vect;
		}
		else if (m_shakeVelCount[i] / SHAKE_COUNT == 1 || m_shakeVelCount[i] / SHAKE_COUNT == 3)
		{
			// �������č����ɐU��
			m_chestTrans[i] = m_chestTempPos[i] + -vect;
		}
		else
		{
			// ���W�����̈ʒu��
			m_chestTrans[i] = m_chestTempPos[i];
		}

		// ���J�֌W�̏���
		if (m_shakeVelCount[i] / SHAKE_COUNT >= 8 && m_shakeVelCount[i] / SHAKE_COUNT < 12)
		{
			// ���I�[�v��
			m_isChestOpen[i] = true;
		}
		else if (m_shakeVelCount[i] / SHAKE_COUNT >= 12)
		{
			// �J�E���g���Z�b�g
			m_shakeCount[i] = 0;
			m_shakeVelCount[i] = 0;
			// ���J���܂ł̎��Ԃ��v�Z
			m_shakeNeedCount[i] = 60 + rand()*(90 - 60 + 1) / (1 + RAND_MAX);

			// ���N���[�Y
			m_isChestOpen[i] = false;
		}
	}
}
