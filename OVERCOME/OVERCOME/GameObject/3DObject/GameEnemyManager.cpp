//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <fstream>
#include <string>
#include <sstream>

#include "../../pch.h"
#include <math.h>
#include <time.h>
#include "GameEnemyManager.h"
#include "Player.h"
#include "GameMap.h"
#include "GameDecorateObject.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int GameEnemyManager::MAX_SPAWN_TIME = 600;
const int GameEnemyManager::CREATE_PROBABILITY = 5;
const int GameEnemyManager::RESPAWN_NEED_TIME = 40;
const int GameEnemyManager::BASE_LENGTH = 150;
const float GameEnemyManager::CONTROL_NORMAL_VELOCITY = 200.0f;
const float GameEnemyManager::CONTROL_POWER_VELOCITY = 300.0f;
const float GameEnemyManager::CONTROL_SPEED_VELOCITY = 100.0f;
const float GameEnemyManager::SMOKE_SPEED = 0.1f;
const int GameEnemyManager::MAX_SMOKE_COUNT = 20;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_spawnElapsedTime(0), m_respawnTime(0), 
	  m_createCount(0), m_entryEnemyPosTmp(0.0f,0.0f,0.0f), m_createFillFlag(false), m_openChestCreateFlag{ false }, m_chestCreateCount{ 0 },
	  m_entryEnemyPos{ SimpleMath::Vector3(0.0f,0.0f,0.0f) }, m_entryEnemyDistribute{ SimpleMath::Vector3(0.0f,0.0f,0.0f) },mp_enemy{ nullptr },
	  m_hpPos{ SimpleMath::Vector3(0.0f,0.0f,0.0f) }, m_shockPos{ SimpleMath::Vector3(0.0f,0.0f,0.0f) }, 
	  m_hitEffectState{ ENEMYHITEFFECT::EFFECT_FIRST }, m_hitPos{ SimpleMath::Vector3(0.0f,0.0f,0.0f) }, m_hitAnimationCount{0}, m_shockCount{ 0 },
	  m_pointPos{ SimpleMath::Vector3(0.0f,0.0f,0.0f) }, m_pointSize{ SimpleMath::Vector2(0.0f,0.0f) }, 
	  m_dengerousDirLR(DANGERDIRECTION::DIR_NONE), m_compereLength{0.0f}, m_lengthTmp(0), 
	  m_textureHP{ nullptr }, m_texturePoint{ nullptr }, m_textureHitEffect{ nullptr }, m_textureSmoke(nullptr),
	  m_batchEffect(nullptr), m_batch(nullptr), m_inputLayout(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameEnemyManager::~GameEnemyManager()
{
	// �G�I�u�W�F�N�g�폜
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		delete mp_enemy[i];
		mp_enemy[i] = nullptr;
	}
}

