//////////////////////////////////////////////////////////////
// File.    KeyManager.h
// Summary. KeyManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ


class KeyManager
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	//static int   m_aTrigCount[0xff + 1];  //< VK_xxの数だけ用意

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	KeyManager();
	// デストラクタ
	virtual ~KeyManager();

	// キー入力情報をセット
	static void SetTrigger(int key);

	// キー入力情報をリセット、毎フレーム呼び出し
	static void AllClear();

	// キー入力情報を取得
	static bool IsTrigger(int key);
};
