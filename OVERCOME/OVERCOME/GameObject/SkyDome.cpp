//////////////////////////////////////////////////////////////
// File.    SkyDome.cpp
// Summary. SkyDomeClass
// Date.    2018/09/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "SkyDome.h"

// using�f�B���N�g��
using namespace DirectX;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
SkyDome::SkyDome(Game* game) : mp_game(game)
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
	EffectFactory fx(mp_game->GetDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelSky = Model::CreateFromCMO(mp_game->GetDevice(), L"Resources\\Models\\sky2.cmo", fx);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool SkyDome::Update(DX::StepTimer const & timer, Game* game)
{
	return true;
}
/// <summary>
/// �`�揈��
/// </summary>
void SkyDome::Render(DirectX::SimpleMath::Matrix view)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	// ���̕`��
	world = SimpleMath::Matrix::Identity;
	m_modelSky->Draw(mp_game->GetContext(), *mp_game->GetState(), world, view, mp_game->GetProjection());
}

/// <summary>
/// �폜����
/// </summary>
void SkyDome::Depose()
{
	// �Q�[���I�u�W�F�N�g���폜
	delete mp_game;
	mp_game = NULL;
}
