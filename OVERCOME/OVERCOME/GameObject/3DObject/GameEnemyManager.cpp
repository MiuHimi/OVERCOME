//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include <math.h>
#include <time.h>
#include "GameEnemyManager.h"
#include "Player.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/DrawManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_respawnTime(0), m_dengerousDirFB(DIRECTION::NONE), m_dengerousDirLR(DIRECTION::NONE), m_lengthTmp(0),m_assault(false),
	  mp_player(nullptr) ,m_textureDengerousH(nullptr), m_textureDengerousV(nullptr),m_textureSmoke(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameEnemyManager::~GameEnemyManager()
{
	// 敵オブジェクト削除
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		delete mp_enemy[i];
		mp_enemy[i] = nullptr;
	}

	// プレイヤーオブジェクト削除
	delete mp_player;
	mp_player = nullptr;
}

/// <summary>
/// 初期化
/// </summary>
void GameEnemyManager::Initialize()
{
	// 乱数初期化
	srand((unsigned int)time(NULL));
	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		m_shockPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		m_shockCount[i] = 0;
	}
}
/// <summary>
/// 生成
/// </summary>
void GameEnemyManager::Create()
{
	// 敵の初期化、生成
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		mp_enemy[i] = new GameEnemy(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create();
	}

	// 敵の初期化、生成
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		m_compereLength[i] = 0.0;
	}

	// 危険サインの設定
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\dangerous_signH.png", nullptr, m_textureDengerousH.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\dangerous_signV.png", nullptr, m_textureDengerousV.GetAddressOf());

	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\smoke.png", nullptr, m_textureSmoke.GetAddressOf());

	// エフェクトの作成
	m_batchEffect = std::make_unique<AlphaTestEffect>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	DX::DeviceResources::SingletonGetInstance().GetD3DDevice()->CreateInputLayout(VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <param name="player">プレイヤーの状態</param>
/// <param name="assaultPoint">襲撃ポイントかどうかをこれで判別</param>
/// <returns>終了状態</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, Player* player, int assaultPoint)
{
	m_respawnTime++;

	// カメラの更新
	mp_gameCamera->Update(timer, player);

	// プレイヤーを襲う位置にいなかったら
	if (assaultPoint == 0)
	{
		// 襲撃不可
		m_assault = false;
	}
	else
	{
		// 襲撃可
		m_assault = true;
	}

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// 敵の襲撃時間だったら更新する
		if (!player->GetEnemyTime())break;
		if (!mp_enemy[i]->GetState())
		{
			if (m_needRespawnTime < m_respawnTime && player->GetPlaying())
			{
				// まだ出現出来たら出現準備
				mp_enemy[i]->SetState(true);
				if (assaultPoint == 0)
				{
					mp_enemy[i]->SetState(false);
					break;
				}

				// カウントしなおし
				m_respawnTime = 0;

				// 場所設定
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

				// プレイヤーとの位置の差分
				SimpleMath::Vector3 enemyPosDiff = SimpleMath::Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x), float(player->GetPos().y - mp_enemy[i]->GetPos().y), float(player->GetPos().z - mp_enemy[i]->GetPos().z));
				// 敵の向き
				SimpleMath::Vector3 enemyDir = SimpleMath::Vector3(mp_enemy[i]->GetDir());

				{
					// ベクトルの長さを求める
					double lengthA = pow((enemyPosDiff.x * enemyPosDiff.x) + (enemyPosDiff.z * enemyPosDiff.z), 0.5);
					double lengthB = pow((enemyDir.x * enemyDir.x) + (enemyDir.z * enemyDir.z), 0.5);
					// 内積とベクトルの長さを使ってcosθを求める
					double cos_sita = enemyPosDiff.x * enemyDir.x + enemyPosDiff.z * enemyDir.z / (lengthA * lengthB);

					// cosθからθを求める
					double sita = acos(cos_sita);
					// デグリーで求める
					//sita = sita * 180.0 / double(XM_PI);

					mp_enemy[i]->SetRotateY(float(sita));
				}

				//{
				//	// ベクトルの長さを求める
				//	double lengthA = pow((enemyPosDiff.y * enemyPosDiff.y) + (enemyPosDiff.z * enemyPosDiff.z), 0.5);
				//	double lengthB = pow((enemyDir.y * enemyDir.y) + (enemyDir.z * enemyDir.z), 0.5);
				//	// 内積とベクトルの長さを使ってcosθを求める
				//	double cos_sita = enemyPosDiff.y * enemyDir.y + enemyPosDiff.z * enemyDir.z / (lengthA * lengthB);
				//	// cosθからθを求める
				//	double sita = acos(cos_sita);
				//	mp_enemy[i]->SetRotateX(float(sita));
				//}
			}
		}
	}

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// 敵の襲撃時間だったら更新する
		if (mp_enemy[i]->GetState() && !player->GetEnemyTime())
		{
			mp_enemy[i]->SetState(false);
			break;
		}
		// 敵の動き
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->SetVel(SimpleMath::Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x) / 80.0f,
										float(player->GetPos().y - mp_enemy[i]->GetPos().y) / 80.0f, 
										float(player->GetPos().z - mp_enemy[i]->GetPos().z) / 80.0f));

			double baseLength = 150.0;
			m_compereLength[i] = (mp_enemy[i]->GetPos().x - player->GetPos().x)*(mp_enemy[i]->GetPos().x - player->GetPos().x) +
								 (mp_enemy[i]->GetPos().y - player->GetPos().y)*(mp_enemy[i]->GetPos().y - player->GetPos().y) +
								 (mp_enemy[i]->GetPos().z - player->GetPos().z)*(mp_enemy[i]->GetPos().z - player->GetPos().z);
			if (baseLength*baseLength > m_compereLength[i] * m_compereLength[i])
			{
				if (m_compereLength[i] <= m_compereLength[m_lengthTmp])
				{
					m_lengthTmp = i;
				}

				// プレイヤーとの位置の差分
				SimpleMath::Vector3 pDir = mp_gameCamera->GetCameraAngle();
				pDir.Normalize();
				SimpleMath::Vector3 playerDir = SimpleMath::Vector3(pDir);
				// 敵の向き
				SimpleMath::Vector3 eDir = mp_enemy[m_lengthTmp]->GetVel();
				eDir.Normalize();
				SimpleMath::Vector3 enemyPos = SimpleMath::Vector3(eDir);

				// ベクトルの長さを求める
				double lengthA = pow((playerDir.x * playerDir.x) + (playerDir.z * playerDir.z), 0.5);
				double lengthB = pow((enemyPos.x * enemyPos.x) + (enemyPos.z * enemyPos.z), 0.5);
				// 内積とベクトルの長さを使ってcosθを求める
				double cos_sita = playerDir.x * enemyPos.x + playerDir.z * enemyPos.z / (lengthA * lengthB);

				// cosθからθを求める
				double sita = acos(cos_sita);
				// デグリーで求める
				sita = sita * 180.0 / double(XM_PI);

				// 正の値だったらプレイヤーに対して左側、逆だったら右側
				float dirRL = -playerDir.z * enemyPos.x + playerDir.x * enemyPos.z;
				// 正の値だったらプレイヤーに対して後ろ側、逆だったら前側
				float dirFB = playerDir.x * enemyPos.x + playerDir.z * enemyPos.z;

				if (dirRL < 0.0f)m_dengerousDirLR = DIRECTION::RIGHT;
				else m_dengerousDirLR = DIRECTION::LEFT;
				if (dirRL > -0.3f && dirRL < 0.3f)
				{
					m_assault = false; 
					m_dengerousDirLR = DIRECTION::NONE;
				}
				if (dirFB < 0.0f)m_dengerousDirFB = DIRECTION::FRONT;
				else m_dengerousDirFB = DIRECTION::BACK;
			}
			else
			{
				m_dengerousDirFB = DIRECTION::NONE;
				m_dengerousDirLR = DIRECTION::NONE;
			}

			mp_enemy[i]->Update(timer);

			// マップの中心から50ｍ離れたら消える
			float dist = (mp_enemy[i]->GetPos().x - 0.0f)*(mp_enemy[i]->GetPos().x - 0.0f) +
						 (mp_enemy[i]->GetPos().y - 0.0f)*(mp_enemy[i]->GetPos().y - 0.0f) +
						 (mp_enemy[i]->GetPos().z - 0.0f)*(mp_enemy[i]->GetPos().z - 0.0f);
			if (m_maxAliveDist * m_maxAliveDist < dist)
			{
				mp_enemy[i]->SetState(false);
			}
		}
	}

	// やられ演出関連
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// やられ演出サインが出たら
		if (mp_enemy[i]->GetShock())
		{
			// やられた位置から少しずつ上へ
			m_shockPos[i].y += 0.1f;
			// カウントを進める
			m_shockCount[i]++;
		}
		// 既定フレームを超えたら
		if (m_shockCount[i] > 20)
		{
			// カウントリセット
			m_shockCount[i] = 0;
			// 位置を元に戻す
			m_shockPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			// やられ演出サインを伏せる
			mp_enemy[i]->SetShock(false);
		}
	}

	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameEnemyManager::Render(MatrixManager* matrixManager, SimpleMath::Vector3 eyePos)
{
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->Render(matrixManager);
			/*SimpleMath::Matrix world = 
				SimpleMath::Matrix::CreateConstrainedBillboard(
					mp_enemy[i]->GetPos(),
					eyePos,
					SimpleMath::Vector3::Up);
			// やられ演出を描画
			DrawSmoke(matrixManager, world);*/
		}
	}

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		if (mp_enemy[i]->GetShock())
		{
			SimpleMath::Matrix world =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_shockPos[i], eyePos, SimpleMath::Vector3::Up);
			// やられ演出を描画
			int alpha = 255 - (10 * m_shockCount[i]);
			DrawSmoke(matrixManager, world, alpha);
		}
	}
	
	// 敵がいたら危険サイン表示
	if (m_assault == true)
	{
		/*if (m_dengerousDirFB == DIRECTION::FRONT)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousH.Get(), SimpleMath::Vector2(150.0f, 50.0f));
		}
		else if (m_dengerousDirFB == DIRECTION::BACK)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousH.Get(), SimpleMath::Vector2(150.0f, 750.0f));
		}*/
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
/// 廃棄
/// </summary>
void GameEnemyManager::Depose()
{
}

/// <summary>
/// やられ演出設定
/// </summary>
/// <param name="i">敵の番号(配列)</param>
void GameEnemyManager::ShockEnemy(int i)
{
	if (!mp_enemy[i]->GetShock())
	{
		mp_enemy[i]->SetShock(true);
		m_shockPos[i] = mp_enemy[i]->GetPos();
	}
}

/// <summary>
/// やられ演出
/// </summary>
/// <param name="matrixManager">行列管理変数</param>
/// <param name="world">ワールド座標</param>
void GameEnemyManager::DrawSmoke(MatrixManager* matrixManager, SimpleMath::Matrix &world, int &drawAlpha)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// 頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  0.5f,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, 0.5f,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, -0.5f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  -0.5f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// テクスチャサンプラーの設定（クランプテクスチャアドレッシングモード）
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	// 不透明に設定
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// カリングは左周り
	context->RSSetState(m_states->CullCounterClockwise());
	// 不透明のみ描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	m_batchEffect->SetTexture(m_textureSmoke.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	// 不透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
	// 不透明以外の半透明部分を描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->Apply(context);
	// 半透明で描画
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込まないが参照だけする
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// 半透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}
