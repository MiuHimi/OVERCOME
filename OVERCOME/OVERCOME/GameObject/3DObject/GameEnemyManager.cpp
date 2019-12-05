//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
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

// usingディレクトリ
using namespace DirectX;

// constディレクトリ
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
/// コンストラクタ
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
/// デストラクタ
/// </summary>
GameEnemyManager::~GameEnemyManager()
{
	// 敵オブジェクト削除
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		delete mp_enemy[i];
		mp_enemy[i] = nullptr;
	}
}

/// <summary>
/// 初期化
/// </summary>
void GameEnemyManager::Initialize()
{
	// 乱数初期化
	srand((unsigned int)time(NULL));

	// サウンド再生
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->LoadAcb(L"ScenePlay.acb", L"ScenePlay.awb");
}
/// <summary>
/// 生成
/// </summary>
void GameEnemyManager::Create()
{
	// 敵の初期化、生成
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 1～100で生成の種類を分岐させる
		int probability = ((rand() % 100) + 1);
		mp_enemy[i] = new GameEnemy(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create(probability);
	}

	// 敵配置マップファイルパスの生成
	std::string filePath = "Resources\\EnemyLayout\\Stage";
	std::ostringstream os;
	int stageNum = 1;
	os << stageNum;
	filePath += os.str() + ".csv";

	// 敵配置マップの取得
	std::ifstream ifs(filePath);
	std::string line;
	if (!ifs)
	{
		// ファイル読み込み失敗
		throw std::range_error("Read failure.");
	}

	// 敵配置マップの記憶
	int j = 0;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		int i = 0;
		// 軸(x,y,z)ごとに設定した回数をカウント
		int axisCount = 0;
		// 調整する値を加算した回数をカウント
		int distributeCount = 0;
		while (getline(stream, buf, ','))
		{
			i++;
			// 値取得
			int num = std::atoi(buf.c_str());
			// 値が0なら現行の軸の位置設定は終了
			if (num == 0)continue;

			if (i == ENTRYCOUNT::NONE)
			{
				//無効な値
				throw("Invalid value.");
			}
			if (i >= ENTRYCOUNT::ENTRY_X && i <= ENTRYCOUNT::END_X)
			{
				// カウント数が同じ(まだ未設定)
				if (axisCount == distributeCount)
				{
					// x値の位置設定
					m_entryEnemyPos[j].x = (float)num;
					// 設定が終了したためカウントを進める
					axisCount++;
					continue;
				}
				// 軸設定カウントのほうが大きい(基準となる位置は設定済み)
				if (axisCount > distributeCount)
				{
					// x値の調整値設定
					m_entryEnemyDistribute[j].x = (float)num;
					// 設定が終了したためカウントを進める
					distributeCount++;
					continue;
				}
			}
			else if (i >= ENTRYCOUNT::ENTRY_Y && i <= ENTRYCOUNT::END_Y)
			{
				// カウント数が同じ(まだ未設定)
				if (axisCount == distributeCount)
				{
					// y値の位置設定
					m_entryEnemyPos[j].y = (float)num;
					// 設定が終了したためカウントを進める
					axisCount++;
					continue;
				}
				// 軸設定カウントのほうが大きい(基準となる位置は設定済み)
				if (axisCount > distributeCount)
				{
					// y値の調整値設定
					m_entryEnemyDistribute[j].y = (float)num;
					// 設定が終了したためカウントを進める
					distributeCount++;
					continue;
				}
			}
			else if (i >= ENTRYCOUNT::ENTRY_Z && i <= ENTRYCOUNT::END_Z)
			{
				// カウント数が同じ(まだ未設定)
				if (axisCount == distributeCount)
				{
					// z値の位置設定
					m_entryEnemyPos[j].z = (float)num;
					// 設定が終了したためカウントを進める
					axisCount++;
					continue;
				}
				// 軸設定カウントのほうが大きい(基準となる位置は設定済み)
				if (axisCount > distributeCount)
				{
					// z値の調整値設定
					m_entryEnemyDistribute[j].z = (float)num;
					// 設定が終了したためカウントを進める
					distributeCount++;
					continue;
				}
			}
		}
		j++;
	}
	
	for (int i = 0; i < GameEnemyManager::HP_NUM; i++)
	{
		// HPバーの設定
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
		// 得点スプライトの設定
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
		// ヒットエフェクトスプライトの設定
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
	
	// やられ演出用煙の設定
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\Play\\smoke.png", nullptr, m_textureSmoke.GetAddressOf());

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 得点サイズの初期化
		m_pointSize[i] = SimpleMath::Vector2(0.3f, 0.2f);
	}
	
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
/// <param name="playerPos">プレイヤーの位置</param>
/// <param name="roadType">道の種類</param>
/// <param name="assaultPoint">襲撃ポイントの番号</param>
/// <param name="cameraDir">カメラの向き(プレイヤーの視線)</param>
/// <returns>終了状態</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, SimpleMath::Vector3& playerPos, int roadType, int assaultPoint, SimpleMath::Vector3& cameraDir, SimpleMath::Vector3* chestPos, float chestHeight, std::array<bool, 3> isChestOpen)
{
	// サイン波が変動するための値
	static float wave;
	wave += 0.1f;

	// やられ演出関連
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 自弾に当たったら
		if (mp_enemy[i]->GetHit())
		{
			// アニメーションカウント
			if (m_hitAnimationCount[i] >= 0 && m_hitAnimationCount[i] < 5)
			{
				m_hitEffectState[i] = EFFECT_FIRST;
			}
			else if (m_hitAnimationCount[i] >= 5 && m_hitAnimationCount[i] < 10)
			{
				m_hitEffectState[i] = EFFECT_SECOND;
			}

			// カウントを進める
			m_hitAnimationCount[i]++;
		}
		// 既定フレームを超えたら
		if (m_hitAnimationCount[i] > 10)
		{
			// カウントリセット
			m_hitAnimationCount[i] = 0;
			// 位置を元に戻す
			m_hitPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			// やられ演出サインを伏せる
			mp_enemy[i]->SetHit(false);
		}
	}

	UpdateSmoke();
	UpdatePoint();

	// 襲撃可能かどうか
	bool isAssault = IsAssault(roadType);
	// falseだったら襲撃しない
	if (!isAssault)return false;

	// 敵生成カウント
	m_respawnTime++;

	// 敵生成管理
	CreateEnemy(assaultPoint, playerPos, chestPos, chestHeight, isChestOpen);

	// 敵移動管理
	MoveEnemy(timer, playerPos, cameraDir, wave);

	return true;
}
/// <summary>
/// 描画
/// </summary>
/// <param name="matrixManager">行列管理</param>
/// <param name="eyePos">プレイヤーの視点</param>
void GameEnemyManager::Render(MatrixManager* matrixManager, SimpleMath::Vector3 eyePos)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 敵が生存していたら
		if (mp_enemy[i]->GetState())
		{
			// 敵を描画
			mp_enemy[i]->Render(matrixManager);
			// 自弾の攻撃を受けている間表示
			if (mp_enemy[i]->GetHit())
			{
				// 表示位置設定(ビルボードで表示)
				SimpleMath::Matrix world =
					SimpleMath::Matrix::CreateConstrainedBillboard(
						m_hitPos[i], eyePos, SimpleMath::Vector3::Up);
				// ヒットエフェクトを描画
				DrawHitEffect(matrixManager, world, m_hitEffectState[i]);
			}
			// 表示位置設定(ビルボードで表示)
			SimpleMath::Matrix world =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_hpPos[i], eyePos, SimpleMath::Vector3::Up);
			// HPを描画
			DrawHP(matrixManager, world,i);
		}

		// 自弾の攻撃を受けていたら
		if (mp_enemy[i]->GetShock())
		{
			// 煙表示位置設定(ビルボードで表示)
			SimpleMath::Matrix shockWorld =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_shockPos[i], eyePos, SimpleMath::Vector3::Up);
			// 得点表示位置設定(ビルボードで表示)
			SimpleMath::Matrix pointWorld =
				SimpleMath::Matrix::CreateConstrainedBillboard(
					m_pointPos[i], eyePos, SimpleMath::Vector3::Up);
			// やられ演出を描画
			float alpha = 1.0f - (float)(m_shockCount[i] * 0.03f);
			if (alpha < 0.0f) alpha = 0.0f;
			DrawSmoke(matrixManager, shockWorld, alpha);
			DrawPoint(matrixManager, pointWorld, i, m_pointSize[i]);
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
	// やられ演出を行っていなかったら
	if (!mp_enemy[i]->GetShock())
	{
		// 演出フラグを立てる
		mp_enemy[i]->SetShock(true);
		// 自弾が当たった時の敵の位置を設定
		m_shockPos[i] = mp_enemy[i]->GetPos();
		m_pointPos[i] = mp_enemy[i]->GetPos();

		// 敵消滅SE
		ADX2Le* adx2le = ADX2Le::GetInstance();
		adx2le->Play(5);
	}
}

