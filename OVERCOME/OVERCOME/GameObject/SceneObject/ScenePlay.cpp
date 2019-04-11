//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "ScenePlay.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

std::unique_ptr<Player> ScenePlay::mp_player;
bool SceneManager::m_clearSceneState;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
ScenePlay::ScenePlay(SceneManager* sceneManager)
	: SceneBase(sceneManager),
   	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
ScenePlay::~ScenePlay()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void ScenePlay::Initialize()
{
	m_toResultMoveOnChecker = false;

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// �J�����I�u�W�F�N�g�̍쐬
	//mp_camera = std::make_unique<GameCamera>();
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// �Q�[�����̐���
	mp_gameFloor = std::make_unique<GameFloor>();
	// �Q�[�����̃��f���ǂݍ���
	mp_gameFloor->Create();

	// �Q�[�����H�̐���
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	// �Q�[�����H�̃��f���ǂݍ���
	mp_gameRoad->Create();

	// �Q�[�����H�̐���
	mp_gameTarget = std::make_unique<GameTarget>();
	mp_gameTarget->Initialize();
	// �Q�[�����H�̃��f���ǂݍ���
	mp_gameTarget->Create();

	// �Q�[�����H�̐���
	mp_gameMap = std::make_unique<GameMap>();
	mp_gameMap->Initialize();
	// �Q�[�����H�̃��f���ǂݍ���
	mp_gameMap->Create();

	// �Q�[���G�Ǘ��̐���
	mp_gameEnemyManager = std::make_unique<GameEnemyManager>();
	mp_gameEnemyManager->Initialize();
	// �Q�[���G�Ǘ��̃��f���ǂݍ���
	mp_gameEnemyManager->Create();

	// �v���C���[�̐���
	mp_player = std::make_unique<Player>();
	mp_player->Initialize();
	// �v���C���[�̃��f���̓ǂݍ���
	mp_player->Create();

	// �X�J�C�h�[���̐���
	//mp_skydome = std::make_unique<SkyDome>();
	//mp_skydome->Initialize();
	// �X�J�C�h�[���̃��f���̓ǂݍ���
	//mp_skydome->Create();

	// �X�R�A�̐���
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create();

	m_fadeInCount = 1;
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureFadeIn.GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureBackground.GetAddressOf());

	// ���b�V���Փ˔���
	m_box = std::make_unique<CollisionMesh>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\StageMap\\stage01.obj");


	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();

	// �r���[�s��̍쐬
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float fovAngleY = XMConvertToRadians(45.0f);

	// �ˉe�s����쐬
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// �s���ݒ�
	mp_matrixManager->SetViewProjection(view, projection);

	// �G�t�F�N�g�}�l�[�W���[�̏�����
	//mp_effectManager = nullptr;
	//mp_effectManager = new EffectManager();
	//mp_effectManager->Create();
	//mp_effectManager->Initialize(5, SimpleMath::Vector3(0, 0, 0), SimpleMath::Vector3(0, 0, 0));
	//mp_effectManager->SetRenderState(view, projection);


	// �T�E���h�Đ�
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"ScenePlay.acb", L"ScenePlay.awb");
	adx2le->Play(0);
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void ScenePlay::Finalize()
{
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}
	
	/*if (mp_effectManager != nullptr) {
		mp_effectManager->Lost();
		delete mp_effectManager;
		mp_effectManager = nullptr;
	}*/
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void ScenePlay::Update(DX::StepTimer const& timer)
{
	// �t�F�[�h�C��
	m_fadeInCount -= 0.01f;
	if (m_fadeInCount < 0.0f)m_fadeInCount = 0.0f;

	//mp_effectManager->Update(timer);

	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();

	// �J�����̍X�V
	mp_camera->Update(timer, mp_player->GetPlayer());

	// ���H�ƃv���C���[�̏Փ˔���
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			if (mp_gameRoad->GetRoadObject(j, i).roadType != 0)
			{
				if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
				{
					// �S�[���ɓ��B������
					if (SceneManager::GetStageNum() == 1 &&
							i == mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x &&
							j == mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y ||   // �X�e�[�W�P�̃S�[��
						SceneManager::GetStageNum() == 2 && i == 11 && j == 13 ||     // �X�e�[�W�Q�̃S�[��(����P)
						SceneManager::GetStageNum() == 2 && i == 13 && j == 15)       // �X�e�[�W�Q�̃S�[��(����Q)
					{
						m_toResultMoveOnChecker = true;
						SceneManager::SetResultSceneState(true);
						// �}�E�X�J�[�\���̕\��
						ShowCursor(TRUE);
					}
				}
			}
		}
	}

	// �I�ƒe�̏Փ˔���
	/*for (int j = 0; j < mp_gameTarget->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameTarget->GetMaxFloorBlock(); i++)
		{
			// �I�̏�Ԏ擾
			if (mp_gameTarget->GetState(j, i))
			{
				for (int k = 0; k < mp_player->GetBulletManager()->GetMaxBulletNum(); k++)
				{
					// �e�̏�Ԏ擾
					if (mp_player->GetBulletManager()->GetBulletState(k))
					{
						if (Collision::HitCheck_Sphere2Box(mp_player->GetBulletManager()->GetBulletCollide(k), mp_gameTarget->GetCollisionObject(j, i)->GetCollision()))
						{
							// �I�ƒe�̏��
							mp_gameTarget->SetState(j, i, false);
							mp_player->GetBulletManager()->SetBulletState(k, false);

							// ���_
							mp_gameScore->FluctuationScore(100);
						}
					}
				}
			}
		}
	}*/

	int id;
	SimpleMath::Vector3 s;
	SimpleMath::Vector3 playerPos = mp_player->GetPos();
	SimpleMath::Vector3 v[2] = { SimpleMath::Vector3(playerPos.x, 100.0f, playerPos.z), SimpleMath::Vector3(playerPos.x, -100.0f, playerPos.z) };
	// ���ƃv���C���[�̓����蔻����s��
	if (m_box->HitCheck_Segment(v[0], v[1], &id, &s) == true)
	{
		// �v���C���[�̈ʒu��ݒ肷��
		mp_player->SetHeightPos(s.y);
	}

	// �G�ƃv���C���[�̏Փ˔���
	for (int i = 0; i < mp_gameEnemyManager->GetMaxEnemyNum(); i++)
	{
		if (!mp_gameEnemyManager->GetEnemyState(i))continue;
		if (mp_gameEnemyManager->GetEnemyState(i))
		{
			if (Collision::HitCheck_Sphere2Box(mp_gameEnemyManager->GetEnemyCollide(i), mp_player->GetCollision()))
			{
				mp_gameEnemyManager->SetEnemyState(i, false);
			}
		}
	}

	// �G�ƒe�̏Փ˔���
	for (int i = 0; i < mp_gameEnemyManager->GetMaxEnemyNum(); i++)
	{
		if (!mp_gameEnemyManager->GetEnemyState(i))continue;
		if (mp_gameEnemyManager->GetEnemyState(i))
		{
			for (int j = 0; j < mp_player->GetBulletManager()->GetMaxBulletNum(); j++)
			{
				if (!mp_player->GetBulletManager()->GetBulletState(j))continue;
				if (mp_player->GetBulletManager()->GetBulletState(j))
				{
					if (Collision::HitCheck_Sphere2Sphere(mp_gameEnemyManager->GetEnemyCollide(i),
						mp_player->GetBulletManager()->GetBulletCollide(j)))
					{
						// 20�_�l��
						mp_gameScore->FluctuationScore(20);
						// �݂���state��false��
						mp_gameEnemyManager->SetEnemyState(i, false);
						mp_player->GetBulletManager()->SetBulletState(j, false);
					}
				}
			}
		}
	}

	// �e�̕\�����E�̐ݒ�
	SimpleMath::Vector3 pPos = mp_player->GetPos();
	SimpleMath::Vector3 bPos[5];
	for (int i = 0; i < mp_player->GetBulletManager()->GetMaxBulletNum(); i++)
	{
		// ���˂���Ă���e�̂݌v��
		if (!mp_player->GetBulletManager()->GetBulletState(i))
		{
			break;
		}

		// �������l(100)
		float length = 100.0f;
		float len = 0.0f;
		bPos[i] = mp_player->GetBulletManager()->GetPos(i);

		// �e�ƃv���C���[�̋������v��
		len = ((pPos.x - bPos[i].x)*(pPos.x - bPos[i].x)) +
			((pPos.y - bPos[i].y)*(pPos.y - bPos[i].y)) +
			((pPos.z - bPos[i].z)*(pPos.z - bPos[i].z));

		// ������100�𒴂�����e������
		if (length*length < len)
		{
			mp_player->GetBulletManager()->SetBulletState(i, false);
		}
	}

	// �Q�[�����̍X�V
	mp_gameFloor->Update(timer);
	// �Q�[�����H�̍X�V
	mp_gameRoad->Update(timer);
	// �Q�[���I�̍X�V
	//mp_gameTarget->Update(timer);

	// �v���C���[�̍X�V
	mp_player->Update(timer);

	// �G�̍X�V
	mp_gameEnemyManager->Update(timer, mp_player->GetPlayer());

	// �X�R�A�̍X�V
	mp_gameScore->Update(timer);
	if (mp_gameScore->GetScore() == 0)
	{
		// �X�R�A��0��������Q�[���I�[�o�[
		SceneManager::SetResultSceneState(false);
	}

	// �V�[������
	if (m_toResultMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_RESULT);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void ScenePlay::Render()
{
	// �r���[�s��̍쐬
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), DirectX::SimpleMath::Vector3::Up);
	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float fovAngleY = XMConvertToRadians(45.0f);

	// �ˉe�s����쐬
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// �s���ݒ�
	mp_matrixManager->SetViewProjection(view, projection);

	// �Q�[�����̕`��
	mp_gameFloor->Render(mp_matrixManager);
	// �Q�[�����H�̕`��
	//mp_gameRoad->Render(mp_matrixManager);
	// �Q�[���I�̕`��
	//mp_gameTarget->Render(mp_matrixManager);

	// �}�b�v�̕`��
	mp_gameMap->Render(mp_matrixManager);

	// �v���C���[�̕`��
	mp_player->Render(mp_matrixManager);

	// �G�̕`��
	mp_gameEnemyManager->Render(mp_matrixManager);

	// �X�J�C�h�[���̕`��
	//mp_skydome->Render(mp_matrixManager);
	
	//mp_effectManager->Render();

	// �X�R�A�̕`��
	mp_gameScore->Render();

	DrawManager::SingletonGetInstance().DrawAlpha(m_textureBackground.Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0, 1.0f, 1.0f, m_fadeInCount));
}
