//////////////////////////////////////////////////////////////
// File.    GameRoad.cpp
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <fstream>
#include <sstream>

#include "../../pch.h"
#include "../SceneObject/SceneManager.h"
#include "GameMap.h"
#include "../3DObject/Player.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

//int SceneManager::m_stageID;

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameMap::GameMap()
	: mp_modelMap(nullptr),
	  mp_collisionStage(nullptr),
	  m_correctPos(SimpleMath::Vector3::Zero)
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

	// �t�@�C���p�X
	std::wstring filePath;
	// �X�e�[�W�ԍ�
	int fileNumber = /*SceneManager::GetStageNum()*/1;

	// �������A�������f���̃p�X�𐶐�
	std::wstringstream ssModel;
	ssModel << "Resources\\Models\\stage0" << fileNumber << ".cmo";
	filePath = ssModel.str();
	// ���f�������[�h���ă��f���n���h�����擾����
	mp_modelMap = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), filePath.c_str(), fx);

	// �������A�����Փ˔��胂�f���̃p�X�𐶐�
	std::wstringstream ssCollision;
	ssCollision << "Resources\\StageMap\\stage0" << fileNumber << ".obj";
	filePath = ssCollision.str();
	// ���b�V���Փ˔���
	mp_collisionStage = std::make_unique<CollisionMesh>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), filePath.c_str());

	mp_modelMap->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(8.0f);
			fog->SetFogEnd(10.0f);
			fog->SetFogColor(Colors::Black);
		}
	});
}

/// <summary>
/// �X�V
/// </summary>
bool GameMap::Update(DX::StepTimer const & timer, Player *player)
{
	int id;
	SimpleMath::Vector3 s;
	SimpleMath::Vector3 playerPos = player->GetPos();
	SimpleMath::Vector3 v[2] = { SimpleMath::Vector3(playerPos.x, 100.0f, playerPos.z), SimpleMath::Vector3(playerPos.x, -100.0f, playerPos.z) };
	// ���ƃv���C���[�̓����蔻����s��
	if (mp_collisionStage->HitCheck_Segment(v[0], v[1], &id, &s) == true)
	{
		// �v���C���[�̈ʒu��ݒ肷��
		//player->SetHeightPos(s.y);
		m_correctPos = s;
	}

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameMap::Render(MatrixManager * matrixManager)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	// �`��
	mp_modelMap->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// �j��
/// </summary>
void GameMap::Depose()
{
}
