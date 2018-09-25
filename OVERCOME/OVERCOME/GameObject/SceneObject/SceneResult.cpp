//////////////////////////////////////////////////////////////
// File.    SceneResult.cpp
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneResult.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneResult::SceneResult(SceneManager* sceneManager)
	: SceneBase(sceneManager)
{
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneResult::SceneResult(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneResult::~SceneResult()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneResult::Initialize()
{
	m_toTitleMoveOnChecker = false;

	// �X�v���C�g�t�H���g�̍쐬
	m_font = std::make_unique<SpriteFont>(/*device*/mp_game->GetDevice(), L"SegoeUI_18.spritefont");
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneResult::Finalize()
{
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneResult::Update(DX::StepTimer const& timer, Game* game)
{
	// ���͏����X�V
	InputManager::GetInstance().Update();
	// �L�[����
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toTitleMoveOnChecker = true;
	}
	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
//void SceneLogo::Render()
//{
//}
void SceneResult::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// �f�o�b�O�p
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneResult", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"SPACEkey to SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);
	sprites->End();
}
