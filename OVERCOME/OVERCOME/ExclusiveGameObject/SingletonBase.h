//////////////////////////////////////////////////////////////
// File.    SingletonBase.h
// Summary. SingletonBaseClass(TemplateClass)
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once
#include <memory>

template<class T> class SingletonBase
{
// ‰ü—ÇŒã‚Ìƒƒ“ƒo[•Ï”AŠÖ”
private:
	typedef std::unique_ptr<T> singleton_pointer_type;

public:
	static T& SingletonGetInstance()
	{
		static typename T::singleton_pointer_type s_singleton(T::CreateInstance());

		return GetReference(s_singleton);
	}

private:
	inline static T *CreateInstance() { return new T(); }

	inline static T &GetReference(const singleton_pointer_type &ptr) { return *ptr; }

protected:
	SingletonBase(){}

private:
	SingletonBase(const SingletonBase &) = delete;
	SingletonBase& operator=(const SingletonBase &) = delete;
	SingletonBase(SingletonBase &&) = delete;
	SingletonBase& operator=(SingletonBase &&) = delete;
};