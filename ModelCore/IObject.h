#pragma once
#include <stdint.h>

class IObject
{
public:	
	virtual void AddRef() = 0;
	virtual void Release() = 0;	
};

struct auto_refcount {
	uint32_t value;
	auto_refcount() : value(0) {}
};

template <class T>
class RefCountInternal
{
	auto_refcount ref_count_;
public:
	void InternalAddRef() {
		++ref_count_.value;
	}
	void InternalRelease() {
		if (--ref_count_.value == 0) {
			delete this;
		}
	}
};

template <class T>
class RefCount : public RefCountInternal<T>
{
public:
	void AddRef() {
		InternalAddRef();
	}
	void Release() {
		InternalRelease();
	}
};

template <class T>
class RefCountThreadSafeInternal
{
	auto_refcount ref_count_;
public:
	void InternalAddRef() {
		InterlockedIncrement(&ref_count_.value);
	}
	void InternalRelease() {
		if (InterlockedDecrement(&ref_count_.value) == 0) {
			delete static_cast<const T*>(this);
		}
	}
};

template <class T>
class RefCountThreadSafe : public RefCountThreadSafeInternal<T>
{	
public:
	void AddRef() {
		InternalAddRef();
	}
	void Release() {
		InternalRelease();
	}
};



template <class T, class Base>
class IObjectImpl : public RefCountThreadSafeInternal<T>, public Base
{
public:
	void AddRef()
	{
		InternalAddRef();
	}
	void Release()
	{
		InternalRelease();
	}
};




#define IMPLEMENT_IOBJECT_THREADSAFE() \
public: \
	virtual void AddRef() { \
		InterlockedIncrement(&ref_count_.value); \
	} \
	virtual void Release() { \
		if (InterlockedDecrement(&ref_count_.value) == 0) { \
			delete this; \
		} \
	} \
private: \
	auto_refcount ref_count_; \

#define IMPLEMENT_IOBJECT() \
public: \
	virtual void AddRef() { \
		++ref_count_.value; \
	} \
	virtual void Release() { \
		if (--ref_count_.value == 0) { \
			delete this; \
		} \
	} \
private: \
	auto_refcount ref_count_; \


#define IMPLEMENT_IOBJECT_SINGLETON() \
public: \
	virtual void AddRef() {}\
	virtual void Release() {} 