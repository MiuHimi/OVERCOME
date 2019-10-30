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


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameDecorateObject::GameDecorateObject()
	: m_world(SimpleMath::Matrix::Identity),
	  m_doorRota{ SimpleMath::Matrix::Identity }, m_doorTrans{ SimpleMath::Vector3::Zero }, m_isMoveDoor{ false }, m_isShowDoor{ false },
	  mp_modelEnemyDoor {nullptr}, mp_modelSmallRoom(nullptr)
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

	mp_modelSmallRoom = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\small_room.cmo", fx);

	// ���f���Ƀt�H�O��������
	mp_modelSmallRoom->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(8.0f);
			fog->SetFogEnd(15.0f);
			fog->SetFogColor(Colors::Black);
		}
	});
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
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>�I�����</returns>
bool GameDecorateObject::Update(int roadID)
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
	else if(roadID != 34 && m_doorTrans[5].x > -25.0f)
	{
		m_isShowDoor[5] = true;
	}
	else
		m_isShowDoor[5] = false;

	return false;
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