/// <summary>
/// やられ演出更新
/// </summary>
void GameEnemyManager::UpdateSmoke()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 自弾に当たったら
		if (mp_enemy[i]->GetShock())
		{
			// 当たった位置から少しずつ上へ
			m_shockPos[i].y += SMOKE_SPEED;
			// カウントを進める
			m_shockCount[i]++;
		}
		// 既定フレームを超えたら
		if (m_shockCount[i] > MAX_SMOKE_COUNT)
		{
			// カウントリセット
			m_shockCount[i] = 0;
			// 位置を元に戻す
			m_shockPos[i] = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
			// やられ演出サインを伏せる
			mp_enemy[i]->SetShock(false);
		}
	}
}

/// <summary>
/// 得点演出更新
/// </summary>
void GameEnemyManager::UpdatePoint()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 自弾に当たったら
		if (mp_enemy[i]->GetShock())
		{
			m_pointSize[i].x += 0.005f;
			m_pointSize[i].y += 0.005f;
		}
		// 既定フレームを超えたら
		if (m_shockCount[i] > MAX_SMOKE_COUNT)
		{
			m_pointSize[i] = SimpleMath::Vector2(0.3f, 0.2f);
		}
	}
}

/// <summary>
/// 襲撃可能かどうかを返す
/// </summary>
/// <param name="roadType">チェックする道の種類</param>
bool GameEnemyManager::IsAssault(int roadType)
{
	// 襲撃ポイントだったら襲撃、そうでなければ襲撃しない
	if (roadType == (int)GameRoad::RoadType::ASSAULT)
	{
		// 出現できる時間まで襲撃する
		m_spawnElapsedTime++;
		if (m_spawnElapsedTime > MAX_SPAWN_TIME)
		{
			m_spawnElapsedTime = 0;
		}
	}
	else
	{
		// カウントリセット
		m_respawnTime = 0;

		// 襲撃ポイントにいなければ襲撃しない
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			// 残った敵を片付ける
			if (mp_enemy[i]->GetState())
			{
				// 演出して片付ける
				mp_enemy[i]->SetState(false);
				ShockEnemy(i);
			}

		}

		// 襲撃不可
		m_dengerousDirLR = DANGERDIRECTION::DIR_NONE;
		return false;
	}

	return true;
}