/// <summary>
/// ������
/// </summary>
void GameEnemyManager::Initialize()
{
	// ����������
	srand((unsigned int)time(NULL));

	// �T�E���h�Đ�
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"ScenePlay.acb", L"ScenePlay.awb");
}
/// <summary>
/// ����
/// </summary>
void GameEnemyManager::Create()
{
	// �G�̏������A����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 1�`100�Ő����̎�ނ𕪊򂳂���
		int probability = ((rand() % 100) + 1);
		mp_enemy[i] = new GameEnemy(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create(probability);
	}

	// �G�z�u�}�b�v�t�@�C���p�X�̐���
	std::string filePath = "Resources\\EnemyLayout\\Stage";
	std::ostringstream os;
	int stageNum = 1;
	os << stageNum;
	filePath += os.str() + ".csv";

	// �G�z�u�}�b�v�̎擾
	std::ifstream ifs(filePath);
	std::string line;
	if (!ifs)
	{
		// �t�@�C���ǂݍ��ݎ��s
		throw std::range_error("Read failure.");
	}

	// �G�z�u�}�b�v�̋L��
	int j = 0;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		int i = 0;
		// ��(x,y,z)���Ƃɐݒ肵���񐔂��J�E���g
		int axisCount = 0;
		// ��������l�����Z�����񐔂��J�E���g
		int distributeCount = 0;
		while (getline(stream, buf, ','))
		{
			i++;
			// �l�擾
			int num = std::atoi(buf.c_str());
			// �l��0�Ȃ猻�s�̎��̈ʒu�ݒ�͏I��
			if (num == 0)continue;

			if (i == ENTRYCOUNT::NONE)
			{
				//�����Ȓl
				throw("Invalid value.");
			}
			if (i >= ENTRYCOUNT::ENTRY_X && i <= ENTRYCOUNT::END_X)
			{
				// �J�E���g��������(�܂����ݒ�)
				if (axisCount == distributeCount)
				{
					// x�l�̈ʒu�ݒ�
					m_entryEnemyPos[j].x = (float)num;
					// �ݒ肪�I���������߃J�E���g��i�߂�
					axisCount++;
					continue;
				}
				// ���ݒ�J�E���g�̂ق����傫��(��ƂȂ�ʒu�͐ݒ�ς�)
				if (axisCount > distributeCount)
				{
					// x�l�̒����l�ݒ�
					m_entryEnemyDistribute[j].x = (float)num;
					// �ݒ肪�I���������߃J�E���g��i�߂�
					distributeCount++;
					continue;
				}
			}
			else if (i >= ENTRYCOUNT::ENTRY_Y && i <= ENTRYCOUNT::END_Y)
			{
				// �J�E���g��������(�܂����ݒ�)
				if (axisCount == distributeCount)
				{
					// y�l�̈ʒu�ݒ�
					m_entryEnemyPos[j].y = (float)num;
					// �ݒ肪�I���������߃J�E���g��i�߂�
					axisCount++;
					continue;
				}
				// ���ݒ�J�E���g�̂ق����傫��(��ƂȂ�ʒu�͐ݒ�ς�)
				if (axisCount > distributeCount)
				{
					// y�l�̒����l�ݒ�
					m_entryEnemyDistribute[j].y = (float)num;
					// �ݒ肪�I���������߃J�E���g��i�߂�
					distributeCount++;
					continue;
				}
			}
			else if (i >= ENTRYCOUNT::ENTRY_Z && i <= ENTRYCOUNT::END_Z)
			{
				// �J�E���g��������(�܂����ݒ�)
				if (axisCount == distributeCount)
				{
					// z�l�̈ʒu�ݒ�
					m_entryEnemyPos[j].z = (float)num;
					// �ݒ肪�I���������߃J�E���g��i�߂�
					axisCount++;
					continue;
				}
				// ���ݒ�J�E���g�̂ق����傫��(��ƂȂ�ʒu�͐ݒ�ς�)
				if (axisCount > distributeCount)
				{
					// z�l�̒����l�ݒ�
					m_entryEnemyDistribute[j].z = (float)num;
					// �ݒ肪�I���������߃J�E���g��i�߂�
					distributeCount++;
					continue;
				}
			}
		}
		j++;
	}
	
	for (int i = 0; i < GameEnemyManager::HP_NUM; i++)
	{
		// HP�o�[�̐ݒ�
		switch (i)
		{
		case (int)GameEnemyManager::HP_MAX:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_hp_max_frame.png", nullptr, m_textureHP[GameEnemyManager::HP_MAX].GetAddressOf());
			break;
		case (int)GameEnemyManager::HP_NORMAL_DAMAGE:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_hp_normal_damage_frame.png", nullptr, m_textureHP[GameEnemyManager::HP_NORMAL_DAMAGE].GetAddressOf());
			break;
		case (int)GameEnemyManager::HP_POWER_DAMAGE:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_hp_power_damage_frame.png", nullptr, m_textureHP[GameEnemyManager::HP_POWER_DAMAGE].GetAddressOf());
			break;
		case (int)GameEnemyManager::HP_POWER_CRITICAL:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_hp_power_critical_frame.png", nullptr, m_textureHP[GameEnemyManager::HP_POWER_CRITICAL].GetAddressOf());
			break;
		case (int)GameEnemyManager::HP_NUM:
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < GameEnemyManager::ENEMYTYPE::MAX_TYPE; i++)
	{
		// ���_�X�v���C�g�̐ݒ�
		switch (i)
		{
		case (int)GameEnemyManager::ENEMYTYPE::SPEED_TYPE:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_point10.png", nullptr, m_texturePoint[GameEnemyManager::ENEMYTYPE::SPEED_TYPE].GetAddressOf());
			break;
		case (int)GameEnemyManager::ENEMYTYPE::NORMAL_TYPE:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_point20.png", nullptr, m_texturePoint[GameEnemyManager::ENEMYTYPE::NORMAL_TYPE].GetAddressOf());
			break;
		case (int)GameEnemyManager::ENEMYTYPE::POWER_TYPE:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_point30.png", nullptr, m_texturePoint[GameEnemyManager::ENEMYTYPE::POWER_TYPE].GetAddressOf());
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < GameEnemyManager::ENEMYHITEFFECT::MAX_EFFECT; i++)
	{
		// �q�b�g�G�t�F�N�g�X�v���C�g�̐ݒ�
		switch (i)
		{
		case (int)GameEnemyManager::ENEMYHITEFFECT::EFFECT_FIRST:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_hiteffect_first.png", nullptr, m_textureHitEffect[GameEnemyManager::ENEMYHITEFFECT::EFFECT_FIRST].GetAddressOf());
			break;
		case (int)GameEnemyManager::ENEMYHITEFFECT::EFFECT_SECOND:
			CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\enemy_hiteffect_second.png", nullptr, m_textureHitEffect[GameEnemyManager::ENEMYHITEFFECT::EFFECT_SECOND].GetAddressOf());
			break;
		default:
			break;
		}
	}
	
	// ���ꉉ�o�p���̐ݒ�
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\smoke.png", nullptr, m_textureSmoke.GetAddressOf());

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// ���_�T�C�Y�̏�����
		m_pointSize[i] = SimpleMath::Vector2(0.3f, 0.2f);
	}
	
	// �G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<AlphaTestEffect>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	DX::DeviceResources::SingletonGetInstance().GetD3DDevice()->CreateInputLayout(VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <param name="playerPos">�v���C���[�̈ʒu</param>
/// <param name="roadType">���̎��</param>
/// <param name="assaultPoint">�P���|�C���g�̔ԍ�</param>
/// <param name="cameraDir">�J�����̌���(�v���C���[�̎���)</param>
/// <returns>�I�����</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, SimpleMath::Vector3& playerPos, int roadType, int assaultPoint, SimpleMath::Vector3& cameraDir, SimpleMath::Vector3* chestPos, float chestHeight, std::array<bool, 3> isChestOpen)
{
	// �T�C���g���ϓ����邽�߂̒l
	static float wave;
	wave += 0.1f;

	// ���ꉉ�o�֘A
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// ���e�ɓ���������
		if (mp_enemy[i]->GetHit())
		{
			// �A�j���[�V�����J�E���g
			if (m_hitAnimationCount[i] >= 0 && m_hitAnimationCount[i] < 5)
			{
				m_hitEffectState[i] = EFFECT_FIRST;
			}
			else if (m_hitAnimationCount[i] >= 5 && m_hitAnimationCount[i] < 10)
			{
				m_hitEffectState[i] = EFFECT_SECOND;
			}

			// �J�E���g��i�߂�
			m_hitAnimationCount[i]++;
		}
		// ����t���[���𒴂�����
		if (m_hitAnimationCount[i] > 10)
		{
			// �J�E���g���Z�b�g
			m_hitAnimationCount[i] = 0;
			// �ʒu�����ɖ߂�
			m_hitPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			// ���ꉉ�o�T�C���𕚂���
			mp_enemy[i]->SetHit(false);
		}
	}

	UpdateSmoke();
	UpdatePoint();

	// �P���\���ǂ���
	bool isAssault = IsAssault(roadType);
	// false��������P�����Ȃ�
	if (!isAssault)return false;

	// �G�����J�E���g
	m_respawnTime++;

	// �G�����Ǘ�
	CreateEnemy(assaultPoint, playerPos, chestPos, chestHeight, isChestOpen);

	// �G�ړ��Ǘ�
	MoveEnemy(timer, playerPos, cameraDir, wave);

	return true;
}
/// <summary>
/// �`��
/// </summary>
/// <param name="matrixManager">�s��Ǘ�</param>
/// <param name="eyePos">�v���C���[�̎��_</param>
void GameEnemyManager::Render(MatrixManager* matrixManager, SimpleMath::Vector3 eyePos)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �G���������Ă�����
		if (mp_enemy[i]->GetState())
		{
			// �G��`��
			mp_enemy[i]->Render(matrixManager);
			// ���e�̍U�����󂯂Ă���ԕ\��
			if (mp_enemy[i]->GetHit())
			{
				// �\���ʒu�ݒ�(�r���{�[�h�ŕ\��)
				SimpleMath::Matrix world =
					SimpleMath::Matrix::CreateConstrainedBillboard(
						m_hitPos[i], eyePos, SimpleMath::Vector3::Up);
				// �q�b�g�G�t�F�N�g��`��
				DrawHitEffect(matrixManager, world, m_hitEffectState[i]);
			}
			// �\���ʒu�ݒ�(�r���{�[�h�ŕ\��)
			SimpleMath::Matrix world =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_hpPos[i], eyePos, SimpleMath::Vector3::Up);
			// HP��`��
			DrawHP(matrixManager, world,i);
		}

		// ���e�̍U�����󂯂Ă�����
		if (mp_enemy[i]->GetShock())
		{
			// ���\���ʒu�ݒ�(�r���{�[�h�ŕ\��)
			SimpleMath::Matrix shockWorld =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_shockPos[i], eyePos, SimpleMath::Vector3::Up);
			// ���_�\���ʒu�ݒ�(�r���{�[�h�ŕ\��)
			SimpleMath::Matrix pointWorld =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_pointPos[i], eyePos, SimpleMath::Vector3::Up);
			// ���ꉉ�o��`��
			float alpha = 1.0f - (float)(m_shockCount[i] * 0.03f);
			if (alpha < 0.0f) alpha = 0.0f;
			DrawSmoke(matrixManager, shockWorld, alpha);
			DrawPoint(matrixManager, pointWorld, i, m_pointSize[i]);
		}
	}
}

