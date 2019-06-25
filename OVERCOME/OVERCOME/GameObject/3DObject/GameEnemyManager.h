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
	static const int              m_maxEnemyNum = 100;       // 最大敵数
	static const int              m_maxAliveDist = 50;		 // 

	static const int              m_needRespawnTime = 90;    // リスポーンに必要な時間(フレーム)
	int                           m_respawnTime;             // リスポーン時間(フレーム)

	GameEnemy*                    mp_enemy[m_maxEnemyNum];   // 敵管理
	Player*                       mp_player;				 // プレイヤーオブジェクト
	std::unique_ptr<GameCamera>   mp_gameCamera;             // カメラオブジェクト

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousH;       // テクスチャハンドル(危険サイン横)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousV;       // テクスチャハンドル(危険サイン縦)

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

	double                       m_compereLength[m_maxEnemyNum]; // 距離を比較
	int                          m_lengthTmp;                 // プレイヤーに最短距離の敵を記憶

	bool                         m_assault;                   // 敵移動中

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
	bool Update(DX::StepTimer const& timer, Player* player);
	// 描画
	void Render(MatrixManager* matrixManager);
	// 廃棄処理
	void Depose();

	//-----------------------------------Getter-----------------------------------//

	int GetMaxEnemyNum()                       { return m_maxEnemyNum; }
	DirectX::SimpleMath::Vector3 GetPos(int i) { return mp_enemy[i]->GetPos(); }
	bool GetEnemyState(int i)				   { return mp_enemy[i]->GetState(); }
	Collision::Sphere GetEnemyCollide(int i)   { return mp_enemy[i]->GetCollision(); }

	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	void SetEnemyState(int i, bool flag) {mp_enemy[i]->SetState(flag); }
	//----------------------------------------------------------------------------//

private:


};