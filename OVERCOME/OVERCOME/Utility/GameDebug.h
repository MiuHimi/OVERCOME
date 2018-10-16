//////////////////////////////////////////////////////////////
// File.    GameDebug.h
// Summary. GameDebugClass
// Date.    2018/10/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <iostream>
#include <fstream>
#include <sstream>

#include "../pch.h"
#include "../ExclusiveGameObject/SingletonBase.h"

class GameDebug : public SingletonBase<GameDebug>
{
// �����o�[�ϐ�
public:

private:
	std::unique_ptr<DirectX::SpriteBatch>   m_sprites;       // �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteFont>    m_font;          // �X�v���C�g�t�H���g

// �����o�[�֐�
public:
	~GameDebug();

	// �t�����h�֐�
	friend SingletonBase<GameDebug>;

	// �f�o�b�O�`��
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugPos);
	void DebugIntRender(char* debugLen, int intValue, DirectX::SimpleMath::Vector2 debugPos);
	void DebugFloatRender(char* debugLen, float floatValue, DirectX::SimpleMath::Vector2 debugPos);
	void DebugBoolRender(char* debugLen, bool boolValue, DirectX::SimpleMath::Vector2 debugPos);

private:
	GameDebug();

};