/// <summary>
/// �p��
/// </summary>
void GameEnemyManager::Depose()
{
}

/// <summary>
/// ���ꉉ�o�ݒ�
/// </summary>
/// <param name="i">�G�̔ԍ�(�z��)</param>
void GameEnemyManager::ShockEnemy(int i)
{
	// ���ꉉ�o���s���Ă��Ȃ�������
	if (!mp_enemy[i]->GetShock())
	{
		// ���o�t���O�𗧂Ă�
		mp_enemy[i]->SetShock(true);
		// ���e�������������̓G�̈ʒu��ݒ�
		m_shockPos[i] = mp_enemy[i]->GetPos();
		m_pointPos[i] = mp_enemy[i]->GetPos();

		// �G����SE
		ADX2Le* adx2le = ADX2Le::GetInstance();
		adx2le->Play(5);
	}
}

/// <summary>
/// ���ꉉ�o�X�V
/// </summary>
void GameEnemyManager::UpdateSmoke()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// ���e�ɓ���������
		if (mp_enemy[i]->GetShock())
		{
			// ���������ʒu���班�������
			m_shockPos[i].y += SMOKE_SPEED;
			// �J�E���g��i�߂�
			m_shockCount[i]++;
		}
		// ����t���[���𒴂�����
		if (m_shockCount[i] > MAX_SMOKE_COUNT)
		{
			// �J�E���g���Z�b�g
			m_shockCount[i] = 0;
			// �ʒu�����ɖ߂�
			m_shockPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			// ���ꉉ�o�T�C���𕚂���
			mp_enemy[i]->SetShock(false);
		}
	}
}

