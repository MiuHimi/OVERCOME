//////////////////////////////////////////////////////////////
// File.    CommonStateManager.h
// Summary. CommonStateManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

#include <CommonStates.h>

#include "SingletonBase.h"

class CommonStateManager : public SingletonBase<CommonStateManager>
{
// メンバー変数
public:
	std::unique_ptr<DirectX::CommonStates>   m_states;    // コモンステート

private:

// メンバー関数
public:
	// デストラクタ
	~CommonStateManager();

	// フレンド関数
	friend SingletonBase<CommonStateManager>;

	// コモンステートを取得する
	DirectX::CommonStates* GetStates();

private:
	// コンストラクタ
	CommonStateManager();

};
