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

#include "../../pch.h"
#include "../SceneObject/SceneManager.h"
#include "GameRoad.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

//int SceneManager::m_stageID;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameRoad::GameRoad()
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameRoad::~GameRoad()
{
}

/// <summary>
/// ����������
/// </summary>
void GameRoad::Initialize()
{
	// �X�e�[�W�}�b�v�̓ǂݍ���
	std::string filePath = "Resources\\StageMap\\stage";
	std::ostringstream os;
	int stageNum = 1;
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

	// �����l�ƃS�[���̈ʒu�̋L��
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		int i = 0;
		while (getline(stream, buf, ','))
		{
			int pos = std::atoi(buf.c_str());
			m_posStartEnd[i].x = float(pos / 100);
			m_posStartEnd[i].y = float(pos % 100);
			i++;

			if (i > 1) break;
		}
		if (i > 1) break;
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
			// �񌅂̂����A�\�̌��𓹂̎��(�P���|�C���g��X�^�[�g�S�[���Ȃ�)�A��̌����P������^�C�~���O���ɐݒ�
			m_road[j][i].roadType = roadType / 10;
			m_road[j][i].roadNum = roadType % 10;
			i++;
		}
		j++;
	}

	
	// ���̍��W�ݒ�
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// X���W�ݒ�
			int x = i;
			if (i >= 0 && i < 10)
			{
				// ���W��[x < 0]�̏ꍇ
				m_road[j][i].pos.x = float((m_maxFloorWidth / 2) - m_roadBlockSize * x - m_roadBlockSize / 2);
				m_road[j][i].pos.x *= -1.0f;
			}
			else if (i >= 10)
			{
				// ���W��[x > 0]�̏ꍇ
				x -= 10;
				m_road[j][i].pos.x = float(m_roadBlockSize * x + m_roadBlockSize / 2);
			}
			// Y���W�ݒ�
			m_road[j][i].pos.y = 0.0f;
			// Z���W�ݒ�
			int z = j;
			if (j >= 0 && j < 10)
			{
				// ���W��[z < 0]�̏ꍇ
				m_road[j][i].pos.z = float((m_maxFloorHeight / 2) - m_roadBlockSize * z - m_roadBlockSize / 2);
				m_road[j][i].pos.z *= -1.0f;
			}
			else if (j >= 10)
			{
				// ���W��[z > 0]�̏ꍇ
				z -= 10;
				m_road[j][i].pos.z = float(m_roadBlockSize * z + m_roadBlockSize / 2);
			}
		}
	}
	
	
}
/// <summary>
/// ��������
/// </summary>
void GameRoad::Create()
{
	// �t�H�O�̐ݒ�
	SetFogEffectDistance(15.0f, 20.0f);
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
void GameRoad::Render(MatrixManager* matrixManager)
{
}

/// <summary>
/// �폜����
/// </summary>
void GameRoad::Depose()
{
}

/// <summary>
/// �t�H�O�̃X�^�[�g�ƃG���h��ݒ�
/// </summary>
/// <param name="start">���ʂ�������n�߂鋗��</param>
/// <param name="end">���ʂ����S�ɂ����鋗��</param>
void GameRoad::SetFogEffectDistance(float start, float end)
{
	// �t�H�O�̐ݒ�
	//m_modelRoadStraight->UpdateEffects([&](IEffect* effect)
	//{
	//	auto fog = dynamic_cast<IEffectFog*>(effect);
	//	if (fog)
	//	{
	//		fog->SetFogEnabled(true);
	//		fog->SetFogStart(start); // assuming RH coordiantes
	//		fog->SetFogEnd(end);
	//		fog->SetFogColor(Colors::Black);
	//	}
	//});
}
