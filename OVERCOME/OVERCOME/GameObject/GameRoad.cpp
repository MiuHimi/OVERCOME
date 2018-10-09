//////////////////////////////////////////////////////////////
// File.    GameRoad.cpp
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <fstream>
#include <string>
#include <sstream>

#include "../pch.h"
#include "GameRoad.h"

#include "../ExclusiveGameObject/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

int SceneManager::m_stageNum;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="game">�Q�[���I�u�W�F�N�g</param>
GameRoad::GameRoad(Game* game) : mp_game(game)
                                ,m_stageNum(0)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameRoad::~GameRoad()
{
	//GameRoad::Depose();
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			delete mp_roadCollideObject[j][i];
			mp_roadCollideObject[j][i] = nullptr;
		}
	}
}

/// <summary>
/// ����������
/// </summary>
void GameRoad::Initialize()
{
	// �X�e�[�W�}�b�v�̓ǂݍ���
	std::string filePath = "Resources\\StageMap\\Stage";
	std::ostringstream os;
	m_stageNum = SceneManager::GetStageNum();
	os << m_stageNum;
	filePath += os.str() + ".csv";

	// �X�e�[�W�}�b�v�̎擾
	//std::ifstream ifs(L"Resources\\StageMap\\Stage02.csv");
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
/// ��������
/// </summary>
void GameRoad::Create(Game* game)
{
	// �G�t�F�N�g�t�@�N�g���[
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// ���f���̃e�N�X�`���̓����Ă���t�H���_���w�肷��
	fx.SetDirectory(L"Resources\\Models");
	// ���f�������[�h���ă��f���n���h�����擾����
	m_modelRoadStraight = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_straight.cmo", fx);
	m_modelRoadStop = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_stop.cmo", fx);
	m_modelRoadCurve = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_curve.cmo", fx);
	m_modelRoadBranch = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_branch.cmo", fx);

	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);      // ���E���̒��S
	box.r = DirectX::SimpleMath::Vector3(50.0f, 0.0f, 50.0f);    // �e���a

	// ���H�̍쐬
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			mp_roadCollideObject[j][i] = new CollisionBox();
			mp_roadCollideObject[j][i]->SetGame(game);
			if (m_roadObject[j][i].roadType == 1)     mp_roadCollideObject[j][i]->SetModel(m_modelRoadStraight.get());
			else if (m_roadObject[j][i].roadType == 2)mp_roadCollideObject[j][i]->SetModel(m_modelRoadStop.get());
			else if (m_roadObject[j][i].roadType == 3)mp_roadCollideObject[j][i]->SetModel(m_modelRoadCurve.get());
			else if (m_roadObject[j][i].roadType == 4)mp_roadCollideObject[j][i]->SetModel(m_modelRoadBranch.get());
			else(mp_roadCollideObject[j][i]->SetModel(NULL));
		}
	}

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			if (m_roadObject[j][i].roadType == 1)
			{
				box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);                              // ���^���E�̒��S
				if (m_roadObject[j][i].rotaAngle == 0 || m_roadObject[j][i].rotaAngle == 2)box.r = DirectX::SimpleMath::Vector3(1.5f, 1.0f, 2.5f);               // �e���a�ݒ�
				if (m_roadObject[j][i].rotaAngle == 1 || m_roadObject[j][i].rotaAngle == 1)box.r = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 1.5f);               // �e���a�ݒ�
				//m_roadStraight->SetCollision(box);
				//m_roadStraight->DrawDebugCollision();
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
			if (m_roadObject[j][i].roadType == 2)
			{
				if (m_roadObject[j][i].rotaAngle == 0)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z + 0.5f);   // ���^���E�̒��S
				if (m_roadObject[j][i].rotaAngle == 1)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x + 0.5f, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // ���^���E�̒��S
				if (m_roadObject[j][i].rotaAngle == 2)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z - 0.5f);   // ���^���E�̒��S
				if (m_roadObject[j][i].rotaAngle == 3)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x - 0.5f, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // ���^���E�̒��S
				if (m_roadObject[j][i].rotaAngle == 0 || m_roadObject[j][i].rotaAngle == 2)box.r = DirectX::SimpleMath::Vector3(1.5f, 1.0f, 2.0f);               // �e���a�ݒ�
				if (m_roadObject[j][i].rotaAngle == 1 || m_roadObject[j][i].rotaAngle == 3)box.r = DirectX::SimpleMath::Vector3(2.0f, 1.0f, 1.5f);               // �e���a�ݒ�
				//m_roadStop->SetCollision(box);
				//m_roadStop->DrawDebugCollision();
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
			if (m_roadObject[j][i].roadType == 3)
			{
				box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // ���^���E�̒��S
				box.r = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 2.5f);               // �e���a�ݒ�
				//m_roadCurve->SetCollision(box);
				//m_roadCurve->DrawDebugCollision();
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
			if (m_roadObject[j][i].roadType == 4)
			{
				box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // ���^���E�̒��S
				box.r = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 2.5f);               // �e���a�ݒ�
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
		}
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
/// <returns>�I�����</returns>
bool GameRoad::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// �`�揈��
/// </summary>
void GameRoad::Render(DirectX::SimpleMath::Matrix view)
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
			DirectX::SimpleMath::Matrix& projection = MatrixManager::GetProjectionMatrix();
			// �`�擹�H�I��
			int roadType = m_roadObject[j][i].roadType;
			switch (roadType)
			{
			case 0: break;                                                                                                                  // �����Ȃ�
			case 1: m_modelRoadStraight->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection); break;   // �������H
			case 2: m_modelRoadStop->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection);     break;   // ���[���H
			case 3: m_modelRoadCurve->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection);    break;   // �Ȑ����H
			case 4: m_modelRoadBranch->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection);    break;   // ���򓹘H
			}
			// �f�o�b�O���H�`��
			//if(m_roadObject[j][i].roadType == 1 || m_roadObject[j][i].roadType == 2 || m_roadObject[j][i].roadType == 3)mp_roadCollideObject[j][i]->DrawDebugCollision(view);
		}
	}
}

/// <summary>
/// �폜����
/// </summary>
void GameRoad::Depose()
{
	// �Q�[���I�u�W�F�N�g���폜
	delete mp_game;
	mp_game = NULL;
}
