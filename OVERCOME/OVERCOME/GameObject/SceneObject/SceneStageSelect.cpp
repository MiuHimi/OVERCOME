//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int SceneStageSelect::STAGE_ICON_SIZE = 80;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager, bool isFullScreen)
	: SceneBase(sceneManager, isFullScreen),
	  m_toPlayMoveOnChecker(false),
	  m_selectedStage(0),
	  m_colorAlpha(0.0f),
	  mp_textureBackground(nullptr),
	  mp_sprite(nullptr),
	  mp_matrixManager(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
}

/// <summary>
/// ���S�V�[���̏���������
/// </summary>
void SceneStageSelect::Initialize()
{
	m_toPlayMoveOnChecker = false;

	// �A�C�R���Փ˔���̏�����
	for (int i = 0; i < stage::NUM; i++)
	{
		m_collideStageIcon[i].pos = SimpleMath::Vector2(float((i * 30) + 315 + i * STAGE_ICON_SIZE), 360.0f);
		m_collideStageIcon[i].width = float(STAGE_ICON_SIZE);
		m_collideStageIcon[i].height = float(STAGE_ICON_SIZE);
	}

	// �e�N�X�`���̃��[�h
	for (int i = 0; i < stage::NUM; i++)
	{
		CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\StageSelect\\stage_len.png", nullptr, mp_textureStageIcon[i].GetAddressOf());
	}
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\StageSelect\\stageselect_background_image.png", nullptr, mp_textureBackground.GetAddressOf());

	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// ���l�̐ݒ�(������)
	m_colorAlpha = 1.0f;

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
	
	// �T�E���h�Đ�
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"SceneStageSelect.acb", L"SceneStageSelect.awb");
	adx2le->Play(0);
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneStageSelect::Finalize()
{
	// �s��Ǘ��ϐ��̍폜
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
/// �X�e�[�W�I���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	InputManager::SingletonGetInstance().Update();
	// �}�E�X�̍X�V
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// �T�E���h�̍X�V
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// �X�e�[�W�̑I��
	if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		SimpleMath::Vector2 mousePos = SimpleMath::Vector2(float(InputManager::SingletonGetInstance().GetMousePosX()), float(InputManager::SingletonGetInstance().GetMousePosY()));
		for (int i = 0; i < stage::NUM; i++)
		{
			// �}�E�X�ƃA�C�R���̏Փ˔���
			if (mousePos.x > m_collideStageIcon[i].pos.x && mousePos.x < m_collideStageIcon[i].pos.x + float(STAGE_ICON_SIZE) &&
				mousePos.y > m_collideStageIcon[i].pos.y && mousePos.y < m_collideStageIcon[i].pos.y + float(STAGE_ICON_SIZE))
			{
				// �I�����ꂽ�X�e�[�W�ԍ����L��
				m_selectedStage = i+1;
				// �v���C�X�e�[�W������
				//SceneManager::SetStageNum(m_selectedStage);
				// �V�[���J�ڔ���
				m_toPlayMoveOnChecker = true;
				// �T�E���h�Đ�
				adx2le->Play(1);
				break;
			}
		}
	}

	// �V�[���J�ڊJ�n
	if (m_toPlayMoveOnChecker)
	{
		m_colorAlpha -= 0.01f;
		if (m_colorAlpha < 0.0f) m_colorAlpha = 0.0f;
	}

	// �V�[���J��
	if (m_toPlayMoveOnChecker && m_colorAlpha <= 0.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneStageSelect::Render()
{
	// �X�e�[�W�Z���N�g��ʂ̕`��
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// �w�i
	RECT rectBG = { 0, 0, 800, 600};
	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectBG, SimpleMath::Vector4( 1.0f, 1.0f, 1.0f , m_colorAlpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// �A�C�R��
	for (int i = 0; i < stage::NUM; i++)
	{
		RECT rect = { i*STAGE_ICON_SIZE, 0, i*STAGE_ICON_SIZE+STAGE_ICON_SIZE, STAGE_ICON_SIZE };
		mp_sprite->Draw(mp_textureStageIcon[i].Get(), m_collideStageIcon[i].pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_colorAlpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
}

