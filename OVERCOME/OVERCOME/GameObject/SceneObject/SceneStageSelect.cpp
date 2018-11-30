//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

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
	  m_showFlag(false),
	  m_selectSceneID(0)
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

	// �J�����I�u�W�F�N�g�̍쐬
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelRoadStraight = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_straight.cmo", fx);
	m_modelRoadStop = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_stop.cmo", fx);
	m_modelRoadCurve = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_curve.cmo", fx);
	m_modelRoadBranch = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_branch.cmo", fx);

}

/// <summary>
/// ���S�V�[���̏I������
/// </summary>
void SceneStageSelect::Finalize()
{
}

/// <summary>
/// �X�e�[�W�I���V�[���̍X�V����
/// </summary>
/// <param name="timer">���ԏ��</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	// �J�����̍X�V(�v���C���[���͖���)
	mp_camera->Update(timer, nullptr);

	// ���͏����X�V
	InputManager::SingletonGetInstance().Update();

	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	{
		m_selectSceneID++;
		m_showFlag = false;
		if (m_selectSceneID > 2)m_selectSceneID = 0;
		SceneManager::SetStageNum(m_selectSceneID);
	}

	// �L�[����
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ���S�V�[���̕`�揈��
/// </summary>
void SceneStageSelect::Render()
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

	// �f�o�b�O�p
	GameDebug::SingletonGetInstance().DebugRender("SceneStageSelect", DirectX::SimpleMath::Vector2(20.0f, 10.0f));

	if (!m_showFlag)
	{
		/*switch (SceneManager::GetStageNum())
		{
		case 1:
			LoadStage(m_selectSceneID);
			GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			m_showFlag = true;
			break;
		case 2:
			LoadStage(m_selectSceneID);
			GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			m_showFlag = true;
			break;
		default:
			GameDebug::SingletonGetInstance().DebugRender("StageNone", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			break;
		}*/
		if (m_selectSceneID == 0)
		{
			GameDebug::SingletonGetInstance().DebugRender("StageNone", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
		}
		else
		{
			LoadStage(m_selectSceneID);
			GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			m_showFlag = true;
		}
	}
	
	GameDebug::SingletonGetInstance().Render();
	if (m_showFlag)
	{
		ShowStage();
	}
}

/// <summary>
/// �I�����Ă���X�e�[�W��\��
/// </summary>
/// <param name="stageID">�X�e�[�WID</param>
void SceneStageSelect::LoadStage(int stageID)
{
	// �X�e�[�W�}�b�v�̓ǂݍ���
	std::string filePath = "Resources\\StageMap\\Stage";
	std::ostringstream os;
	int stageNum = m_selectSceneID;
	os << stageNum;
	filePath += os.str() + ".csv";

	// �X�e�[�W�}�b�v�̎擾
	std::ifstream ifs(filePath);
	std::string line;
	if (!ifs)
	{
		// �t�@�C���ǂݍ��ݎ��s
		throw std::range_error("Read failure.");
	}

	/*// �}�b�v�̍s����ǂݍ���
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		// �}�b�v�̗񐔂�ǂݍ���
		while (getline(wifs, line))
		{

		}
	}
	
	// �s�z����m�ۂ���
	map.resize(mapRow);
	// ��z����m�ۂ���
	for (int row = 0; row < mapRow; row++)
	{
		map[row].resize(mapColum);
	}*/

	// 
	m_roadObject.resize(m_maxFloorBlock);
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			m_roadObject[i].resize(m_maxFloorBlock);
		}
	}


	int j = 0;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		int i = 0;
		while (getline(stream, buf, ','))
		{
			// �����𐔒l�ɕϊ�
			int roadType = std::atoi(buf.c_str());
			// �񌅂̂����A�\�̌��𓹘H�̎�ށA��̌�����]�p�ɐݒ�
			m_roadObject[j][i].roadType = roadType / 10;
			m_roadObject[j][i].rotaAngle = roadType % 10;
			i++;
		}
		j++;
	}

	// ���H�̍��W�ݒ�
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// X���W�ݒ�
			int x = i;
			if (i >= 0 && i < 10)
			{
				// ���W��[x < 0]�̏ꍇ
				m_roadObject[j][i].pos.x = float((m_maxFloorWidth / 2) - m_roadBlockSize * x - m_roadBlockSize / 2);
				m_roadObject[j][i].pos.x *= -1.0f;
			}
			else if (i >= 10)
			{
				// ���W��[x > 0]�̏ꍇ
				x -= 10;
				m_roadObject[j][i].pos.x = float(m_roadBlockSize * x + m_roadBlockSize / 2);
			}
			// Y���W�ݒ�
			m_roadObject[j][i].pos.y = 0.0f;
			// Z���W�ݒ�
			int z = j;
			if (j >= 0 && j < 10)
			{
				// ���W��[z < 0]�̏ꍇ
				m_roadObject[j][i].pos.z = float((m_maxFloorHeight / 2) - m_roadBlockSize * z - m_roadBlockSize / 2);
				m_roadObject[j][i].pos.z *= -1.0f;
			}
			else if (j >= 10)
			{
				// ���W��[z > 0]�̏ꍇ
				z -= 10;
				m_roadObject[j][i].pos.z = float(m_roadBlockSize * z + m_roadBlockSize / 2);
			}
		}
	}
}
/// <summary>
/// �I�����Ă���X�e�[�W��\��
/// </summary>
void SceneStageSelect::ShowStage()
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// ���W�m��
			trans = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z));

			// ��]�ݒ�
			float angle = float(m_roadObject[j][i].rotaAngle * 90.0f);  // ��]�p��ݒ�( (0 or 1 or 2 or 3) * 90.0f )
			// ��]�m��
			rot = SimpleMath::Matrix::CreateFromQuaternion(SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(angle)));

			// �s��m��
			world = SimpleMath::Matrix::Identity;
			world *= rot * trans;

			auto& res = DX::DeviceResources::SingletonGetInstance();
			// �`�擹�H�I��
			int roadType = m_roadObject[j][i].roadType;
			switch (roadType)
			{
			case 0: break;                                                                                                                  // �����Ȃ�
			case 1: m_modelRoadStraight->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());  break;   // �������H
			case 2: m_modelRoadStop->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());      break;   // ���[���H
			case 3: m_modelRoadCurve->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());     break;   // �Ȑ����H
			case 4: m_modelRoadBranch->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());    break;   // ���򓹘H
			}
		}
	}


}
