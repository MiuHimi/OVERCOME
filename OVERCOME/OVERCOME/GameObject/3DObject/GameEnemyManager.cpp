//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
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
#include "../../Utility/DrawManager.h"

// using�f�B���N�g��
using namespace DirectX;

// const�f�B���N�g��
const int GameEnemyManager::SPAWNTIME = 600;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_respawnTime(0), m_dengerousDirFB(DIRECTION::NONE), m_dengerousDirLR(DIRECTION::NONE), m_lengthTmp(0),m_assault(false),
	  mp_player(nullptr) ,m_textureDengerousH(nullptr), m_textureDengerousV(nullptr),m_textureSmoke(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameEnemyManager::~GameEnemyManager()
{
	// �G�I�u�W�F�N�g�폜
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		delete mp_enemy[i];
		mp_enemy[i] = nullptr;
	}

	// �v���C���[�I�u�W�F�N�g�폜
	delete mp_player;
	mp_player = nullptr;
}

/// <summary>
/// ������
/// </summary>
void GameEnemyManager::Initialize()
{
	// ����������
	srand((unsigned int)time(NULL));

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		m_shockPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		m_shockCount[i] = 0;
	}
}
/// <summary>
/// ����
/// </summary>
void GameEnemyManager::Create()
{
	// �G�̏������A����
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		mp_enemy[i] = new GameEnemy(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create();
	}

	// �G�̏������A����
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		m_compereLength[i] = 0.0;
	}

	// �댯�T�C���̐ݒ�
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\dangerous_signH.png", nullptr, m_textureDengerousH.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\dangerous_signV.png", nullptr, m_textureDengerousV.GetAddressOf());

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
/// <returns>�I�����</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, int roadType, int assaultPoint, DirectX::SimpleMath::Vector3& cameraDir)
{
	// �P���|�C���g��������P���A�����łȂ���ΏP�����Ȃ�
	if (roadType == (int)GameRoad::RoadType::ASSAULT)
	{
		// �P����
		m_assault = true;

		m_spawnElapsedTime++;
		if (m_spawnElapsedTime > SPAWNTIME)
		{
			m_spawnElapsedTime = 0;
		}
	}
	else
	{
		// �J�E���g���Z�b�g
		m_respawnTime = 0;

		for (int i = 0; i < m_maxEnemyNum; i++)
		{
			// �P���|�C���g�ɂ��Ȃ�������P���𒆎~����
			if (roadType == !(int)GameRoad::RoadType::ASSAULT)
			{
				mp_enemy[i]->SetState(false);
			}
		}

		// �P���s��
		m_assault = false;
		return false;
	}

	// �G�����J�E���g
	m_respawnTime++;

	// �������ԂɂȂ�����
	if (m_respawnTime % m_needRespawnTime == 0)
	{
		// �G�̏P�����Ԃ�������X�V����
		for (int i = 0; i < m_maxEnemyNum; i++)
		{
			// �G�����`�v���C���[�֌�����
			if (!mp_enemy[i]->GetState())
			{
				// �܂��o���o������o������
				mp_enemy[i]->SetState(true);

				// �ꏊ�ݒ�
				switch (assaultPoint)
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

	// �G�ړ�
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// �G���������Ă�����
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->SetVel(SimpleMath::Vector3((playerPos.x - mp_enemy[i]->GetPos().x) / 80.0f,
													(playerPos.y - mp_enemy[i]->GetPos().y) / 80.0f,
													(playerPos.z - mp_enemy[i]->GetPos().z) / 80.0f));

			mp_enemy[i]->Update(timer);

			double baseLength = 150.0;
			m_compereLength[i] = (mp_enemy[i]->GetPos().x - playerPos.x)*(mp_enemy[i]->GetPos().x - playerPos.x) +
				(mp_enemy[i]->GetPos().y - playerPos.y)*(mp_enemy[i]->GetPos().y - playerPos.y) +
				(mp_enemy[i]->GetPos().z - playerPos.z)*(mp_enemy[i]->GetPos().z - playerPos.z);
			if (baseLength*baseLength > m_compereLength[i] * m_compereLength[i])
			{
				if (m_compereLength[i] <= m_compereLength[m_lengthTmp])
				{
					m_lengthTmp = i;
				}

				// �v���C���[�Ƃ̈ʒu�̍���
				SimpleMath::Vector3 pDir = cameraDir;
				pDir.Normalize();
				SimpleMath::Vector3 playerDir = SimpleMath::Vector3(pDir);
				// �G�̌���
				SimpleMath::Vector3 eDir = mp_enemy[m_lengthTmp]->GetVel();
				eDir.Normalize();
				SimpleMath::Vector3 enemyPos = SimpleMath::Vector3(eDir);

				// �x�N�g���̒��������߂�
				double lengthA = pow((playerDir.x * playerDir.x) + (playerDir.z * playerDir.z), 0.5);
				double lengthB = pow((enemyPos.x * enemyPos.x) + (enemyPos.z * enemyPos.z), 0.5);
				// ���ςƃx�N�g���̒������g����cos�Ƃ����߂�
				double cos_sita = playerDir.x * enemyPos.x + playerDir.z * enemyPos.z / (lengthA * lengthB);

				// cos�Ƃ���Ƃ����߂�
				double sita = acos(cos_sita);
				// �f�O���[�ŋ��߂�
				sita = sita * 180.0 / double(XM_PI);

				// ���̒l��������v���C���[�ɑ΂��č����A�t��������E��
				float dirRL = -playerDir.z * enemyPos.x + playerDir.x * enemyPos.z;

				if (dirRL < 0.0f)m_dengerousDirLR = DIRECTION::RIGHT;
				else m_dengerousDirLR = DIRECTION::LEFT;
				if (dirRL > -0.3f && dirRL < 0.3f)
				{
					m_assault = false;
					m_dengerousDirLR = DIRECTION::NONE;
				}

			}
			else
			{
				m_dengerousDirFB = DIRECTION::NONE;
				m_dengerousDirLR = DIRECTION::NONE;
			}


			// �}�b�v�̒��S����50�����ꂽ�������
			float dist = (mp_enemy[i]->GetPos().x - 0.0f)*(mp_enemy[i]->GetPos().x - 0.0f) +
				(mp_enemy[i]->GetPos().y - 0.0f)*(mp_enemy[i]->GetPos().y - 0.0f) +
				(mp_enemy[i]->GetPos().z - 0.0f)*(mp_enemy[i]->GetPos().z - 0.0f);
			if (m_maxAliveDist * m_maxAliveDist < dist)
			{
				mp_enemy[i]->SetState(false);
			}
		}
	}

	// ���ꉉ�o�֘A
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// ���ꉉ�o�T�C�����o����
		if (mp_enemy[i]->GetShock())
		{
			// ���ꂽ�ʒu���班�������
			m_shockPos[i].y += 0.1f;
			// �J�E���g��i�߂�
			m_shockCount[i]++;
		}
		// ����t���[���𒴂�����
		if (m_shockCount[i] > 20)
		{
			// �J�E���g���Z�b�g
			m_shockCount[i] = 0;
			// �ʒu�����ɖ߂�
			m_shockPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			// ���ꉉ�o�T�C���𕚂���
			mp_enemy[i]->SetShock(false);
		}
	}

	return true;
}
/// <summary>
/// �`��
/// </summary>
void GameEnemyManager::Render(MatrixManager* matrixManager, SimpleMath::Vector3 eyePos)
{
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		if (mp_enemy[i]->GetState())
		{
			// �G��`��
			mp_enemy[i]->Render(matrixManager);
		}

		if (mp_enemy[i]->GetShock())
		{
			SimpleMath::Matrix world =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_shockPos[i], eyePos, SimpleMath::Vector3::Up);
			// ���ꉉ�o��`��
			int alpha = 255 - (10 * m_shockCount[i]);
			DrawSmoke(matrixManager, world, alpha);
		}
	}
	
	// �G��������댯�T�C���\��
	if (m_assault == true)
	{
		if (m_dengerousDirLR == DIRECTION::RIGHT)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousV.Get(), SimpleMath::Vector2(700.0f, 50.0f));
		}
		else if (m_dengerousDirLR == DIRECTION::LEFT)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousV.Get(), SimpleMath::Vector2(50.0f, 50.0f));
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
	if (!mp_enemy[i]->GetShock())
	{
		mp_enemy[i]->SetShock(true);
		m_shockPos[i] = mp_enemy[i]->GetPos();
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