/// <summary>
/// 敵生成管理
/// </summary>
/// <param name="assultP">襲撃ポイント</param>
/// <param name="playerPos">プレイヤーの位置</param>
/// <param name="chestEntryPos">チェストから出現する位置(配列)</param>
/// <param name="isChestOpen">チェストが開いているか(配列)</param>
void GameEnemyManager::CreateEnemy(int assultP, DirectX::SimpleMath::Vector3& playerPos, SimpleMath::Vector3* chestEntryPos, float chestHeight, std::array<bool, 3> isChestOpen)
{
	// 生成時間になったら敵の設定をする
	if (assultP == 1 && m_respawnTime % RESPAWN_NEED_TIME != 0)return;
	if (assultP == 2)
	{
		for (int i = 0; i < (int)GameDecorateObject::MAX_CHEST_NUM; i++)
		{
			// 生成できる場所があったら
			if (!m_openChestCreateFlag[i])
			{
				// チェストが開いていたら
				if (isChestOpen[i])
				{
					// 生成できる
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
				// 再び生成できるようにカウントを進める
				m_chestCreateCount[i]++;
				// 生成できるカウントになったら
				if (m_chestCreateCount[i] > RESPAWN_NEED_TIME)
				{
					// 再び生成させる
					m_openChestCreateFlag[i] = false;
					// カウントをリセット
					m_chestCreateCount[i] = 0;
				}

				if (i != ((int)GameDecorateObject::MAX_CHEST_NUM - 1))
					continue;
				else
					// どの場所も生成できなかったらこれ以降の処理は行わない
					return;
			}
		}
	}
	if (assultP == 3 && m_respawnTime % 120 != 0)return;
	if (assultP == 4 && m_respawnTime % 120 != 0 || m_createFillFlag)return;

	// 敵の襲撃時間だったら更新する
	int posXTmp = 0;
	int posYTmp = 0;
	ADX2Le* adx2le = ADX2Le::GetInstance();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 敵生成～プレイヤーへ向ける処理
		if (!mp_enemy[i]->GetState())
		{
			// 生成数が目標に達したら
			if ((assultP == 3 && m_createCount >= (int)MAXCREATECOUNT::FOUR) || (assultP == 4 && m_createCount >= (int)MAXCREATECOUNT::SIXTEEN))
			{
				// カウントリセットして抜ける
				m_createCount = 0;
				m_respawnTime = 0;
				break;
			}

			// まだ出現出来たら出現準備
			mp_enemy[i]->SetState(true);

			// 場所設定
			int point = assultP - 1;
			switch (point)
			{
			case 0:
				mp_enemy[i]->SetPos(SimpleMath::Vector3(m_entryEnemyPos[point].x + (rand() % (int)m_entryEnemyDistribute[point].x - (m_entryEnemyDistribute[point].x / 2)),
														m_entryEnemyPos[point].y + (rand() % (int)m_entryEnemyDistribute[point].y),
														m_entryEnemyPos[point].z));

				// 敵生成SE
				adx2le->Play(3);
				break;
			case 1:
				for (int j = 0; j < (int)GameDecorateObject::MAX_CHEST_NUM; j++)
				{
					// 生成できるか判定
					int isCreate = rand() % CREATE_PROBABILITY;
					if (isCreate != 0)
					{
						if (!m_openChestCreateFlag[j] && isChestOpen[j])
						{
							mp_enemy[i]->SetPos(SimpleMath::Vector3(chestEntryPos[j].x,
																	chestEntryPos[j].y + chestHeight,
																	chestEntryPos[j].z));
							m_openChestCreateFlag[j] = true;

							// 敵生成SE
							adx2le->Play(3);
							break;
						}
					}
					else
					{
						// 仮にそこから生成した判定にしておく
						m_openChestCreateFlag[j] = true;
						// trueにしたstateをfalseに戻す
						mp_enemy[i]->SetState(false); 
						return;
					}
				}
				break;
			case 2:
				mp_enemy[i]->SetPos(SimpleMath::Vector3(m_entryEnemyPos[point].x ,
														m_entryEnemyPos[point].y + posYTmp * (mp_enemy[i]->GetSize()*2.0f),
														m_entryEnemyPos[point].z));
				// カウント
				posYTmp++;
				// 生成数をカウント
				m_createCount++; 

				// 敵生成SE
				adx2le->Play(3);

				break;
			case 3:
				mp_enemy[i]->SetPos(SimpleMath::Vector3(m_entryEnemyPos[point].x + posXTmp * (mp_enemy[i]->GetSize()*3.0f),
														m_entryEnemyPos[point].y + posYTmp * (mp_enemy[i]->GetSize()*3.0f),
														m_entryEnemyPos[point].z));
				// 敵を並べるためのカウント
				posXTmp++;
				if (posXTmp == (int)MAXCREATECOUNT::FOUR)
				{
					posXTmp = 0;
					posYTmp++;
				}
				// 生成数をカウント
				m_createCount++;
				m_createFillFlag = true;

				// 敵生成SE
				adx2le->Play(3);

				break;
			default:
				break;
			}
			
			// 体力設定
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

			// プレイヤーの位置
			SimpleMath::Vector3 pointA = playerPos;
			// 敵の位置
			SimpleMath::Vector3 pointB = mp_enemy[i]->GetPos();
			// プレイヤーから敵に向かうベクトル
			SimpleMath::Vector3 subVector = pointA - pointB;

			// θを求める
			float sita = std::atan2(subVector.x, subVector.z);
			// 算出した値で回転しプレイヤーに向ける
			mp_enemy[i]->SetRotateY(sita);

			if ((assultP == 1 || assultP == 2))
			break;
		}
	}
}

