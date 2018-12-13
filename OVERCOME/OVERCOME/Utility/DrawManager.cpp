//////////////////////////////////////////////////////////////
// File.    DrawManager.cpp
// Summary. DrawManagerClass
// Date.    2018/10/18
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <iostream>
#include <fstream>
#include <sstream>
#include <wchar.h>
#include <shlwapi.h>
#include <tchar.h>

#include "../pch.h"
#include "DrawManager.h"
#include "DeviceResources.h"

//#include "FilePath.h"

#pragma comment(lib, "shlwapi.lib")

/// <summary>
/// コンストラクタ
/// </summary>
DrawManager::DrawManager()
{
	// スプライトバッチの作成
	m_sprites = std::make_unique<DirectX::SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());
	// コモンステートの作成 
	m_states = std::make_unique<DirectX::CommonStates>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
}
/// <summary>
/// デストラクタ
/// </summary>
DrawManager::~DrawManager()
{
}

/// <summary>
/// スプライトの描画
/// </summary>
/// <param name="texture"></param>
/// <param name="pos"></param>
void DrawManager::Draw(ID3D11ShaderResourceView * texture, DirectX::SimpleMath::Vector2 pos)
{
	// スプライトの描画
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(texture, pos);
	m_sprites->End();
}
/// <summary>
/// スプライトの描画(切り取りなし)
/// </summary>
/// <param name="texture"></param>
/// <param name="pos"></param>
/// <param name="sourceRectangle"></param>
void DrawManager::DrawRect(ID3D11ShaderResourceView * texture, DirectX::SimpleMath::Vector2 pos, RECT const * sourceRectangle)
{
	// スプライトの描画
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(texture, pos, sourceRectangle);
	m_sprites->End();
}
/// <summary>
/// スプライトの描画(透明度変更)
/// </summary>
/// <param name="texture"></param>
/// <param name="pos"></param>
/// <param name="color"></param>
void DrawManager::DrawAlpha(ID3D11ShaderResourceView * texture, DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Color color)
{
	// スプライトの描画
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(texture, pos, color);
	m_sprites->End();
}

/// <summary>
/// テクスチャの描画
/// </summary>
/// <param name="textureName">テクスチャの名前</param>
/// <param name="pos">描画する座標</param>
/*void DrawManager::Draw(char* textureName, DirectX::SimpleMath::Vector2 pos)
{
	// ファイルパス変数の初期化
	FilePath::ScenePath sceneFilePath[FilePath::ScenePath::NumScene];
	for (int i = 0; i < FilePath::ScenePath::NumScene; i++)
	{
		sceneFilePath[i] = FilePath::InitializeSceneFilePath(i);
	}
	FilePath::ObjectPath objectFilePath[FilePath::ObjectPath::NumObject];
	for (int i = 0; i < FilePath::ObjectPath::NumObject; i++)
	{
		objectFilePath[i] = FilePath::InitializeObjectFilePath(i);
	}

	std::wstring filePath;

	// テクスチャファイルのパス検索
	for (int i = 0; i < FilePath::ScenePath::NumScene; ++i)
	{
		int retval = 0;

		// シーンパスの設定
		char* scenePath;
		scenePath = FilePath::SetSceneFilePath(sceneFilePath[i]);

		for (int i = 0; i < FilePath::ObjectPath::NumObject; ++i)
		{
			// オブジェクトパスの設定
			char* objectPath;
			objectPath = FilePath::SetObjectFilePath(objectFilePath[i]);

			// 文字列を連結しパスを生成
			std::wstringstream ss;
			ss << "Resources\\" << scenePath << "\\" << objectPath << "\\" << textureName;
			filePath = ss.str();

			// ファイルを発見したらループを抜ける
			retval = PathFileExists(filePath.c_str());
			if (retval == 1)  break;
		}
		if (retval == 1)  break;

		if (i == FilePath::ScenePath::NumScene -1 && retval == 0)
		{
			// 全ファイル探してもテクスチャが見つからない場合はエラー
			throw std::range_error("Can't fount resource path.");
		}
	}

	// テクスチャの読み込み
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), filePath.c_str(), nullptr, m_texture.GetAddressOf());

	if (!m_texture)
	{
		// ファイル読み込み失敗
		throw std::range_error("Read failure.");
	}

	// スプライトの描画
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(m_texture.Get(), pos);
	m_sprites->End();
}*/
/// <summary>
/// テクスチャの描画
/// </summary>
/// <param name="textureName">テクスチャの名前</param>
/// <param name="pos">描画する座標</param>
/// <param name="sourceRectangle">切り取り座標</param>
/*void DrawManager::DrawRect(char * textureName, DirectX::SimpleMath::Vector2 pos, RECT const * sourceRectangle)
{
	// ファイルパス変数の初期化
	FilePath::ScenePath sceneFilePath[FilePath::ScenePath::NumScene];
	for (int i = 0; i < FilePath::ScenePath::NumScene; i++)
	{
		sceneFilePath[i] = FilePath::InitializeSceneFilePath(i);
	}
	FilePath::ObjectPath objectFilePath[FilePath::ObjectPath::NumObject];
	for (int i = 0; i < FilePath::ObjectPath::NumObject; i++)
	{
		objectFilePath[i] = FilePath::InitializeObjectFilePath(i);
	}

	std::wstring filePath;

	// テクスチャファイルのパス検索
	for (int i = 0; i < FilePath::ScenePath::NumScene; ++i)
	{
		int retval = 0;

		// シーンパスの設定
		char* scenePath;
		switch (sceneFilePath[i])
		{
		case FilePath::ScenePath::LogoScene:  scenePath = "LogoScene";  break;
		case FilePath::ScenePath::TitleScene: scenePath = "TitleScene"; break;
		case FilePath::ScenePath::PlayScene:  scenePath = "PlayScene";  break;
		}
		for (int i = 0; i < FilePath::ObjectPath::NumObject; ++i)
		{
			// オブジェクトパスの設定
			char* objectPath;
			switch (objectFilePath[i])
			{
			case FilePath::ObjectPath::Character: objectPath = "Character"; break;
			case FilePath::ObjectPath::UI:        objectPath = "UI";        break;
			case FilePath::ObjectPath::Animation: objectPath = "Animation"; break;
			}

			// 文字列を連結しパスを生成
			std::wstringstream ss;
			ss << "Resources\\" << scenePath << "\\" << objectPath << "\\" << textureName;
			filePath = ss.str();

			// ファイルを発見したらループを抜ける
			retval = PathFileExists(filePath.c_str());
			if (retval == 1)  break;
		}
		if (retval == 1)  break;

		if (i == FilePath::ScenePath::NumScene - 1 && retval == 0)
		{
			// 全ファイル探してもテクスチャが見つからない場合はエラー
			throw std::range_error("Can't fount resource path.");
		}
	}

	// テクスチャの読み込み
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), filePath.c_str(), nullptr, m_texture.GetAddressOf());

	if (!m_texture)
	{
		// ファイル読み込み失敗
		throw std::range_error("Read failure.");
	}

	// スプライトの描画
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
	m_sprites->Draw(m_texture.Get(), pos, sourceRectangle);
	m_sprites->End();
}*/
