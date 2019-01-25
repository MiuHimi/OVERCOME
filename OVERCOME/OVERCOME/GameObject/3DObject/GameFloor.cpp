//////////////////////////////////////////////////////////////
// File.    GameFloor.cpp
// Summary. GameFloorClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "GameFloor.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"
#include "../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
GameFloor::GameFloor()
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameFloor::~GameFloor()
{
	//GameFloor::Depose();
}

/// <summary>
/// ����������
/// </summary>
void GameFloor::Initialize()
{
}
/// <summary>
/// ��������
/// </summary>
void GameFloor::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelLattice = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\floor01.cmo", fx);

	// �t�H�O�̐ݒ�
	SetFogEffectDistance(10.0f, 30.0f);

	// �Փ˔���p�I�u�W�F�N�g����
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);      // ���E���̒��S
	box.r = DirectX::SimpleMath::Vector3(50.0f, 0.0f, 50.0f);    // �e���a0.5

	// ���̍쐬
	SetModel(m_modelLattice.get());
	SetCollision(box);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameFloor::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// �`�揈��
/// </summary>
void GameFloor::Render(MatrixManager* matrixManager)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	// ���̕`��
	world = SimpleMath::Matrix::Identity;
	m_modelLattice->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		                 world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// �폜����
/// </summary>
void GameFloor::Depose()
{
}

/// <summary>
/// �t�H�O�̃X�^�[�g�ƃG���h��ݒ�
/// </summary>
/// <param name="start">���ʂ�������n�߂鋗��</param>
/// <param name="end">���ʂ����S�ɂ����鋗��</param>
void GameFloor::SetFogEffectDistance(float start, float end)
{
	m_modelLattice->UpdateEffects([&](IEffect* effect)
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