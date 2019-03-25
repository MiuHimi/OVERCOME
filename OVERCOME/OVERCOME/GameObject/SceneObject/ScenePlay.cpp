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
#include "../../Utility/GameDebug.h"
#include "../../Utility/DrawManager.h"

#include "../ExclusiveGameObject/ADX2Le.h"

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

	// �������Ԃ̐���
	mp_gameTimer = std::make_unique<GameTimer>();
	mp_gameTimer->Create();

	// �X�R�A�̐���
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create();

	m_fadeInCount = 1;
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureFadeIn.GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureBackground.GetAddressOf());

	// ���b�V���Փ˔���
	m_box = std::make_unique<CollisionMesh>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"ExclusiveGameObject\\stage01.obj");


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
	mp_effectManager = nullptr;
	mp_effectManager = new EffectManager();
	mp_effectManager->Create();
	mp_effectManager->Initialize(5, SimpleMath::Vector3(0, 0, 0), SimpleMath::Vector3(0, 0, 0));
	mp_effectManager->SetRenderState(view, projection);


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
	
	if (mp_effectManager != nullptr) {
		mp_effectManager->Lost();
		delete mp_effectManager;
		mp_effectManager = nullptr;
	}
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

	// ���ƃv���C���[�̏Փ˔���
	//m_hitPlayerToFloorFlag = false;
	mp_player->SetFloorCollideState(false);
	if (Collision::HitCheck_Box2Box(mp_gameFloor->GetCollision(), mp_player->GetCollision()) == true)
	{
		// ���������珰�Ƃ̏Փ˃t���O�𗧂Ă�
		//m_hitPlayerToFloorFlag = true;

		// �W�����v���[�V�������I��������(���߂̃t���O�𗧂Ă�)
		mp_player->SetJumpState(false);
		// ���Ƃ̏Փ˃t���O�𗧂Ă�
		mp_player->SetFloorCollideState(true);
		// �I�u�W�F�N�g�ɐڐG�������߃t���O�𕚂���
		mp_player->SetNotTouchState(false);

		// �n�ʂɂ߂荞�܂Ȃ��悤�ɂ���
		if (mp_player->GetPos().y < 0.0f)
		{
			mp_player->SetHeightPos(0.0f);
		}
	}

	int id;
	SimpleMath::Vector3 s;
	SimpleMath::Vector3 playerPos = mp_player->GetPos();
	SimpleMath::Vector3 v[2] = { SimpleMath::Vector3(playerPos.x, 100.0f, playerPos.z), SimpleMath::Vector3(playerPos.x, -100.0f, playerPos.z) };
	// ���H�ƃv���C���[�̓����蔻����s��
	if (m_box->HitCheck_Segment(v[0], v[1], &id, &s) == true)
	{
		// �{�[���̈ʒu��ݒ肷��
		mp_player->SetHeightPos(s.y);
	}

	// ���H�ƃv���C���[�̏Փ˔���
	//m_hitPlayerToRoadFlag = false;
	mp_player->SetRoadCollideState(false);
	bool hitObject = false;
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			if (mp_gameRoad->GetRoadObject(j, i).roadType != 0)
			{
				if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
				{
					//m_hitPlayerToRoadFlag = true;

					// �W�����v���[�V�������I��������(���߂̃t���O�𗧂Ă�)
					mp_player->SetJumpState(false);
					// ���H�Ƃ̏Փ˃t���O�𗧂Ă�
					mp_player->SetRoadCollideState(true);
					// �I�u�W�F�N�g�ɐڐG�������߃t���O�𕚂���
					mp_player->SetNotTouchState(false);

					// �v���C���[�ɉ��_�̃`�����X�A��Ԏ��̂��߂ɓ��H���W���擾
					if (mp_gameScore->GetPointChance() == false && mp_gameRoad->GetRoadObject(j, i).roadType == 2)
					{
						mp_gameScore->SetAddPointChance(j, i);
						mp_gameScore->SetPointChance(true);
					}
					if (mp_gameScore->GetPointChance() == true && mp_gameRoad->GetRoadObject(j, i).roadType == 2)
					{
						mp_gameScore->AddPointChance(j, i);
					}
					if (mp_gameScore->GetPointChance() == true && mp_gameRoad->GetRoadObject(j, i).roadType != 2)
					{
						mp_gameScore->SetAddPointChance(0, 0);
						mp_gameScore->SetPointChance(false);
					}

					// ���H�ɂ߂荞�܂Ȃ��悤�ɂ���
					if (mp_player->GetPos().y > 0.5f && mp_player->GetPos().y <= 1.0f)
					{
						mp_player->SetHeightPos(1.0f);
					}

					hitObject = true;

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
			if (hitObject == true) break;
		}
		if (hitObject == true) break;
	}

	// �I�ƒe�̏Փ˔���
	for (int j = 0; j < mp_gameTarget->GetMaxFloorBlock(); j++)
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
	}

	// �e�̕\�����E�̐ݒ�
	SimpleMath::Vector3 pPos = mp_player->GetPos();
	SimpleMath::Vector3 bPos[5];
	for (int i = 0; i < mp_player->GetBulletManager()->GetMaxBulletNum(); i++)
	{
		// ���˂���Ă���e�̂݌v��
		if (!mp_player->GetBulletManager()->GetBulletState(i)) break;

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
	mp_gameTarget->Update(timer);

	// �v���C���[�̍X�V
	mp_player->Update(timer);

	// �������Ԃ̍X�V
	mp_gameTimer->Update(timer);
	if (mp_gameTimer->GetTimeUpFlag() == true)
	{
		// �������Ԃ�������Q�[���I�[�o�[
		m_toResultMoveOnChecker = true;
		SceneManager::SetResultSceneState(false);
	}

	// �X�R�A�̍X�V
	if (hitObject == false && mp_player->GetJumpState() == false)
	{
		// ���H�ɏ���ĂȂ�or�W�����v���Ă��Ȃ�������t���O�𗧂Ă�
		mp_gameScore->SetDeductFlag(true);
	}
	if (hitObject == true || mp_player->GetJumpState() == true)
	{
		// ���H�ɏ���Ă��邩�A�W�����v���Ă�����t���O�𕚂���
		mp_gameScore->SetDeductFlag(false);
	}
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
	// �X�v���C�g�̕`��
	/*m_sprites->Begin(DirectX::SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	m_sprites->Draw(m_textureBackground.Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f));
	
	m_sprites->End();*/

	//DrawManager::SingletonGetInstance().Draw(m_textureBackground.Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f));

	// �p�x�̃f�o�b�O
	//GameDebug::SingletonGetInstance().DebugRender(mp_camera->GetAngle(), SimpleMath::Vector2(10.0f, 30.0f));
	//GameDebug::SingletonGetInstance().DebugRender(mp_camera->GetAngleDbg(), SimpleMath::Vector2(10.0f, 50.0f));
	//GameDebug::SingletonGetInstance().Render();

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
	mp_gameTarget->Render(mp_matrixManager);

	// �}�b�v�̕`��
	mp_gameMap->Render(mp_matrixManager);

	// �X�J�C�h�[���̕`��
	//mp_skydome->Render(mp_matrixManager);

	//m_box->DrawCollision(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), mp_matrixManager->GetView(), mp_matrixManager->GetProjection());

	// �v���C���[�̕`��
	mp_player->Render(mp_matrixManager);
	//mp_player->DrawDebugCollision();

	// �������Ԃ̕`��
	//mp_gameTimer->Render();

	//mp_effectManager->Render();

	// �X�R�A�̕`��
	mp_gameScore->Render();


	DrawManager::SingletonGetInstance().DrawAlpha(m_textureBackground.Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0, 1.0f, 1.0f, m_fadeInCount));
}
