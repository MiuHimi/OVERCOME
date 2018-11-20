//////////////////////////////////////////////////////////////
// File.    GameBulletManager.h
// Summary. GameBulletManagerClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

#include <vector>

#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "GameBullet.h"
#include "../ExclusiveGameObject/CollisionSphere.h"

class GameBullet;
class GameBulletManager : public CollisionSphere
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	std::vector<GameBullet*>     mp_bullet;                  // 弾管理配列

	static const int             m_maxBulletNum = 5;         // 最大弾数
	static const int             m_needReloadTime = 60;      // 次弾発射までの時間(フレーム)

	int                          m_reloadTime;               // 次弾装填時間(フレーム)
	bool                         m_shootingFlag;             // 発射済みフラグ(m_reloadTimeカウント開始)

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameBulletManager();
	// デストラクタ
	~GameBulletManager();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3 ShootPos, DirectX::SimpleMath::Vector3 cameraDir);
	// 描画
	void Render();
	// 廃棄処理
	void Depose();


private:


};