//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "SingletonBase.h"

class MatrixManager : public SingletonBase<MatrixManager>
{
// メンバー変数
public:

private:
	DirectX::SimpleMath::Matrix   m_view;         // ビュー行列
	DirectX::SimpleMath::Matrix   m_projection;   // ビュー行列

// メンバー関数
public:
	// デストラクタ
	~MatrixManager();

	// フレンド関数
	friend SingletonBase<MatrixManager>;

	// Getter
	DirectX::SimpleMath::Matrix& GetView();
	DirectX::SimpleMath::Matrix& GetProjection();

	// Setter
	DirectX::SimpleMath::Matrix SetView(DirectX::SimpleMath::Matrix view);
	DirectX::SimpleMath::Matrix SetProjection(DirectX::SimpleMath::Matrix projection);

private:
	// コンストラクタ
	MatrixManager();

};