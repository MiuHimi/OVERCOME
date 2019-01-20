//////////////////////////////////////////////////////////////
// File.    SceneLogo.cpp
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneManager.h"
#include "SceneLogo.h"

#include "../../Utility/GameDebug.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneLogo::SceneLogo(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  m_toTitleMoveOnChecker(false),
	  m_fadeoutNeedTime(2),
	  fadeoutCount(0),
	  mp_matrixManager(nullptr)
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
	// �e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\background.png", nullptr, m_textureBackground.GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\logo_image.png", nullptr, m_textureLogo.GetAddressOf());

	// �s��Ǘ��ϐ��̏�����
	mp_matrixManager = new MatrixManager();

	// �r���[�s��̍쐬
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::Identity;

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
}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneLogo::Finalize()
{
	if (mp_matrixManager != nullptr)
	{
		delete mp_matrixManager;
		mp_matrixManager = nullptr;
	}
}

/// <summary>
/// ���S�V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneLogo::Update(DX::StepTimer const& timer)
{
	// �t���[�����J�E���g
	static int count = 0;
	count++;

	// �t�F�[�h�A�E�g�J�n
	if (count / 60 >= m_fadeoutNeedTime)
	{
		fadeoutCount += 0.01f;
	}

	// �V�[���J��
	if (count / 60 >= m_sceneChangeNeedSecond)
	{
		m_toTitleMoveOnChecker = true;
		count = 0;
	}

	if (m_toTitleMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_TITLE);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneLogo::Render()
{
	// ���S�̕`��
	DrawManager::SingletonGetInstance().Draw(m_textureLogo.Get(), SimpleMath::Vector2(0.0f, 0.0f));
	DrawManager::SingletonGetInstance().DrawAlpha(m_textureBackground.Get(), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0, 1.0f, 1.0f, fadeoutCount));
}
