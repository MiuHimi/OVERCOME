//////////////////////////////////////////////////////////////
// File.    ScenePlay.cpp
// Summary. ScenePlayClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "ScenePlay.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;

std::unique_ptr<Player> ScenePlay::mp_player;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
ScenePlay::ScenePlay(SceneManager* sceneManager)
	: SceneBase(sceneManager)
{
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
ScenePlay::ScenePlay(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
ScenePlay::~ScenePlay()
{
	/*delete mp_game;
	mp_game = nullptr;*/
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void ScenePlay::Initialize()
{
	m_toResultMoveOnChecker = false;

	// �X�v���C�g�t�H���g�̍쐬
	m_font = std::make_unique<SpriteFont>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"SegoeUI_18.spritefont");

	// �J�����I�u�W�F�N�g�̍쐬
	mp_camera = std::make_unique<MyCamera>();

	// �Q�[�����̐���
	mp_gameFloor = std::make_unique<GameFloor>(mp_game);
	// �Q�[�����̃��f���ǂݍ���
	mp_gameFloor->Create();

	// �Q�[�����H�̐���
	mp_gameRoad = std::make_unique<GameRoad>(mp_game);
	mp_gameRoad->Initialize();
	// �Q�[�����H�̃��f���ǂݍ���
	mp_gameRoad->Create(mp_game);

	// �v���C���[�̐���
	mp_player = std::make_unique<Player>(mp_game);
	mp_player->Initialize();
	// �v���C���[�̃��f���̓ǂݍ���
	mp_player->Create();

	// �X�J�C�h�[���̐���
	mp_skydome = std::make_unique<SkyDome>(mp_game);
	mp_skydome->Initialize();
	// �X�J�C�h�[���̃��f���̓ǂݍ���
	mp_skydome->Create();

	// �������Ԃ̐���
	mp_gameTimer = std::make_unique<GameTimer>();
	mp_gameTimer->Create();
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
void ScenePlay::Update(DX::StepTimer const& timer, Game* game)
{
	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();

	// ���ƃv���C���[�̏Փ˔���
	m_hitPlayerToFloorFlag = false;
	if (Collision::HitCheck_Box2Box(mp_gameFloor->GetCollision(), mp_player->GetCollision()) == true)
	{
		m_hitPlayerToFloorFlag = true;
		mp_player->SetJumpState(false);
		if (mp_player->GetPos().y < 0.0f)
		{
			mp_player->SetHeightPos(0.0f);
		}
	}

	// ���H�ƃv���C���[�̏Փ˔���
	m_hitPlayerToRoadFlag = false;
	mp_player->SetCollideState(false);
	for (int j = 0; j < mp_gameRoad->GetMaxFloorBlock(); j++)
	{
		for (int i = 0; i < mp_gameRoad->GetMaxFloorBlock(); i++)
		{
			if (mp_gameRoad->GetRoadObject(j, i).roadType == 1 || mp_gameRoad->GetRoadObject(j, i).roadType == 2 || mp_gameRoad->GetRoadObject(j, i).roadType == 3)
			{
				if (Collision::HitCheck_Box2Box(mp_gameRoad->GetCollisionObject(j, i)->GetCollision(), mp_player->GetCollision()) == true)
				{
					m_hitPlayerToRoadFlag = true;

					if (mp_player->GetPos().y <= 0.0f)
					{
						mp_player->SetHeightVel(0.0f);
					}
					if (mp_player->GetPos().y >= 0.5f && mp_player->GetPos().y <= 1.0f)
					{
						mp_player->SetJumpState(false);
						mp_player->SetHeightVel(1.0f);
					}
					mp_player->SetCollideState(true);

					if (i == 5 && j == 7)
					{
						m_toResultMoveOnChecker = true;
					}
				}
			}
		}
	}

	// �Q�[�����̍X�V
	mp_gameFloor->Update(timer);
	// �Q�[�����H�̍X�V
	mp_gameRoad->Update(timer);

	// �v���C���[�̍X�V
	mp_player->Update(timer);

	// �������Ԃ̍X�V
	mp_gameTimer->Update(timer);

	// �J�����̍X�V
	mp_camera->Update(timer, mp_player->GetPlayer());

	

	// �V�[������
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Z))
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
//void SceneLogo::Render()
//{
//}
void ScenePlay::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// �r���[�s��̍쐬
	DirectX::SimpleMath::Matrix m_view = DirectX::SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), DirectX::SimpleMath::Vector3::Up);

	// �Q�[�����̕`��
	mp_gameFloor->Render(game, m_view);
	// �Q�[�����H�̕`��
	mp_gameRoad->Render(m_view);

	// �X�J�C�h�[���̕`��
	mp_skydome->Render(m_view);

	// �v���C���[�̕`��
	mp_player->Render(m_view);
	//mp_player->DrawDebugCollision(m_view);

	// �������Ԃ̕`��
	mp_gameTimer->Render();

	// �f�o�b�O�p
	/*sprites->Begin();
	m_font->DrawString(sprites, L"ScenePlay", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"Zkey to SceneResult", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);

	if (m_hitPlayerToFloorFlag == true)
	{
		m_font->DrawString(sprites, L"FloorHIT", DirectX::SimpleMath::Vector2(20.0f, 50.0f));
	}
	if (m_hitPlayerToRoadFlag == true)
	{
		m_font->DrawString(sprites, L"RoadHIT", DirectX::SimpleMath::Vector2(20.0f, 70.0f));
	}
	sprites->End();*/
}
