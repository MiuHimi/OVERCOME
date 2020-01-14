//////////////////////////////////////////////////////////////
// File.    Drawable.h
// Summary. DrawableClass
// Date.    2019/12/18
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

class Drawable
{
// �����o�[�ϐ�
private:
	bool m_isVisible;		// �\����ԃt���O

// �����o�[�֐�
public:
	// �R���X�g���N�^
	Drawable() : m_isVisible(true) {};
	// �f�X�g���N�^
	virtual ~Drawable() = default;

	// �`��֐�
	virtual void Draw() const = 0;

	// �\����Ԑݒ�
	void SetVisible(bool visibleFlag)	{ m_isVisible = visibleFlag; }

	// �\����Ԏ擾
	bool IsVisible() const				{ return m_isVisible; }
};