//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.h
// Summary. GameEnemyManagerClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <vector>
#include <array>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "GameEnemy.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class GameEnemy;
class MatrixManager;
class GameEnemyManager
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:
	// 危険方向
	enum ENEMYTYPE
	{
		SPEED_TYPE,
		NORMAL_TYPE,
		POWER_TYPE,

		MAX_TYPE
	};
	enum DANGERDIRECTION
	{
		DIR_NONE,

		DIR_FRONT,
		DIR_BACK,
		DIR_RIGHT,
		DIR_LEFT,
	};
	enum ENTRYCOUNT
	{
		NONE,

		ENTRY_X,
		DISTRIBUTE_X,
		END_X,
		ENTRY_Y,
		DISTRIBUTE_Y,
		END_Y,
		ENTRY_Z,
		DISTRIBUTE_Z,
		END_Z,
	};
	enum ENEMYHP
	{
		HP_MAX,
		HP_NORMAL_DAMAGE,
		HP_POWER_DAMAGE,
		HP_POWER_CRITICAL,
		HP_NUM,
	};
	enum ENEMYPOWER
	{
		NONE_DAMAGE,
		SPEED_ENEMY_DAMAGE,
		NORMAL_ENEMY_DAMAGE,
		POWER_ENEMY_DAMAGE,
		MAX_DAMAGE,
	};
	enum ENEMYHITEFFECT
	{
		EFFECT_FIRST,
		EFFECT_SECOND,

		MAX_EFFECT
	};
	enum MAXCREATECOUNT
	{
		DONTCREATE,
		FOUR = 4,
		SIXTEEN = 16
	};

