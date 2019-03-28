//////////////////////////////////////////////////////////////
// File.    GameBullet.cpp
// Summary. GameBulletClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "GameBullet.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameBullet::GameBullet(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model)
	: m_pos(0.0f, 0.0f, 0.0f),
	  m_vel(0.0f, 0.0f, 0.0f),
	  m_state(false),
	  m_world(DirectX::SimpleMath::Matrix::Identity),
	  mp_modelBullet(nullptr)
{
	m_pos = pos;
	m_vel = vel;

	m_state = stateFlag;
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameBullet::~GameBullet()
{
}

void GameBullet::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�����쐬
	mp_modelBullet = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
	// �Փ˔���p���f���ݒ�
	Obj3D::SetModel(mp_modelBullet.get());

	// �t�H�O�̐ݒ�
	SetFogEffectDistance(10.0f, 30.0f);

	m_sphere.c = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_sphere.r = 0.0f;
	SetCollision(m_sphere);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameBullet::Update(DX::StepTimer const & timer)
{
	m_pos += m_vel;

	// ���[���h�s��̍쐬
	m_world = SimpleMath::Matrix::CreateTranslation(m_pos);

	m_sphere.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);      // ���E���̒��S
	m_sphere.r = 1.0f;                                                         // ���a
	SetCollision(m_sphere);

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameBullet::Render(MatrixManager* matrixManager)
{
	mp_modelBullet->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		                 m_world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// �t�H�O�̃X�^�[�g�ƃG���h��ݒ�
/// </summary>
/// <param name="start">���ʂ�������n�߂鋗��</param>
/// <param name="end">���ʂ����S�ɂ����鋗��</param>
void GameBullet::SetFogEffectDistance(float start, float end)
{
	mp_modelBullet->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(start); // assuming RH coordiantes
			fog->SetFogEnd(end);
			fog->SetFogColor(Colors::Black);
		}
	});
}