/// <summary>
/// 敵移動管理
/// </summary>
/// <param name="timer">経過時間</param>
/// <param name="playerPos">プレイヤーの位置</param>
/// <param name="cameraDir">プレイヤーの向き(カメラの向き)</param>
void GameEnemyManager::MoveEnemy(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3& cameraDir, float& waveValue)
{
	// 敵移動
	int initID = -1;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// 敵が生存していたら
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

			// ふわふわさせるためにサイン波を使用
			float sinWave = sin(waveValue + float(i*2)) * 2.0f;
			// 速度設定
			mp_enemy[i]->SetVel(SimpleMath::Vector3((playerPos.x - mp_enemy[i]->GetPos().x) / controlSpeed,
												   ((playerPos.y - mp_enemy[i]->GetPos().y) + sinWave) / controlSpeed,
													(playerPos.z - mp_enemy[i]->GetPos().z) / controlSpeed));

			// 敵更新
			mp_enemy[i]->Update(timer);

			// HPバー表示位置設定
			m_hpPos[i] = mp_enemy[i]->GetPos();
			m_hpPos[i].y -= mp_enemy[i]->GetSize() /2.0f;

			m_compereLength[i] = (mp_enemy[i]->GetPos().x - playerPos.x)*(mp_enemy[i]->GetPos().x - playerPos.x) +
								 (mp_enemy[i]->GetPos().y - playerPos.y)*(mp_enemy[i]->GetPos().y - playerPos.y) +
								 (mp_enemy[i]->GetPos().z - playerPos.z)*(mp_enemy[i]->GetPos().z - playerPos.z);
			// 危険サインを出せる位置にまで来たら
			if (BASE_LENGTH * BASE_LENGTH > m_compereLength[i] * m_compereLength[i])
			{
				// 最初だけ比較用に保存しておく
				if (initID == -1)
				{
					initID = i;
					m_lengthTmp = initID;
				}
				// 一番近い敵の配列番号を保存
				if (m_compereLength[i] <= m_compereLength[m_lengthTmp])
				{
					m_lengthTmp = i;
				}

				// 外積を使ってプレイヤーから見てどちら側にいるかを判定
				SimpleMath::Vector3 pointA = playerPos;
				SimpleMath::Vector3 camDir = cameraDir;
				SimpleMath::Vector3 pointB = pointA + camDir;
				SimpleMath::Vector3 pointC = mp_enemy[m_lengthTmp]->GetPos();

				float p = (pointB.x - pointA.x)*(pointC.z - pointA.z) -
						  (pointB.z - pointA.z)*(pointC.x - pointA.x);

				// 左右判定
				if (p > 0.0f)m_dengerousDirLR = DANGERDIRECTION::DIR_RIGHT;
				else if (p < 0.0f) m_dengerousDirLR = DANGERDIRECTION::DIR_LEFT;
				// ほぼ真ん中に居たらサインを表示しない
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
/// HP表示
/// </summary>
/// <param name="matrixManager">行列管理オブジェクト</param>
/// <param name="world">ワールド行列</param>
/// <param name="enemyID">敵のID(タイプ)</param>
void GameEnemyManager::DrawHP(MatrixManager * matrixManager, DirectX::SimpleMath::Matrix & world, int enemyID)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// 頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  0.1f,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, 0.1f,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.5f, -0.1f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.5f,  -0.1f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
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
	m_batchEffect->SetFogEnabled(true);
	m_batchEffect->SetFogStart(4.0f);
	m_batchEffect->SetFogEnd(10.0f);
	m_batchEffect->SetFogColor(Colors::Black);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	switch (mp_enemy[enemyID]->GetType())
	{
	// 通常敵の場合
	case GameEnemy::EnemyType::NORMAL:
		if(mp_enemy[enemyID]->GetHP() == 2)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_MAX].Get());
		else if(mp_enemy[enemyID]->GetHP() == 1)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_NORMAL_DAMAGE].Get());
		else 
			// 無効なHP
			throw std::range_error("Invalid HP.");
		break;
	// パワー敵の場合
	case GameEnemy::EnemyType::POWER:
		if (mp_enemy[enemyID]->GetHP() == 3)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_MAX].Get());
		else if (mp_enemy[enemyID]->GetHP() == 2)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_POWER_DAMAGE].Get());
		else if (mp_enemy[enemyID]->GetHP() == 1)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_POWER_CRITICAL].Get());
		else
			// 無効なHP
			throw std::range_error("Invalid HP.");
		break;
	// スピード敵の場合
	case GameEnemy::EnemyType::SPEED:
		if (mp_enemy[enemyID]->GetHP() == 1)
			m_batchEffect->SetTexture(m_textureHP[ENEMYHP::HP_MAX].Get());
		else
			// 無効なHP
			throw std::range_error("Invalid HP.");
		break;
	default:
		break;
	}
	
	m_batchEffect->SetAlpha(1.0f);
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