/// <summary>
/// ���_���o�X�V
/// </summary>
void GameEnemyManager::UpdatePoint()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// ���e�ɓ���������
		if (mp_enemy[i]->GetShock())
		{
			m_pointSize[i].x += 0.005f;
			m_pointSize[i].y += 0.005f;
		}
		// ����t���[���𒴂�����
		if (m_shockCount[i] > MAX_SMOKE_COUNT)
		{
			m_pointSize[i] = SimpleMath::Vector2(0.3f, 0.2f);
		}
	}
}

/// <summary>
/// �P���\���ǂ�����Ԃ�
/// </summary>
/// <param name="roadType">�`�F�b�N���铹�̎��</param>
bool GameEnemyManager::IsAssault(int roadType)
{
	// �P���|�C���g��������P���A�����łȂ���ΏP�����Ȃ�
	if (roadType == (int)GameRoad::RoadType::ASSAULT)
	{
		// �o���ł��鎞�Ԃ܂ŏP������
		m_spawnElapsedTime++;
		if (m_spawnElapsedTime > MAX_SPAWN_TIME)
		{
			m_spawnElapsedTime = 0;
		}
	}
	else
	{
		// �J�E���g���Z�b�g
		m_respawnTime = 0;

		// �P���|�C���g�ɂ��Ȃ���ΏP�����Ȃ�
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			// �c�����G��Еt����
			if (mp_enemy[i]->GetState())
			{
				// ���o���ĕЕt����
				mp_enemy[i]->SetState(false);
				ShockEnemy(i);
			}

		}

		// �P���s��
		m_dengerousDirLR = DANGERDIRECTION::DIR_NONE;
		return false;
	}

	return true;
}

