//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneTitle.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneTitle::SceneTitle(SceneManager* sceneManager) 
	                 : SceneBase(sceneManager)
{
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneTitle::SceneTitle(Game * game, SceneManager * sceneManager)
	: mp_game(game)
	, SceneBase(sceneManager)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneTitle::~SceneTitle()
{
}

/// <summary>
/// �^�C�g���V�[���̏���������
/// </summary>
void SceneTitle::Initialize()
{
	m_toPlayMoveOnChecker = false;

	// �X�v���C�g�t�H���g�̍쐬
	m_font = std::make_unique<SpriteFont>(/*device*/mp_game->GetDevice(), L"SegoeUI_18.spritefont");
}
/// <summary>
/// �^�C�g���V�[���̏I������
/// </summary>
void SceneTitle::Finalize()
{
}

/// <summary>
/// �^�C�g���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneTitle::Update(DX::StepTimer const& timer, Game* game)
{
	// ���͏����X�V
	InputManager::GetInstance().Update();
	// �L�[����
	//if (InputManager::GetInstance().GetTracker().leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Z))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}

}

/// <summary>
/// �^�C�g���V�[���̕`�揈��
/// </summary>
//void SceneTitle::Render()
//{
//	/*DebugText* debugText = DebugText::GetInstance();
//	debugText->AddText(Vector2(10, 10), L"SceneTitle");
//	debugText->AddText(Vector2(10, 30), L"SPACEkey to SceneLogo");*/
//}
void SceneTitle::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// �f�o�b�O�p
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneTitle", DirectX::SimpleMath::Vector2(20.0f, 10.0f), Colors::Yellow);
	m_font->DrawString(sprites, L"SPACEkey to ScenePlay", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);
	sprites->End();
}