/// <summary>
/// ヒットエフェクト表示
/// </summary>
/// <param name="matrixManager">行列管理オブジェクト</param>
/// <param name="world">ワールド行列</param>
/// <param name="enemyEffectState">敵のヒットエフェクトの状態</param>
void GameEnemyManager::DrawHitEffect(MatrixManager * matrixManager, DirectX::SimpleMath::Matrix & world, ENEMYHITEFFECT enemyEffectState)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// 頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(0.9f,  0.9f,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.9f, 0.9f,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-0.9f, -0.9f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(0.9f,  -0.9f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
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
	m_batchEffect->SetFogEnabled(true);
	m_batchEffect->SetFogStart(4.0f);
	m_batchEffect->SetFogEnd(10.0f);
	m_batchEffect->SetFogColor(Colors::Black);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	switch (enemyEffectState)
	{
		// エフェクト一段階目の場合
	case GameEnemyManager::ENEMYHITEFFECT::EFFECT_FIRST:
		m_batchEffect->SetTexture(m_textureHitEffect[ENEMYHITEFFECT::EFFECT_FIRST].Get());
		break;
		// エフェクト二段階目の場合
	case GameEnemyManager::ENEMYHITEFFECT::EFFECT_SECOND:
		m_batchEffect->SetTexture(m_textureHitEffect[ENEMYHITEFFECT::EFFECT_SECOND].Get());
		break;
	default:
		// 無効な値
		throw std::range_error("Invalid Value.");
		break;
	}

	m_batchEffect->SetAlpha(1.0f);
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

