//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2019/06/24
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "ScenePlay.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/MatrixManager.h"


#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;

bool SceneManager::m_clearSceneState;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
ScenePlay::ScenePlay(SceneManager* sceneManager)
	: SceneBase(sceneManager),
	  m_toResultMoveOnChecker(false),
	  m_returnToTitleChecker(false),
	  m_colorAlpha(0.0f),
	  mp_fade(nullptr),
	  mp_sprite(nullptr),
	  mp_camera(nullptr),
	  mp_player(nullptr),
	  mp_gameEnemy(nullptr), mp_gameEnemyManager(nullptr),
	  mp_gameRoad(nullptr), mp_gameMap(nullptr),
	  mp_gameScore(nullptr),
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

	// �v���C���[�̐���
	mp_player = std::make_unique<Player>();
	mp_player->Initialize();
	// �v���C���[�̃��f���̓ǂݍ���
	mp_player->Create();

	// �Q�[���G�Ǘ��̐���
	mp_gameEnemyManager = std::make_unique<GameEnemyManager>();
	mp_gameEnemyManager->Initialize();
	// �Q�[���G�Ǘ��̃��f���ǂݍ���
	mp_gameEnemyManager->Create();

	// �Q�[�����H�̐���
	mp_gameRoad = std::make_unique<GameRoad>();
	mp_gameRoad->Initialize();
	// �Q�[�����H�̃��f���ǂݍ���
	mp_gameRoad->Create();

	// �Q�[���}�b�v�̐���
	mp_gameMap = std::make_unique<GameMap>();
	mp_gameMap->Initialize();
	// �Q�[���}�b�v�̃��f���ǂݍ���
	mp_gameMap->Create();

	// �Q�[�����H�̐���
	//mp_gameTarget = std::make_unique<GameTarget>();
	//mp_gameTarget->Initialize();
	// �Q�[�����H�̃��f���ǂݍ���
	//mp_gameTarget->Create();


	// �X�R�A�̐���
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create(L"Resources\\Images\\GameScore\\score_len.png", L"Resources\\Images\\GameScore\\score_background.png");

	m_colorAlpha = 1.0f;
	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\black.png", nullptr, mp_fade.GetAddressOf());
	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();

	// �r���[�s��̍쐬
	SimpleMath::Matrix view = SimpleMath::Matrix::Identity;

	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float angle = 45.0f;
	float fovAngleY = XMConvertToRadians(angle);

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
	// �s��Ǘ��ϐ��̍폜
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
	// �V�[���J�ڒ���̃t�F�[�h�C��(�őO��)
	m_colorAlpha -= 0.01f;
	if (m_colorAlpha < 0.0f)m_colorAlpha = 0.0f;

	//mp_effectManager->Update(timer);

	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();

	// �J�����̍X�V
	mp_camera->Update(timer, mp_player->GetPos(), mp_player->GetHeight(), mp_player->GetDir());

	// ���H�ƃv���C���[�̏Փ˔���
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			//if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollision(j, i)->GetCollision(), mp_player->GetCollision()) == true)
			//{
			//	if (i != mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x || j != mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y)break;

			//	// �S�[���ɓ��B������
			//	if (i == mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x &&
			//		j == mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y)
			//	{
			//		m_toResultMoveOnChecker = true;
			//		SceneManager::SetResultSceneState(true);
			//		// �}�E�X�J�[�\���̕\��
			//		ShowCursor(TRUE);
			//	}
			//}

			SimpleMath::Vector3 roadpos = mp_gameRoad->GetRoadObject((int)mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).x, (int)mp_gameRoad->GetPosType(GameRoad::PosType::GOAL).y).pos;
			SimpleMath::Vector3 playerpos = mp_player->GetPos();
			float distX = roadpos.z - playerpos.x;
			float distY = roadpos.x - playerpos.z;
			float dist = distX*distX + distY*distY;

			float radius = 1.0f + 5.0f;
			float rad = radius * radius;

			// �S�[���ɓ��B������
			if (rad > dist)
			{
				m_toResultMoveOnChecker = true;
				SceneManager::SetResultSceneState(true);
				// �}�E�X�J�[�\���̕\��
				ShowCursor(TRUE);
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
						mp_gameEnemyManager->ShockEnemy(i);
						mp_player->GetBulletManager()->SetBulletState(j, false);
					}
				}
			}
		}
	}

	// �e�̕\�����E�̐ݒ�
	SimpleMath::Vector3 pPos = mp_player->GetPos();
	SimpleMath::Vector3 bPos[10];
	for (int i = 0; i < mp_player->GetBulletManager()->GetMaxBulletNum(); i++)
	{
		// ���˂���Ă���e�̂݌v��
		if (!mp_player->GetBulletManager()->GetBulletState(i))
		{
			break;
		}

		// �������l(100)
		float length = 50.0f;
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

	// ���I�u�W�F�N�g�̍X�V
	mp_gameRoad->Update(timer);
	// �}�b�v�̍X�V
	mp_gameMap->Update(timer, mp_player->GetPlayer());
	// �Q�[���I�̍X�V
	//mp_gameTarget->Update(timer);

	// �v���C���[�̍X�V
	mp_player->Update(timer);

	// �G�̍X�V
	mp_gameEnemyManager->Update(timer, mp_player->GetPos(), mp_gameRoad->GetRoadObject((int)mp_player->GetPassingRoad().y, (int)mp_player->GetPassingRoad().x).roadType, mp_gameRoad->GetRoadObject((int)mp_player->GetPassingRoad().y, (int)mp_player->GetPassingRoad().x).roadNum, mp_camera->GetCameraAngle());

	// �X�R�A�̍X�V
	mp_gameScore->Update(timer);
	if (mp_gameScore->GetScore() == 0)
	{
		// �X�R�A��0��������Q�[���I�[�o�[
		SceneManager::SetResultSceneState(false);
	}

	// �V�[������
	if (m_toResultMoveOnChecker)
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
	SimpleMath::Matrix view = SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), SimpleMath::Vector3::Up);
	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	float aspectRatio = float(size.right) / float(size.bottom);
	// ��p��ݒ�
	float angle = 45.0f;
	float fovAngleY = XMConvertToRadians(angle);

	// �ˉe�s����쐬
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// �s���ݒ�
	mp_matrixManager->SetViewProjection(view, projection);


	// ���I�u�W�F�N�g�̕`��
	mp_gameRoad->Render(mp_matrixManager);
	// �}�b�v�̕`��
	mp_gameMap->Render(mp_matrixManager);
	// �Q�[���I�̕`��
	//mp_gameTarget->Render(mp_matrixManager);

	// �v���C���[�̕`��
	mp_player->Render(mp_matrixManager);

	// �G�̕`��
	SimpleMath::Vector3 playerGlance = mp_player->GetPos();
	playerGlance.y = mp_player->GetHeight();
	mp_gameEnemyManager->Render(mp_matrixManager, playerGlance);
	
	//mp_effectManager->Render();

	// �X�R�A�̕`��
	mp_gameScore->Render();

	// �t�F�[�h�C���̕`��(�őO��)
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	RECT rect = { 0, 0, 800, 600 };
	mp_sprite->Draw(mp_fade.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	mp_sprite->End();
}
