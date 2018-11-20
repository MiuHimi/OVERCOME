//////////////////////////////////////////////////////////////
// File.    GameDebug.h
// Summary. GameDebugClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include "../pch.h"
#include "SingletonBase.h"

// �f�o�b�O
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

class GameDebug : public SingletonBase<GameDebug>
{
// �����o�[�ϐ�
public:

private:
	std::unique_ptr<DirectX::SpriteBatch>   m_sprites;       // �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteFont>    m_font;          // �X�v���C�g�t�H���g

	struct DebugTextData                                     // �e�L�X�g���̍\����
	{
		std::wstring text;
		DirectX::SimpleMath::Vector2 pos;
		DebugTextData(std::wstring text, DirectX::SimpleMath::Vector2 pos)
		{
			this->text = text;
			this->pos = pos;
		}
	};

	std::list<DebugTextData> m_debugText;					 // �f�o�b�O�ŕ\������e�L�X�g���X�g

// �����o�[�֐�
public:
	~GameDebug();
	void Render();

	// �t�����h�֐�
	friend SingletonBase<GameDebug>;

	// �f�o�b�O�`��
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(float debugLen, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(DirectX::SimpleMath::Vector2 debugInf, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(char* debugLen, float debugInf, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugInf, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector3 debugInf, DirectX::SimpleMath::Vector2 debugPos);

private:
	GameDebug();

	//-�^�ϊ��֐�-------------------------------------

	std::wstring NumtoWstring(float num);
	std::wstring ChartoWstring(char* text);
	//------------------------------------------------
};