/// <summary>
/// �G�����Ǘ�
/// </summary>
/// <param name="assultP">�P���|�C���g</param>
/// <param name="playerPos">�v���C���[�̈ʒu</param>
/// <param name="chestEntryPos">�`�F�X�g����o������ʒu(�z��)</param>
/// <param name="isChestOpen">�`�F�X�g���J���Ă��邩(�z��)</param>
void GameEnemyManager::CreateEnemy(int assultP, DirectX::SimpleMath::Vector3& playerPos, SimpleMath::Vector3* chestEntryPos, float chestHeight, std::array<bool, 3> isChestOpen)
{
	// �������ԂɂȂ�����G�̐ݒ������
	if (assultP == 1 && m_respawnTime % RESPAWN_NEED_TIME != 0)return;
	if (assultP == 2)
	{
		for (int i = 0; i < (int)GameDecorateObject::MAX_CHEST_NUM; i++)
		{
			// �����ł���ꏊ����������
			if (!m_openChestCreateFlag[i])
			{
				// �`�F�X�g���J���Ă�����
				if (isChestOpen[i])
				{
					// �����ł���
					break;
				}
				else
				{
					if (i != ((int)GameDecorateObject::MAX_CHEST_NUM - 1))
						continue;
					else
						return;
				}
			}
			else
			{
				// �Ăѐ����ł���悤�ɃJ�E���g��i�߂�
				m_chestCreateCount[i]++;
				// �����ł���J�E���g�ɂȂ�����
				if (m_chestCreateCount[i] > RESPAWN_NEED_TIME)
				{
					// �Ăѐ���������
					m_openChestCreateFlag[i] = false;
					// �J�E���g�����Z�b�g
					m_chestCreateCount[i] = 0;
				}

				if (i != ((int)GameDecorateObject::MAX_CHEST_NUM - 1))
					continue;
				else
					// �ǂ̏ꏊ�������ł��Ȃ������炱��ȍ~�̏����͍s��Ȃ�
					return;
			}
		}
	}
	if (assultP == 3 && m_respawnTime % 120 != 0)return;
	if (assultP == 4 && m_respawnTime % 120 != 0 || m_createFillFlag)return;

	// �G�̏P�����Ԃ�������X�V����
	int posXTmp = 0;
	int posYTmp = 0;
	ADX2Le* adx2le = ADX2Le::GetInstance();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �G�����`�v���C���[�֌����鏈��
		if (!mp_enemy[i]->GetState())
		{
			// ���������ڕW�ɒB������
			if ((assultP == 3 && m_createCount >= (int)MAXCREATECOUNT::FOUR) || (assultP == 4 && m_createCount >= (int)MAXCREATECOUNT::SIXTEEN))
			{
				// �J�E���g���Z�b�g���Ĕ�����
				m_createCount = 0;
				m_respawnTime = 0;
				break;
			}

			// �܂��o���o������o������
			mp_enemy[i]->SetState(true);

			// �ꏊ�ݒ�
			int point = assultP - 1;
			switch (point)
			{
			case 0:
				mp_enemy[i]->SetPos(SimpleMath::Vector3(m_entryEnemyPos[point].x + (rand() % (int)m_entryEnemyDistribute[point].x - (m_entryEnemyDistribute[point].x / 2)),
														m_entryEnemyPos[point].y + (rand() % (int)m_entryEnemyDistribute[point].y),
														m_entryEnemyPos[point].z));

				// �G����SE
				adx2le->Play(3);
				break;
			case 1:
				for (int j = 0; j < (int)GameDecorateObject::MAX_CHEST_NUM; j++)
				{
					// �����ł��邩����
					int isCreate = rand() % CREATE_PROBABILITY;
					if (isCreate != 0)
					{
						if (!m_openChestCreateFlag[j] && isChestOpen[j])
						{
							mp_enemy[i]->SetPos(SimpleMath::Vector3(chestEntryPos[j].x,
																	chestEntryPos[j].y + chestHeight,
																	chestEntryPos[j].z));
							m_openChestCreateFlag[j] = true;

							// �G����SE
							adx2le->Play(3);
							break;
						}
					}
					else
					{
						// ���ɂ������琶����������ɂ��Ă���
						m_openChestCreateFlag[j] = true;
						// true�ɂ���state��false�ɖ߂�
						mp_enemy[i]->SetState(false); 
						return;
					}
				}
				break;
			case 2:
				mp_enemy[i]->SetPos(SimpleMath::Vector3(m_entryEnemyPos[point].x ,
														m_entryEnemyPos[point].y + posYTmp * (mp_enemy[i]->GetSize()*2.0f),
														m_entryEnemyPos[point].z));
				// �J�E���g
				posYTmp++;
				// ���������J�E���g
				m_createCount++; 

				// �G����SE
				adx2le->Play(3);

				break;
			case 3:
				mp_enemy[i]->SetPos(SimpleMath::Vector3(m_entryEnemyPos[point].x + posXTmp * (mp_enemy[i]->GetSize()*3.0f),
														m_entryEnemyPos[point].y + posYTmp * (mp_enemy[i]->GetSize()*3.0f),
														m_entryEnemyPos[point].z));
				// �G����ׂ邽�߂̃J�E���g
				posXTmp++;
				if (posXTmp == (int)MAXCREATECOUNT::FOUR)
				{
					posXTmp = 0;
					posYTmp++;
				}
				// ���������J�E���g
				m_createCount++;
				m_createFillFlag = true;

				// �G����SE
				adx2le->Play(3);

				break;
			default:
				break;
			}
			
			// �̗͐ݒ�
			switch (mp_enemy[i]->GetType())
			{
			case (int)GameEnemy::EnemyType::NORMAL:
				mp_enemy[i]->SetHP(2);
				break;
			case (int)GameEnemy::EnemyType::POWER:
				mp_enemy[i]->SetHP(3);
				break;
			case (int)GameEnemy::EnemyType::SPEED:
				mp_enemy[i]->SetHP(1);
				break;
			}

			// �v���C���[�̈ʒu
			SimpleMath::Vector3 pointA = playerPos;
			// �G�̈ʒu
			SimpleMath::Vector3 pointB = mp_enemy[i]->GetPos();
			// �v���C���[����G�Ɍ������x�N�g��
			SimpleMath::Vector3 subVector = pointA - pointB;

			// �Ƃ����߂�
			float sita = std::atan2(subVector.x, subVector.z);
			// �Z�o�����l�ŉ�]���v���C���[�Ɍ�����
			mp_enemy[i]->SetRotateY(sita);

			if ((assultP == 1 || assultP == 2))
			break;
		}
	}
}

/// <summary>
/// �G�ړ��Ǘ�
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <param name="playerPos">�v���C���[�̈ʒu</param>
/// <param name="cameraDir">�v���C���[�̌���(�J�����̌���)</param>
void GameEnemyManager::MoveEnemy(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3& cameraDir, float& waveValue)
{
	// �G�ړ�
	int initID = -1;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �G���������Ă�����
		if (mp_enemy[i]->GetState())
		{
			float controlSpeed = 0.0f;
			switch (mp_enemy[i]->GetType())
			{
			case GameEnemy::EnemyType::NORMAL:
				controlSpeed = CONTROL_NORMAL_VELOCITY;
				break;
			case GameEnemy::EnemyType::POWER:
				controlSpeed = CONTROL_POWER_VELOCITY;
				break;
			case GameEnemy::EnemyType::SPEED:
				controlSpeed = CONTROL_SPEED_VELOCITY;
				break;
			}

			// �ӂ�ӂ킳���邽�߂ɃT�C���g���g�p
			float sinWave = sin(waveValue + float(i*2)) * 2.0f;
			// ���x�ݒ�
			mp_enemy[i]->SetVel(SimpleMath::Vector3((playerPos.x - mp_enemy[i]->GetPos().x) / controlSpeed,
												   ((playerPos.y - mp_enemy[i]->GetPos().y) + sinWave) / controlSpeed,
													(playerPos.z - mp_enemy[i]->GetPos().z) / controlSpeed));

			// �G�X�V
			mp_enemy[i]->Update(timer);

			// HP�o�[�\���ʒu�ݒ�
			m_hpPos[i] = mp_enemy[i]->GetPos();
			m_hpPos[i].y -= mp_enemy[i]->GetSize() /2.0f;

			m_compereLength[i] = (mp_enemy[i]->GetPos().x - playerPos.x)*(mp_enemy[i]->GetPos().x - playerPos.x) +
								 (mp_enemy[i]->GetPos().y - playerPos.y)*(mp_enemy[i]->GetPos().y - playerPos.y) +
								 (mp_enemy[i]->GetPos().z - playerPos.z)*(mp_enemy[i]->GetPos().z - playerPos.z);
			// �댯�T�C�����o����ʒu�ɂ܂ŗ�����
			if (BASE_LENGTH * BASE_LENGTH > m_compereLength[i] * m_compereLength[i])
			{
				// �ŏ�������r�p�ɕۑ����Ă���
				if (initID == -1)
				{
					initID = i;
					m_lengthTmp = initID;
				}
				// ��ԋ߂��G�̔z��ԍ���ۑ�
				if (m_compereLength[i] <= m_compereLength[m_lengthTmp])
				{
					m_lengthTmp = i;
				}

				// �O�ς��g���ăv���C���[���猩�Ăǂ��瑤�ɂ��邩�𔻒�
				SimpleMath::Vector3 pointA = playerPos;
				SimpleMath::Vector3 camDir = cameraDir;
				SimpleMath::Vector3 pointB = pointA + camDir;
				SimpleMath::Vector3 pointC = mp_enemy[m_lengthTmp]->GetPos();

				float p = (pointB.x - pointA.x)*(pointC.z - pointA.z) -
						  (pointB.z - pointA.z)*(pointC.x - pointA.x);

				// ���E����
				if (p > 0.0f)m_dengerousDirLR = DANGERDIRECTION::DIR_RIGHT;
				else if (p < 0.0f) m_dengerousDirLR = DANGERDIRECTION::DIR_LEFT;
				// �قڐ^�񒆂ɋ�����T�C����\�����Ȃ�
				else
				{
					m_dengerousDirLR = DANGERDIRECTION::DIR_NONE;
				}
			}
			else
			{
				m_dengerousDirLR = DANGERDIRECTION::DIR_NONE;
			}
		}
	}
}

/// <summary>
/// HP�\��
/// </summary>
/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
/// <param name="world">���[���h�s��</param>
/// <param name="enemyID">�G��ID(�^�C�v)</param>
void GameEnemyManager::DrawHP(MatrixManager * matrixManager, DirectX::SimpleMath::Matrix & world, int enemyID)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// ���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  0.1f,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, 0.1f,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, -0.1f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  -0.1f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	// �s�����ɐݒ�
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->SetFogEnabled(true);
	m_batchEffect->SetFogStart(4.0f);
	m_batchEffect->SetFogEnd(10.0f);
	m_batchEffect->SetFogColor(Colors::Black);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	switch (mp_enemy[enemyID]->GetType())
	{
	// �ʏ�G�̏ꍇ
	case GameEnemy::EnemyType::NORMAL:
		if(mp_enemy[enemyID]->GetHP() == 2)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_MAX].Get());
		else if(mp_enemy[enemyID]->GetHP() == 1)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_NORMAL_DAMAGE].Get());
		else 
			// ������HP
			throw std::range_error("Invalid HP.");
		break;
	// �p���[�G�̏ꍇ
	case GameEnemy::EnemyType::POWER:
		if (mp_enemy[enemyID]->GetHP() == 3)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_MAX].Get());
		else if (mp_enemy[enemyID]->GetHP() == 2)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_POWER_DAMAGE].Get());
		else if (mp_enemy[enemyID]->GetHP() == 1)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_POWER_CRITICAL].Get());
		else
			// ������HP
			throw std::range_error("Invalid HP.");
		break;
	// �X�s�[�h�G�̏ꍇ
	case GameEnemy::EnemyType::SPEED:
		if (mp_enemy[enemyID]->GetHP() == 1)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_MAX].Get());
		else
			// ������HP
			throw std::range_error("Invalid HP.");
		break;
	default:
		break;
	}
	
	m_batchEffect->SetAlpha(1.0f);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	// �s����������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
	// �s�����ȊO�̔�����������`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->Apply(context);
	// �������ŕ`��
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������܂Ȃ����Q�Ƃ�������
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

/// <summary>
/// �q�b�g�G�t�F�N�g�\��
/// </summary>
/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
/// <param name="world">���[���h�s��</param>
/// <param name="enemyEffectState">�G�̃q�b�g�G�t�F�N�g�̏��</param>
void GameEnemyManager::DrawHitEffect(MatrixManager * matrixManager, DirectX::SimpleMath::Matrix & world, ENEMYHITEFFECT enemyEffectState)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// ���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(0.9f,  0.9f,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.9f, 0.9f,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.9f, -0.9f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.9f,  -0.9f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	// �s�����ɐݒ�
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->SetFogEnabled(true);
	m_batchEffect->SetFogStart(4.0f);
	m_batchEffect->SetFogEnd(10.0f);
	m_batchEffect->SetFogColor(Colors::Black);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	switch (enemyEffectState)
	{
		// �G�t�F�N�g��i�K�ڂ̏ꍇ
	case GameEnemyManager::ENEMYHITEFFECT::EFFECT_FIRST:
		m_batchEffect->SetTexture(m_textureHitEffect[ENEMYHITEFFECT::EFFECT_FIRST].Get());
		break;
		// �G�t�F�N�g��i�K�ڂ̏ꍇ
	case GameEnemyManager::ENEMYHITEFFECT::EFFECT_SECOND:
		m_batchEffect->SetTexture(m_textureHitEffect[ENEMYHITEFFECT::EFFECT_SECOND].Get());
		break;
	default:
		// �����Ȓl
		throw std::range_error("Invalid Value.");
		break;
	}

	m_batchEffect->SetAlpha(1.0f);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	// �s����������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
	// �s�����ȊO�̔�����������`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->Apply(context);
	// �������ŕ`��
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������܂Ȃ����Q�Ƃ�������
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

/// <summary>
/// ���_�\��
/// </summary>
/// <param name="matrixManager">�s��Ǘ��I�u�W�F�N�g</param>
/// <param name="world">���[���h�s��</param>
/// <param name="enemyID">�G��ID(�^�C�v)</param>
void GameEnemyManager::DrawPoint(MatrixManager * matrixManager, DirectX::SimpleMath::Matrix & world, int enemyID, SimpleMath::Vector2 &size)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// ���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(size.x,  size.y,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-size.x, size.y,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-size.x, -size.y, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(size.x,  -size.y, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	// �s�����ɐݒ�
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->SetFogEnabled(true);
	m_batchEffect->SetFogStart(4.0f);
	m_batchEffect->SetFogEnd(10.0f);
	m_batchEffect->SetFogColor(Colors::Black);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	switch (mp_enemy[enemyID]->GetType())
	{
		// �ʏ�G�̏ꍇ
	case GameEnemy::EnemyType::NORMAL:
		m_batchEffect->SetTexture(m_texturePoint[ENEMYTYPE::NORMAL_TYPE].Get());
		break;
		// �p���[�G�̏ꍇ
	case GameEnemy::EnemyType::POWER:
		m_batchEffect->SetTexture(m_texturePoint[ENEMYTYPE::POWER_TYPE].Get());
		break;
		// �X�s�[�h�G�̏ꍇ
	case GameEnemy::EnemyType::SPEED:
		m_batchEffect->SetTexture(m_texturePoint[ENEMYTYPE::SPEED_TYPE].Get());
		break;
	default:
		break;
	}

	m_batchEffect->SetAlpha(1.0f);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	// �s����������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
	// �s�����ȊO�̔�����������`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->Apply(context);
	// �������ŕ`��
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������܂Ȃ����Q�Ƃ�������
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}

/// <summary>
/// ���ꉉ�o
/// </summary>
/// <param name="matrixManager">�s��Ǘ��ϐ�</param>
/// <param name="world">���[���h���W</param>
void GameEnemyManager::DrawSmoke(MatrixManager* matrixManager, SimpleMath::Matrix &world, float &drawAlpha)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// ���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  0.5f,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, 0.5f,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, -0.5f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  -0.5f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	// �s�����ɐݒ�
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	m_batchEffect->SetTexture(m_textureSmoke.Get());
	m_batchEffect->SetAlpha(drawAlpha);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	// �s����������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
	// �s�����ȊO�̔�����������`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetAlpha(drawAlpha);
	m_batchEffect->Apply(context);
	// �������ŕ`��
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������܂Ȃ����Q�Ƃ�������
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}
