//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneTitle.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneTitle::SceneTitle(SceneManager * sceneManager)
	: SceneBase(sceneManager),
      m_colorAlpha(0.0f),
	  mp_textureBackground(nullptr),
	  mp_textureTitle(nullptr),
	  mp_sprite(nullptr),
	  m_titleWidth(0.0f),
	  m_titleHeight(0.0f),
	  m_TitlePos(0.0f, 0.0f),
	  mp_matrixManager(nullptr)
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

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\title_background.png", nullptr, mp_textureBackground.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\title.png", nullptr, mp_textureTitle.GetAddressOf());

	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// �^�C�g���̕��A�����A�ʒu�ݒ�
	m_titleWidth = 500.0f;
	m_titleHeight = 120.0f;
	m_TitlePos = SimpleMath::Vector2(150.0f, 100.0f);

	// ���l�̐ݒ�(������)
	m_colorAlpha = 1.0f;

	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();
	
	// �r���[�s��̍쐬
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

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

	// �T�E���h�Đ�
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"SceneTitle.acb", L"SceneTitle.awb");
	adx2le->Play(0);
}
/// <summary>
/// �^�C�g���V�[���̏I������
/// </summary>
void SceneTitle::Finalize()
{
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}

	// �T�E���h�̒�~
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Stop();
}

/// <summary>
/// �^�C�g���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneTitle::Update(DX::StepTimer const& timer)
{
	// �T�E���h�̍X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();
	
	// �}�E�X�̍X�V
	//	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());
	InputManager::SingletonGetInstance().Update();

	// �E�N���b�N�ŃV�[���J�ڊJ�n
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		m_toPlayMoveOnChecker = true;
		adx2le->Play(1);
	}
	if (m_toPlayMoveOnChecker)
	{
		m_colorAlpha -= 0.01f;
		if (m_colorAlpha < 0.0f) m_colorAlpha = 0.0f;
	}

	// �V�[���J��
	if (m_toPlayMoveOnChecker && m_colorAlpha <= 0.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_SELECTSTAGE);
	}

}

/// <summary>
/// �^�C�g���V�[���̕`�揈��
/// </summary>
void SceneTitle::Render()
{
	// �^�C�g���̕`��
	mp_sprite->Begin(DirectX::SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());
	
	RECT rectTiteBG = { 0, 0, 800, 600 };
	RECT rectTite = { 0, 0, int(m_titleWidth), int(m_titleHeight) };

	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectTiteBG, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	mp_sprite->Draw(mp_textureTitle.Get(), m_TitlePos, &rectTite, SimpleMath::Vector4( 1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	
	mp_sprite->End();
}
