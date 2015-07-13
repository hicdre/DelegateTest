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