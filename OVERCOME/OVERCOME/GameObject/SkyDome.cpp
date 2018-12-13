//////////////////////////////////////////////////////////////
// File.    SkyDome.cpp
// Summary. SkyDomeClass
// Date.    2018/09/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "SkyDome.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
SkyDome::SkyDome()
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SkyDome::~SkyDome()
{
	//SkyDome::Depose();
}

/// <summary>
/// ����������
/// </summary>
void SkyDome::Initialize()
{
}
/// <summary>
/// ��������
/// </summary>
void SkyDome::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelSky = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\sky2.cmo", fx);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool SkyDome::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// �`�揈��
/// </summary>
void SkyDome::Render()
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	// ���̕`��
	world = SimpleMath::Matrix::Identity;
	/*m_modelSky->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		             world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());*/
}

/// <summary>
/// �폜����
/// </summary>
void SkyDome::Depose()
{
}
