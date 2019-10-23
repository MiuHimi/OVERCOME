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

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameDecorateObject::GameDecorateObject()
	: m_trans{DirectX::SimpleMath::Vector3::Zero}
	, mp_modelEnemyDoor {nullptr}
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
	for (int i = 0; i < 6; i++)
	{
		mp_modelEnemyDoor[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\wood_door.cmo", fx);
	}
	for (int i = 0; i < 6; i++)
	{
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
	
}
/// <summary>
/// ������
/// </summary>
void GameDecorateObject::Initialize()
{
	for (int i = 0; i < 6; i++)
	{
		m_trans[0] = SimpleMath::Vector3(25.0f, 0.0f, -28.0f);
		m_trans[1] = SimpleMath::Vector3(0.0f, 0.0f, -28.0f);
		m_trans[2] = SimpleMath::Vector3(-5.0f, 0.0f, -23.0f);
		m_trans[3] = SimpleMath::Vector3(8.0f, 0.0f, 25.0f);
		m_trans[4] = SimpleMath::Vector3(-15.0f, 0.0f, 22.0f);
		m_trans[5] = SimpleMath::Vector3(-16.0f, 0.0f, 22.0f);
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <returns>�I�����</returns>
bool GameDecorateObject::Update(int in)
{
	if (in == 31 && m_trans[0].x < 35.0f)
	{
		m_trans[0].x += 0.1f;
	}
	if (in == 32 && m_trans[1].x < 5.0f)
	{
		m_trans[1].x += 0.1f;
	}
	if (in == 32 && m_trans[2].z < -15.0f)
	{
		m_trans[2].z += 0.1f;
	}
	if (in == 33 && m_trans[3].z < 35.0f)
	{
		m_trans[3].z += 0.1f;
	}
	if (in == 34 && m_trans[4].x < -6.0f)
	{
		m_trans[4].x += 0.1f;
	}
	if (in == 34 && m_trans[5].x > -25.0f)
	{
		m_trans[5].x -= 0.1f;
	}


	return false;
}

/// <summary>
/// �`��
/// </summary>
void GameDecorateObject::Render(MatrixManager * matrixManager)
{
	for (int i = 0; i < 6; i++)
	{
		if (i == 0)
		{
			m_world = SimpleMath::Matrix::Identity;
			SimpleMath::Matrix t = SimpleMath::Matrix::CreateTranslation(m_trans[0]);
			m_world *= t;
		}
		if (i == 1)
		{
			m_world = SimpleMath::Matrix::Identity;
			SimpleMath::Matrix t = SimpleMath::Matrix::CreateTranslation(m_trans[1]);
			m_world *= t;
		}
		if (i == 2)
		{
			m_world = SimpleMath::Matrix::Identity;
			SimpleMath::Matrix rota = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
			m_world *= rota;
			SimpleMath::Matrix t = SimpleMath::Matrix::CreateTranslation(m_trans[2]);
			m_world *= t;
		}
		if (i == 3)
		{
			m_world = SimpleMath::Matrix::Identity;
			SimpleMath::Matrix rota = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f));
			m_world *= rota;
			SimpleMath::Matrix t = SimpleMath::Matrix::CreateTranslation(m_trans[3]);
			m_world *= t;
		}
		if (i == 4)
		{
			m_world = SimpleMath::Matrix::Identity;
			SimpleMath::Matrix t = SimpleMath::Matrix::CreateTranslation(m_trans[4]);
			m_world *= t;
		}
		if (i == 5)
		{
			m_world = SimpleMath::Matrix::Identity;
			SimpleMath::Matrix t = SimpleMath::Matrix::CreateTranslation(m_trans[5]);
			m_world *= t;
		}
		
		if (i != 5)
		{
			mp_modelEnemyDoor[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
				m_world, matrixManager->GetView(), matrixManager->GetProjection());
		}
		else if(i == 5 && m_trans[5].x > -25.0f)
		{
			mp_modelEnemyDoor[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
				m_world, matrixManager->GetView(), matrixManager->GetProjection());
		}
		
	}
	
}

/// <summary>
/// �p������
/// </summary>
void GameDecorateObject::Depose()
{
}
