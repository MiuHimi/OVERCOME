//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/09
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

class MatrixManager
{
// メンバー変数
public:

private:
	static DirectX::SimpleMath::Matrix              m_world;                // ワールド行列
	static DirectX::SimpleMath::Matrix              m_view;                 // ビュー行列
	static DirectX::SimpleMath::Matrix              m_projection;           // 射影行列

// メンバー関数
public:
	// Setter
	static void SetWorldMatrix(DirectX::SimpleMath::Matrix world)           { m_world = world; }
	static void SetViewMatrix(DirectX::SimpleMath::Matrix view)             { m_view = view; }
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection) { m_projection = projection; }

	// Getter
	static DirectX::SimpleMath::Matrix GetWorldMatrix()                     { return m_world; }
	static DirectX::SimpleMath::Matrix GetViewMatrix()                      { return m_view; }
	static DirectX::SimpleMath::Matrix GetProjectionMatrix()                { return m_projection; }

private:

};