//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/31
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
	DirectX::SimpleMath::Matrix   m_projection;   // 射影行列

// メンバー関数
public:
	// デストラクタ
	~MatrixManager();

	// フレンド関数
	friend SingletonBase<MatrixManager>;

	// リセット
	void ResetMatrix();
	

	// Getter
	DirectX::SimpleMath::Matrix& GetView()       { return m_view; }
	DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// Setter
	void SetView(DirectX::SimpleMath::Matrix& view)             { m_view = view; }
	void SetProjection(DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }

private:
	// コンストラクタ
	MatrixManager();

};