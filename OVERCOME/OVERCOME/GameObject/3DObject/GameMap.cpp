//////////////////////////////////////////////////////////////
// File.    GameRoad.cpp
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../SceneObject/SceneManager.h"
#include "GameMap.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameMap::GameMap()
	: m_modelMap(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameMap::~GameMap()
{
}

/// <summary>
/// ������
/// </summary>
void GameMap::Initialize()
{
}
/// <summary>
/// ����
/// </summary>
void GameMap::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelMap = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\wood_map01.cmo", fx);
}

/// <summary>
/// �X�V
/// </summary>
bool GameMap::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameMap::Render(MatrixManager * matrixManager)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	// �`��
	m_modelMap->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// �j��
/// </summary>
void GameMap::Depose()
{
}
