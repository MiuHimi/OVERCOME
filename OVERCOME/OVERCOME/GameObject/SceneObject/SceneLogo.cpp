//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneLogo.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(SceneManager* sceneManager) 
	               : SceneBase(sceneManager)
{
}
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(Game * game, SceneManager * sceneManager)
	               : mp_game(game)
	               , SceneBase(sceneManager)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneLogo::~SceneLogo()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneLogo::Initialize()
{
	m_toTitleMoveOnChecker = false;

	m_count = 0;

	// �X�v���C�g�t�H���g�̍쐬
	m_font = std::make_unique<SpriteFont>(/*device*/mp_game->GetDevice(), L"SegoeUI_18.spritefont");
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneLogo::Finalize()
{
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneLogo::Update(DX::StepTimer const& timer, Game* game)
{
	m_count++;

	if (m_count == 120)
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
//	// �f�o�b�O�p
//	/*DebugText* debugText = DebugText::GetInstance();
//	debugText->AddText(Vector2(10, 10), L"SceneLogo");
//	debugText->AddText(Vector2(10, 30), L"Count = %3d", m_count);*/
//}
void SceneLogo::Render(DirectX::SpriteBatch* sprites, Game* game)
{
	// �f�o�b�O�p
	sprites->Begin();
	m_font->DrawString(sprites, L"SceneLogo", DirectX::SimpleMath::Vector2(20.0f, 30.0f), Colors::Yellow);
	sprites->End();
}
