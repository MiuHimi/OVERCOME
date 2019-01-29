//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2018/11/30
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"
#include "../../Utility/InputManager.h"
#include "../../Utility/DrawManager.h"

#include "../ExclusiveGameObject/ADX2Le.h"

// using�f�B���N�g��
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  selectedStage(0),
	  mp_matrixManager(nullptr),
	  m_color(0.0f)
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

	// �E�C���h�E�T�C�Y����A�X�y�N�g����Z�o����
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// �e�N�X�`���̃��[�h
	for (int i = 0; i < stage::NUM; i++)
	{
		DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\StageSelect\\stage_len.png", nullptr, m_textureStageIcon[i].GetAddressOf());
	}

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\StageSelect\\stageselect_background_image.png", nullptr, m_textureBackground.GetAddressOf());

	for (int i = 0; i < stage::NUM; i++)
	{
		m_posStageIcon[i] = SimpleMath::Vector2((i*30)+315.0f + i * m_stageIconSize, 360.0f);
	}

	for (int i = 0; i < stage::NUM; i++)
	{
		m_collideStageIcon[i].pos = SimpleMath::Vector2((i * 30) + 315.0f + i * m_stageIconSize, 360.0f);
		m_collideStageIcon[i].width = 80.0f;
		m_collideStageIcon[i].height = 80.0f;
	}

	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

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
			if (mousePos.x > m_posStageIcon[i].x && mousePos.x < m_posStageIcon[i].x + m_stageIconSize &&
				mousePos.y > m_posStageIcon[i].y && mousePos.y < m_posStageIcon[i].y + m_stageIconSize)
			{
				selectedStage = i+1;
				SceneManager::SetStageNum(selectedStage);
				m_toPlayMoveOnChecker = true;
				adx2le->Play(1);
				break;
			}
		}
	}

	if (m_toPlayMoveOnChecker == true)
	{
		m_color += 0.01f;
	}

	if (m_toPlayMoveOnChecker == true && m_color > 1.0f)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneStageSelect::Render()
{
	// �؂���ꏊ��ݒ�
	RECT rectBG;
	rectBG.top = LONG(0.0f);
	rectBG.left = LONG(0.0f);
	rectBG.right = LONG(800.0f);
	rectBG.bottom = LONG(600.0f);

	// �^�C�g���̕`��
	mp_sprite->Begin(DirectX::SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// BG
	mp_sprite->Draw(m_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), &rectBG, SimpleMath::Vector4(1.0f - m_color, 1.0f - m_color, 1.0f - m_color, 1.0f), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// �A�C�R��
	for (int i = 0; i < stage::NUM; i++)
	{
		// �؂���ꏊ��ݒ�
		RECT rect;
		rect.top = LONG(0.0f);
		rect.left = LONG(i*m_stageIconSize);
		rect.right = LONG(i*m_stageIconSize + m_stageIconSize);
		rect.bottom = LONG(m_stageIconSize);

		mp_sprite->Draw(m_textureStageIcon[i].Get(), m_posStageIcon[i], &rect, SimpleMath::Vector4(1.0f - m_color, 1.0f - m_color, 1.0f - m_color, 1.0f), 0.0f, DirectX::XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
	

}

