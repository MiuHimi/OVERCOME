//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "ScenePlay.h"

#include "../Utility/DeviceResources.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

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
	: SceneBase(sceneManager)
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

	// �v���C���[�̐���
	mp_player = std::make_unique<Player>();
	mp_player->Initialize();
	// �v���C���[�̃��f���̓ǂݍ���
	mp_player->Create();

	// �X�J�C�h�[���̐���
	mp_skydome = std::make_unique<SkyDome>();
	mp_skydome->Initialize();
	// �X�J�C�h�[���̃��f���̓ǂݍ���
	mp_skydome->Create();

	// �������Ԃ̐���
	mp_gameTimer = std::make_unique<GameTimer>();
	mp_gameTimer->Create();

	// �X�R�A�̐���
	mp_gameScore = std::make_unique<GameScore>();
	mp_gameScore->Create();
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void ScenePlay::Finalize()
{
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void ScenePlay::Update(DX::StepTimer const& timer)
{
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
					if (SceneManager::GetStageNum() == 1 && i == 5 && j == 7 ||       // �X�e�[�W�P�̃S�[��
						SceneManager::GetStageNum() == 2 && i == 11 && j == 13 ||     // �X�e�[�W�Q�̃S�[��(����P)
						SceneManager::GetStageNum() == 2 && i == 13 && j == 15)       // �X�e�[�W�Q�̃S�[��(����Q)
					{
						m_toResultMoveOnChecker = true;
						SceneManager::SetResultSceneState(true);
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
							mp_gameTarget->SetState(j, i, false);
							mp_player->GetBulletManager()->SetBulletState(k, false);
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
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toResultMoveOnChecker = true;
	}
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
	MatrixManager::SingletonGetInstance().SetViewProjection(view, projection);


	// �Q�[�����̕`��
	mp_gameFloor->Render();
	// �Q�[�����H�̕`��
	mp_gameRoad->Render();
	// �Q�[���I�̕`��
	mp_gameTarget->Render();

	// �X�J�C�h�[���̕`��
	mp_skydome->Render();

	// �v���C���[�̕`��
	mp_player->Render();
	//mp_player->DrawDebugCollision();

	// �������Ԃ̕`��
	mp_gameTimer->Render();

	// �X�R�A�̕`��
	mp_gameScore->Render();

	/*int a = 0;
	bool bState[5];
	std::vector<bool> tState;
	for (int j = 0; j < mp_gameTarget->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameTarget->GetMaxFloorBlock(); i++)
		{
			if (mp_gameTarget->GetTargetObject(j, i).height != 0.0) break;
			for (int k = 0; k < mp_player->GetBulletManager()->GetMaxBulletNum(); k++)
			{
				bState[k] = mp_player->GetBulletManager()->GetBulletState(k);

				tState.push_back(mp_gameTarget->GetState(j, i));
				GameDebug::SingletonGetInstance().DebugRender(float(tState.back()), DirectX::SimpleMath::Vector2(10.0, 100 + (a * 30)));
			}
			a++;
		}
	}
	for (int k = 0; k < mp_player->GetBulletManager()->GetMaxBulletNum(); k++)
	{
		GameDebug::SingletonGetInstance().DebugRender(float(bState[k]), DirectX::SimpleMath::Vector2(10.0, 50 + (k * 30)));
	}

	GameDebug::SingletonGetInstance().Render();*/

}
