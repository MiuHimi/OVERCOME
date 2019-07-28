//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include <math.h>
#include <time.h>
#include "GameEnemyManager.h"
#include "Player.h"
#include "GameMap.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int GameEnemyManager::MAX_SPAWN_TIME = 600;
const int GameEnemyManager::RESPAWN_NEED_TIME = 40;
const int GameEnemyManager::BASE_LENGTH = 150;
const float GameEnemyManager::CONTROL_VELOCITY = 100.0f;
const float GameEnemyManager::SMOKE_SPEED = 0.1f;
const int GameEnemyManager::MAX_SMOKE_COUNT = 20;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_spawnElapsedTime(0), m_respawnTime(0), mp_enemy{ nullptr }, 
	  m_shockPos{ SimpleMath::Vector3(0.0f,0.0f,0.0f) }, m_shockCount{0},
	  m_dengerousDirLR(DANGERDIRECTION::NONE), m_compereLength{0.0f}, m_lengthTmp(0), 
	  m_textureSmoke(nullptr),
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
}
/// <summary>
/// ����
/// </summary>
void GameEnemyManager::Create()
{
	// �G�̏������A����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		mp_enemy[i] = new GameEnemy(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create();
	}

	// ���ꉉ�o�p���̐ݒ�
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\smoke.png", nullptr, m_textureSmoke.GetAddressOf());

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
bool GameEnemyManager::Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, int roadType, int assaultPoint, DirectX::SimpleMath::Vector3& cameraDir)
{
	// ���ꉉ�o�֘A
	UpdateSmoke();

	// �P���\���ǂ���
	bool isAssault = IsAssault(roadType);
	// false��������P�����Ȃ�
	if (!isAssault)return false;

	// �G�����J�E���g
	m_respawnTime++;

	// �G�����Ǘ�
	CreateEnemy(assaultPoint, playerPos);

	// �G�ړ��Ǘ�
	MoveEnemy(timer, playerPos, cameraDir);

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameEnemyManager::Render(MatrixManager* matrixManager, SimpleMath::Vector3 eyePos)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �G���������Ă�����
		if (mp_enemy[i]->GetState())
		{
			// �G��`��
			mp_enemy[i]->Render(matrixManager);
		}

		// ���e�̍U�����󂯂Ă�����
		if (mp_enemy[i]->GetShock())
		{
			// �\���ʒu�ݒ�(�r���{�[�h�ŕ\��)
			SimpleMath::Matrix world =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_shockPos[i], eyePos, SimpleMath::Vector3::Up);
			// ���ꉉ�o��`��
			int alpha = 255 - (10 * m_shockCount[i]);
			DrawSmoke(matrixManager, world, alpha);
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
		m_dengerousDirLR = DANGERDIRECTION::NONE;
		return false;
	}

	return true;
}

/// <summary>
/// �G�����Ǘ�
/// </summary>
/// <param name="assultP">�P���|�C���g</param>
/// <param name="playerPos">�v���C���[�̈ʒu</param>
void GameEnemyManager::CreateEnemy(int assultP, DirectX::SimpleMath::Vector3& playerPos)
{
	// �������ԂɂȂ�����
	if (m_respawnTime % RESPAWN_NEED_TIME == 0)
	{
		// �G�̏P�����Ԃ�������X�V����
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			// �G�����`�v���C���[�֌�����
			if (!mp_enemy[i]->GetState())
			{
				// �܂��o���o������o������
				mp_enemy[i]->SetState(true);

				// �ꏊ�ݒ�
				switch (assultP)
				{
				case 1: mp_enemy[i]->SetPos(SimpleMath::Vector3(30.0f + float(rand() % 10 - 5), 5.0f + float(rand() % 5), -35.0f)); break;
				case 2: mp_enemy[i]->SetPos(SimpleMath::Vector3(-12.5f + float(rand() % 14 - 7), 5.0f + float(rand() % 5), -32.5f + float(rand() % 14 - 7))); break;
				case 3: mp_enemy[i]->SetPos(SimpleMath::Vector3(17.5f + float(rand() % 14 - 7), 5.0f + float(rand() % 5), 32.5f + float(rand() % 14 - 7))); break;
				case 4: mp_enemy[i]->SetPos(SimpleMath::Vector3(-10.0f + float(rand() % 20 - 10), 5.0f + float(rand() % 5), 5.0f)); break;
				case 5: mp_enemy[i]->SetPos(SimpleMath::Vector3(22.5f + float(rand() % 40 - 20), 7.0f, -42.5f)); break;
				case 6: mp_enemy[i]->SetPos(SimpleMath::Vector3(12.5f + float(rand() % 60 - 30), 7.0f, float(rand() & 10))); break;
				case 7: mp_enemy[i]->SetPos(SimpleMath::Vector3(32.5f + float(rand() % 60 - 30), 7.0f, 7.5f + float(rand() % 30 - 15))); break;
				}

				// �v���C���[�Ƃ̈ʒu�̍���(�v���C���[�Ɍ����������������߂���)
				SimpleMath::Vector3 toPlayerDir =
					SimpleMath::Vector3(playerPos.x - mp_enemy[i]->GetPos().x,
										playerPos.y - mp_enemy[i]->GetPos().y,
										playerPos.z - mp_enemy[i]->GetPos().z);
				// �G�̌���
				SimpleMath::Vector3 enemyDir = SimpleMath::Vector3(mp_enemy[i]->GetDir());

				// �x�N�g���̒��������߂�
				double lengthA = pow((toPlayerDir.x * toPlayerDir.x) + (toPlayerDir.z * toPlayerDir.z), 0.5);
				double lengthB = pow((enemyDir.x * enemyDir.x) + (enemyDir.z * enemyDir.z), 0.5);
				// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
				double cos_sita = toPlayerDir.x * enemyDir.x + toPlayerDir.z * enemyDir.z / (lengthA * lengthB);

				// cos�Ƃ���Ƃ����߂�
				double sita = acos(cos_sita);
				// �f�O���[�ŋ��߂�
				//sita = sita * 180.0 / double(XM_PI);

				mp_enemy[i]->SetRotateY(float(sita));

				break;
			}

		}
	}
}

/// <summary>
/// �G�ړ��Ǘ�
/// </summary>
/// <param name="timer">�o�ߎ���</param>
/// <param name="playerPos">�v���C���[�̈ʒu</param>
/// <param name="cameraDir">�v���C���[�̌���(�J�����̌���)</param>
void GameEnemyManager::MoveEnemy(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3& cameraDir)
{
	// �G�ړ�
	int initID = -1;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �G���������Ă�����
		if (mp_enemy[i]->GetState())
		{
			// ���x�ݒ�
			mp_enemy[i]->SetVel(SimpleMath::Vector3((playerPos.x - mp_enemy[i]->GetPos().x) / CONTROL_VELOCITY,
													(playerPos.y - mp_enemy[i]->GetPos().y) / CONTROL_VELOCITY,
													(playerPos.z - mp_enemy[i]->GetPos().z) / CONTROL_VELOCITY));

			// �G�X�V
			mp_enemy[i]->Update(timer);

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
					m_dengerousDirLR = DANGERDIRECTION::NONE;
				}
			}
			else
			{
				m_dengerousDirLR = DANGERDIRECTION::NONE;
			}
		}
	}
}

/// <summary>
/// ���ꉉ�o
/// </summary>
/// <param name="matrixManager">�s��Ǘ��ϐ�</param>
/// <param name="world">���[���h���W</param>
void GameEnemyManager::DrawSmoke(MatrixManager* matrixManager, SimpleMath::Matrix &world, int &drawAlpha)
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
