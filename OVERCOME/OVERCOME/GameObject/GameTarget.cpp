//////////////////////////////////////////////////////////////
// File.    GameTarget.cpp
// Summary. GameTargetClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <fstream>
#include <string>
#include <sstream>

#include "../pch.h"
#include "GameTarget.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

//int SceneManager::m_stageID;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
GameTarget::GameTarget() : m_stageCount(0)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameTarget::~GameTarget()
{
	//GameRoad::Depose();
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			delete mp_targetCollideObject[j][i];
			mp_targetCollideObject[j][i] = nullptr;
		}
	}
}

/// <summary>
/// ����������
/// </summary>
void GameTarget::Initialize()
{
	// �X�e�[�W�}�b�v�̓ǂݍ���
	std::string filePath = "Resources\\TargetMap\\Stage";
	std::ostringstream os;
	m_stageCount = SceneManager::GetStageNum();
	if (m_stageCount != 1)m_stageCount = 1;
	os << m_stageCount;
	filePath += os.str() + ".csv";

	// �X�e�[�W�}�b�v�̎擾
	std::ifstream ifs(filePath);
	std::string line;
	if (!ifs)
	{
		// �t�@�C���ǂݍ��ݎ��s
		throw std::range_error("Read failure.");
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
			int targetType = std::atoi(buf.c_str());
			// �񌅂̂����A�\�̌��𓹘H�̎�ށA��̌�����]�p�ɐݒ�
			m_targetObject[j][i].height = targetType / 10;
			m_targetObject[j][i].rotaAngle = targetType % 10;
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
				m_targetObject[j][i].pos.x = float((m_maxFloorWidth / 2) - m_roadBlockSize * x - m_roadBlockSize / 2);
				m_targetObject[j][i].pos.x *= -1.0f;
			}
			else if (i >= 10)
			{
				// ���W��[x > 0]�̏ꍇ
				x -= 10;
				m_targetObject[j][i].pos.x = float(m_roadBlockSize * x + m_roadBlockSize / 2);
			}
			// Y���W�ݒ�
			m_targetObject[j][i].pos.y = m_targetObject[j][i].height + m_targetRadius;
			// Z���W�ݒ�
			int z = j;
			if (j >= 0 && j < 10)
			{
				// ���W��[z < 0]�̏ꍇ
				m_targetObject[j][i].pos.z = float((m_maxFloorHeight / 2) - m_roadBlockSize * z - m_roadBlockSize / 2);
				m_targetObject[j][i].pos.z *= -1.0f;
			}
			else if (j >= 10)
			{
				// ���W��[z > 0]�̏ꍇ
				z -= 10;
				m_targetObject[j][i].pos.z = float(m_roadBlockSize * z + m_roadBlockSize / 2);
			}
		}
	}
}
/// <summary>
/// ��������
/// </summary>
void GameTarget::Create()
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelTarget = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\target.cmo", fx);
	
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);      // ���E���̒��S
	box.r = DirectX::SimpleMath::Vector3(50.0f, 0.0f, 50.0f);    // �e���a

	// ���H�̍쐬
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			mp_targetCollideObject[j][i] = new CollisionBox();
			if(m_targetObject[j][i].height != 0)     mp_targetCollideObject[j][i]->SetModel(m_modelTarget.get());
			else mp_targetCollideObject[j][i]->SetModel(NULL);
			
		}
	}

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			if (m_targetObject[j][i].height != 0)
			{
				box.c = DirectX::SimpleMath::Vector3(m_targetObject[j][i].pos.x, m_targetObject[j][i].pos.y, m_targetObject[j][i].pos.z);                              // ���^���E�̒��S
				if (m_targetObject[j][i].rotaAngle == 0 || m_targetObject[j][i].rotaAngle == 2)box.r = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 0.1f);               // �e���a�ݒ�
				if (m_targetObject[j][i].rotaAngle == 1 || m_targetObject[j][i].rotaAngle == 1)box.r = DirectX::SimpleMath::Vector3(0.1f, 1.0f, 1.0f);               // �e���a�ݒ�

				mp_targetCollideObject[j][i]->SetCollision(box);
			}
		}
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameTarget::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// �`�揈��
/// </summary>
void GameTarget::Render()
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	//m_modelTarget->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
	//	                world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// ���W�m��
			trans = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(m_targetObject[j][i].pos.x, m_targetObject[j][i].pos.y, m_targetObject[j][i].pos.z));

			// ��]�ݒ�
			float angle = float(m_targetObject[j][i].rotaAngle * 90.0f);  // ��]�p��ݒ�( (0 or 1 or 2 or 3) * 90.0f )
			// ��]�m��
			rot = SimpleMath::Matrix::CreateFromQuaternion(SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(angle)));

			// �s��m��
			world = SimpleMath::Matrix::Identity;
			world *= rot * trans;

			auto& res = DX::DeviceResources::SingletonGetInstance();
			// �`�擹�H�I��
			
			if (m_targetObject[j][i].height != 0)m_modelTarget->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());
			// �f�o�b�O���H�`��
			if(m_targetObject[j][i].height != 0)mp_targetCollideObject[j][i]->DrawDebugCollision();
		}
	}
}

/// <summary>
/// �폜����
/// </summary>
void GameTarget::Depose()
{
}
