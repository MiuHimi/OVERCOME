//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.h
// Summary. GameDecorateObjectClass
// Date.    2019/10/23
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <Model.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameDecorateObject
{
// メンバー変数
public:

private:
	DirectX::SimpleMath::Matrix				m_rotationY;						// Y軸回転
	DirectX::SimpleMath::Matrix				m_world;						// ワールド座標

	DirectX::SimpleMath::Vector3			m_trans[6];						// ワールド座標

	std::unique_ptr<DirectX::Model>			mp_modelEnemyDoor[6];			// 敵が出てくるドアモデルオブジェクト

// メンバー関数
public:
	// コンストラクタ
	GameDecorateObject();
	// デストラクタ
	~GameDecorateObject();

	/// <summary>
	/// 生成
	/// </summary>
	void Create();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>終了状態</returns>
	bool Update(int in);
	/// <summary>
	/// 描画
	/// </summary>
	void Render(MatrixManager* matrixManager);
	/// <summary>
	/// 廃棄処理
	/// </summary>
	void Depose();

private:

};