private:
	static const int			  MAX_ENEMY = 100;			  // 最大敵数
	static const int			  MAX_ENTRY_POS = 10;		  // 最大敵出現場所数

	static const int			  MAX_SPAWN_TIME;			  // 敵が出現できる最大時間(フレーム数)
	int							  m_spawnElapsedTime;		  // 敵が出現してからの経過時間(フレーム数)

	static const int			  CREATE_PROBABILITY;		  // 生成確率(rand()と共に使用)
	
	bool						  m_createFillFlag;			  // 敵の生成で出現限度が満たされたらtrueに
	bool						  m_openChestCreateFlag[3];	  // チェストから出現(生成)したらtrueに
	int							  m_chestCreateCount[3];	  // 生成したチェストから再び生成させるためのカウント
	int							  m_createCount;			  // 敵の生成数をカウント
	DirectX::SimpleMath::Vector3  m_entryEnemyPosTmp;		  // 敵の出現仮位置

	static const int			  RESPAWN_NEED_TIME;		  // リスポーンに必要な時間(フレーム数)
	int                           m_respawnTime;              // リスポーン時間(フレーム数)

	DirectX::SimpleMath::Vector3  m_entryEnemyPos[MAX_ENTRY_POS];
															  // 敵出現位置
	DirectX::SimpleMath::Vector3  m_entryEnemyDistribute[MAX_ENTRY_POS];
															  // 敵出現位置分布

	static const float			  CONTROL_NORMAL_VELOCITY;	  // 速度調整(通常敵用)
	static const float			  CONTROL_POWER_VELOCITY;	  // 速度調整(パワー敵用)
	static const float			  CONTROL_SPEED_VELOCITY;	  // 速度調整(スピード敵用)

	GameEnemy*					  mp_enemy[MAX_ENEMY];		  // 敵オブジェクト
	DirectX::SimpleMath::Vector3  m_hpPos[MAX_ENEMY];		  // HPを表示する位置
	DirectX::SimpleMath::Vector3  m_shockPos[MAX_ENEMY];	  // 煙が出る位置
	static const float			  SMOKE_SPEED;				  // 煙の昇る速さ
	static const int			  MAX_SMOKE_COUNT;			  // 煙の昇る速さ
	ENEMYHITEFFECT				  m_hitEffectState[MAX_ENEMY];
															  // ヒットエフェクトの状態
	DirectX::SimpleMath::Vector3  m_hitPos[MAX_ENEMY];		  // ヒットエフェクトが出る位置
	int							  m_hitAnimationCount[MAX_ENEMY];
															  // エフェクトアニメーション用のカウント
	int							  m_shockCount[MAX_ENEMY];	  // エフェクトが出てからのカウント
	DirectX::SimpleMath::Vector3  m_pointPos[MAX_ENEMY];	  // 得点が出る位置
	DirectX::SimpleMath::Vector2  m_pointSize[MAX_ENEMY];	  // 得点の大きさ

	DANGERDIRECTION				  m_dengerousDirLR;			  // 危険方向

	static const int			  BASE_LENGTH;				  // 基準となる距離
	float						  m_compereLength[MAX_ENEMY]; // 距離を比較
	int							  m_lengthTmp;				  // プレイヤーとの最短距離の敵を記憶

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureHP[HP_NUM];		  // テクスチャハンドル(HPバー)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_texturePoint[MAX_TYPE];	  // テクスチャハンドル(得点)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureHitEffect[MAX_EFFECT]; // テクスチャハンドル(ヒットエフェクト)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureSmoke;			  // テクスチャハンドル(やられ演出用煙)

	std::unique_ptr<DirectX::AlphaTestEffect> 
								  m_batchEffect;			  // エフェクト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> 
								  m_batch;					  // プリミティブバッチ
	Microsoft::WRL::ComPtr<ID3D11InputLayout> 
								  m_inputLayout;			  // 入力レイアウト

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameEnemyManager();
	// デストラクタ
	~GameEnemyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 生成
	/// </summary>
	void Create();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">経過時間</param>
	/// <param name="playerPos">プレイヤーの位置</param>
	/// <param name="roadType">道の種類</param>
	/// <param name="assaultPoint">道の番号(何番目かを取得)</param>
	/// <param name="cameraDir">プレイヤーの向き(カメラの向き)</param>
	/// <returns>終了状態</returns>
	bool Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos,
				int roadType, int assaultPoint, 
				DirectX::SimpleMath::Vector3& cameraDir, 
				DirectX::SimpleMath::Vector3* chestEntryPos, float chestHeight, std::array<bool, 3> isChestOpen);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="matrixManager">行列管理</param>
	/// <param name="eyePos">プレイヤーの視点</param>
	void Render(MatrixManager* matrixManager, DirectX::SimpleMath::Vector3 eyePos);
	/// <summary>
	/// 廃棄処理
	/// </summary>
	void Depose();

	//-----------------------------------Getter-----------------------------------//
	// 敵の最大数を取得
	int							   GetMaxEnemyNum()       { return MAX_ENEMY; }
	// 敵の位置を取得
	DirectX::SimpleMath::Vector3   GetPos(int i)		  { return mp_enemy[i]->GetPos(); }
	// 敵の状態を取得
	bool						   GetEnemyState(int i)   { return mp_enemy[i]->GetState(); }
	// 敵の種類を取得
	GameEnemy::EnemyType		   GetEnemyType(int i)    { return mp_enemy[i]->GetType(); }
	// 敵のHPを取得
	int							   GetEnemyHP(int i)	  { return mp_enemy[i]->GetHP(); }
	// 敵の衝突判定情報を取得
	Collision::Sphere			   GetEnemyCollide(int i) { return mp_enemy[i]->GetCollision(); }
	// プレイヤーから見た敵の方向を取得
	DANGERDIRECTION				   GetDangerDir()         { return m_dengerousDirLR; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//
	// 敵の状態を設定
	void	SetEnemyState(int i, bool flag)		{mp_enemy[i]->SetState(flag); }
	// 敵のHPを設定
	void	SetEnemyHP(int i, int hp)			{ mp_enemy[i]->SetHP(hp); }
	// 接触判定を設定
	void	SetHit(int i, bool flag, DirectX::SimpleMath::Vector3 hitPos)			{ mp_enemy[i]->SetHit(flag); m_hitPos[i] = /*mp_enemy[i]->GetPos()*/hitPos; }
	//----------------------------------------------------------------------------//

	/// <summary>
	/// やられ演出設定
	/// </summary>
	/// <param name="i">敵の配列</param>
	void ShockEnemy(int i);
	
private:
	//-----------------------------------更新関数-----------------------------------//
	/// <summary>
	/// やられ演出更新
	/// </summary>
	void UpdateSmoke();

	/// <summary>
	/// 得点演出更新
	/// </summary>
	void UpdatePoint();

	/// <summary>
	/// 襲撃可能かどうかを返す
	/// </summary>
	/// <param name="roadType">チェックする道の種類</param>
	/// <returns>true=襲撃可能、false=襲撃不可</returns>
	bool IsAssault(int roadType);

	/// <summary>
	/// 敵生成管理
	/// </summary>
	/// <param name="assultP">襲撃ポイント</param>
	/// <param name="playerPos">プレイヤーの位置</param>
	void CreateEnemy(int assultP, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3* chestEntryPos, float chestHeight, std::array<bool, 3> isChestOpen);

	/// <summary>
	/// 敵移動管理
	/// </summary>
	/// <param name="timer">経過時間</param>
	/// <param name="playerPos">プレイヤーの位置</param>
	/// <param name="cameraDir">プレイヤーの向き(カメラの向き)</param>
	/// <param name="waveValue">サイン波で動かす用の値</param>
	void MoveEnemy(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& playerPos, DirectX::SimpleMath::Vector3& cameraDir, float& waveValue);
	//------------------------------------------------------------------------------//

	//-----------------------------------描画関数-----------------------------------//
	/// <summary>
	/// HP表示
	/// </summary>
	/// <param name="matrixManager">行列管理オブジェクト</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="enemyID">敵のID(タイプ)</param>
	void DrawHP(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world, int enemyID);
	/// <summary>
	/// ヒットエフェクト表示
	/// </summary>
	/// <param name="matrixManager">行列管理オブジェクト</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="enemyEffectState">敵のヒットエフェクトの状態</param>
	void DrawHitEffect(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world, ENEMYHITEFFECT enemyEffectState);
	/// <summary>
	/// 得点表示
	/// </summary>
	/// <param name="matrixManager">行列管理オブジェクト</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="enemyID">敵のID(タイプ)</param>
	void DrawPoint(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world, int enemyID, DirectX::SimpleMath::Vector2 &size);
	/// <summary>
	/// やられ演出表示
	/// </summary>
	/// <param name="matrixManager">行列管理オブジェクト</param>
	/// <param name="world">ワールド行列</param>
	/// <param name="drawAlpha">透明度</param>
	void DrawSmoke(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world,float &drawAlpha);
	//------------------------------------------------------------------------------//

};