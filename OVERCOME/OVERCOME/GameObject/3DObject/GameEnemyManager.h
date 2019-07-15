//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.h
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"

#include <vector>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "GameEnemy.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class GameEnemy;
class GameCamera;
class MatrixManager;
class Player;
class GameEnemyManager
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	static const int			  MAXENEMYNUM = 100;		 // 最大敵数
	static const int			  MAXALIVEDIST;				 // 最大生存距離

	static const int              SPAWNTIME;                 // 敵が出現していられる時間(フレーム)
	int							  m_spawnElapsedTime;		 // 敵が出現してからの経過時間(フレーム)

	static const int			  NEEDRESPAWNTIME;			 // リスポーンに必要な時間(フレーム)
	int                           m_respawnTime;             // リスポーン時間(フレーム)
	
	GameEnemy*                    mp_enemy[MAXENEMYNUM];     // 敵管理
	DirectX::SimpleMath::Vector3  m_shockPos[MAXENEMYNUM];   // エフェクトが出る位置
	int							  m_shockCount[MAXENEMYNUM]; // エフェクトが出てからのカウント
	Player*                       mp_player;				 // プレイヤーオブジェクト
	std::unique_ptr<GameCamera>   mp_gameCamera;             // カメラオブジェクト

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousV;       // テクスチャハンドル(危険サイン)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureSmoke;			 // テクスチャハンドル(やられ演出用煙)

	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect; // エフェクト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>
								  m_batch;					 // プリミティブバッチ
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; // 入力レイアウト

	enum DIRECTION
	{
		FRONT,
		BACK,
		RIGHT,
		LEFT,

		NONE
	};
	DIRECTION                    m_dengerousDirFB;
	DIRECTION                    m_dengerousDirLR;

	double                       m_compereLength[MAXENEMYNUM]; // 距離を比較
	int                          m_lengthTmp;                 // プレイヤーに最短距離の敵を記憶

	bool                         m_danger;                   // 敵がプレイヤーにとって危険な位置にいたらフラグが立つ

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameEnemyManager();
	// デストラクタ
	~GameEnemyManager();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer, Player* player, int playerNowRoadType, int assaultPoint);
	// 描画
	void Render(MatrixManager* matrixManager, DirectX::SimpleMath::Vector3 eyePos);
	// 廃棄処理
	void Depose();

	//-----------------------------------Getter-----------------------------------//

	int GetMaxEnemyNum()                       { return MAXENEMYNUM; }
	DirectX::SimpleMath::Vector3 GetPos(int i) { return mp_enemy[i]->GetPos(); }
	bool GetEnemyState(int i)				   { return mp_enemy[i]->GetState(); }
	Collision::Sphere GetEnemyCollide(int i)   { return mp_enemy[i]->GetCollision(); }

	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	void SetEnemyState(int i, bool flag) {mp_enemy[i]->SetState(flag); }
	//----------------------------------------------------------------------------//

	// やられ演出設定
	void ShockEnemy(int i);
	
private:
	// やられ演出
	void DrawSmoke(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world,int &drawAlpha);

};