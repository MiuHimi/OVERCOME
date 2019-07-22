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
	static const int              m_maxEnemyNum = 100;       // 最大敵数
	static const int              m_maxAliveDist = 90;		 // 

	static const int              m_needRespawnTime = 40;    // リスポーンに必要な時間(フレーム)
	int                           m_respawnTime;             // リスポーン時間(フレーム)

	GameEnemy*                    mp_enemy[m_maxEnemyNum];   // 敵管理
	DirectX::SimpleMath::Vector3  m_shockPos[m_maxEnemyNum]; // エフェクトが出る位置
	int							  m_shockCount[m_maxEnemyNum]; // エフェクトが出てからのカウント
	Player*                       mp_player;				 // プレイヤーオブジェクト

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousH;       // テクスチャハンドル(危険サイン横)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureDengerousV;       // テクスチャハンドル(危険サイン縦)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
								  m_textureSmoke;			 // テクスチャハンドル(やられ演出用煙)

	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect; // エフェクト
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;// プリミティブバッチ
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
	bool Update(DX::StepTimer const& timer, Player* player, int assaultPoint, DirectX::SimpleMath::Vector3& cameraDir);
	// 描画
	void Render(MatrixManager* matrixManager, DirectX::SimpleMath::Vector3 eyePos);
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

	// やられ演出設定
	void ShockEnemy(int i);
	
private:
	// やられ演出
	void DrawSmoke(MatrixManager* matrixManager, DirectX::SimpleMath::Matrix &world,int &drawAlpha);

};