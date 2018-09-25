//////////////////////////////////////////////////////////////
// File.    SingletonBase.h
// Summary. SingletonBaseClass(TemplateClass)
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

template<class T> class SingletonBase
{
protected:

	SingletonBase() {}
	virtual ~SingletonBase() {}

public:

	// �C���X�^���X�Ăяo��
	static inline T& GetInstance()
	{
		static T ins;
		return ins;
	}

private:

	void operator=(const SingletonBase& obj) {}

	SingletonBase(const SingletonBase &obj) {}
};