/// <summary>
/// 得点表示
/// </summary>
/// <param name="matrixManager">行列管理オブジェクト</param>
/// <param name="world">ワールド行列</param>
/// <param name="enemyID">敵のID(タイプ)</param>
void GameEnemyManager::DrawPoint(MatrixManager * matrixManager, DirectX::SimpleMath::Matrix & world, int enemyID, SimpleMath::Vector2 &size)
{
	auto m_states = CommonStateManager::SingletonGetInstance().GetStates();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// 頂点情報
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(size.x,  size.y,  0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-size.x, size.y,  0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-size.x, -size.y, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(size.x,  -size.y, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
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
	m_batchEffect->SetFogEnabled(true);
	m_batchEffect->SetFogStart(4.0f);
	m_batchEffect->SetFogEnd(10.0f);
	m_batchEffect->SetFogColor(Colors::Black);
	m_batchEffect->SetWorld(world);
	m_batchEffect->SetView(matrixManager->GetView());
	m_batchEffect->SetProjection(matrixManager->GetProjection());
	switch (mp_enemy[enemyID]->GetType())
	{
		// 通常敵の場合
	case GameEnemy::EnemyType::NORMAL:
		m_batchEffect->SetTexture(m_texturePoint[ENEMYTYPE::NORMAL_TYPE].Get());
		break;
		// パワー敵の場合
	case GameEnemy::EnemyType::POWER:
		m_batchEffect->SetTexture(m_texturePoint[ENEMYTYPE::POWER_TYPE].Get());
		break;
		// スピード敵の場合
	case GameEnemy::EnemyType::SPEED:
		m_batchEffect->SetTexture(m_texturePoint[ENEMYTYPE::SPEED_TYPE].Get());
		break;
	default:
		break;
	}

	m_batchEffect->SetAlpha(1.0f);
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

/// <summary>
/// やられ演出
/// </summary>
/// <param name="matrixManager">行列管理変数</param>
/// <param name="world">ワールド座標</param>
void GameEnemyManager::DrawSmoke(MatrixManager* matrixManager, SimpleMath::Matrix &world, float &drawAlpha)
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
	m_batchEffect->SetAlpha(drawAlpha);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	// 不透明部分を描画
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
	// 不透明以外の半透明部分を描画する設定
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetAlpha(drawAlpha);
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
