//////////////////////////////////////////////////////////////
// File.    DrawableList.h
// Summary. DrawableListClass
// Date.    2019/12/18
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <map>
#include <memory>
#include <algorithm>
#include "Drawable.h"

class DrawableList
{
// �����o�[�ϐ�
public:
	enum DRAW_LAYER
	{
		STATIC_OBJ_3D,
		DYNAMIC_OBJ_3D,
		STATIC_OBJ_2D,
		DYNAMIC_OBJ_2D,
		UI
	};

private:
	std::multimap<int, std::shared_ptr<Drawable>> m_drawables;	// ���X�g

// �����o�[�֐�
public:
	// �R���X�g���N�^
	DrawableList() = default;
	// �f�X�g���N�^
	~DrawableList() = default;

	// �ǉ�
	void Add(std::shared_ptr<Drawable> drawable, int layer)
	{
		// ���X�g�ɒǉ�����
		m_drawables.insert(std::make_pair(layer, drawable));
	}

	// �`��
	void Draw() const
	{
		// ���X�g�ɒǉ�����Ă���I�u�W�F�N�g�����Ɍ��Ă����A
		std::for_each(m_drawables.begin(), m_drawables.end(), [](auto p)
		{
			// �\���ł���I�u�W�F�N�g�������
			if (p.second->IsVisible())
			{
				// �`�悷��
				p.second->Draw();
			}
		});
	}
};