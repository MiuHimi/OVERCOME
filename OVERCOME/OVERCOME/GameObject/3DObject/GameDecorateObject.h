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
	DirectX::SimpleMath::Matrix				m_world;						// ワールド座標

	static const int						MAX_DOOR_NUM = 6;				// ドアの数
	DirectX::SimpleMath::Matrix				m_doorRota[MAX_DOOR_NUM];		// ドアオブジェクトの回転行列
	DirectX::SimpleMath::Vector3			m_doorTrans[MAX_DOOR_NUM];		// ドアオブジェクトの移動行列
	bool									m_isMoveDoor[MAX_DOOR_NUM];		// ドアが動き出したかどうか
	bool									m_isShowDoor[MAX_DOOR_NUM];		// ドアを表示できる状態かどうか

	DirectX::SimpleMath::Matrix				m_chestRota[3];					// チェストオブジェクトの回転行列
	DirectX::SimpleMath::Vector3			m_chestTrans[3];				// チェストオブジェクトの移動行列
	DirectX::SimpleMath::Vector3			m_chestTempPos[3];				// チェストオブジェクトの基本位置
	int										m_shakeCount[3];				// チェストが振動するまでのカウント
	int										m_shakeVelCount[3];				// チェストがある方向に振動しているカウント
	int										m_shakeNeedCount[3];			// チェストが振動するまでに必要なカウント
	bool									m_isChestOpen[3];				// チェストが開閉しているかどうか
	static const int						SHAKE_COUNT;					// 一方向に振動するカウント
	static const DirectX::SimpleMath::Vector3
											SHAKE_VEL;						// 振動する方向

	std::unique_ptr<DirectX::Model>			mp_modelEnemyDoor[MAX_DOOR_NUM];// 敵が出てくるドアモデルオブジェクト
	std::unique_ptr<DirectX::Model>			mp_modelSmallRoom;				// 小部屋モデルオブジェクト
	std::unique_ptr<DirectX::Model>			mp_modelEnemyChestOpen[3];		// 敵が出てくるチェストモデルオブジェクト
	std::unique_ptr<DirectX::Model>			mp_modelEnemyChestClose[3];		// 敵が出てくるチェストモデルオブジェクト

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
	bool Update(int roadID);
	/// <summary>
	/// 描画
	/// </summary>
	void Render(MatrixManager* matrixManager);
	/// <summary>
	/// 廃棄処理
	/// </summary>
	void Depose();

private:
	/// <summary>
	/// 描画前にするワールド行列の設定
	/// </summary>
	/// <param name="rota">回転行列</param>
	/// <param name="trans">移動行列</param>
	DirectX::SimpleMath::Matrix SetWorldMatrix(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix rota, DirectX::SimpleMath::Matrix trans);

	/// <summary>
	/// 敵出現ドアの更新
	/// </summary>
	/// <param name="roadID">道ID</param>
	void UpdateEnemyDoor(int roadID);

	/// <summary>
	/// 敵出現チェストの更新
	/// </summary>
	/// <param name="roadID">道ID</param>
	void UpdateEnemyChest(int roadID);
};