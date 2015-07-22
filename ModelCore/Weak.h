#pragma once
#include <cassert>


class ThreadChecker
{
	DWORD thread_id_;
public:
	ThreadChecker() : thread_id_(0) {}
	void InitThread() {
		thread_id_ = ::GetCurrentThreadId();
	}
	bool IsInValidThread() const {
		return thread_id_ == ::GetCurrentThreadId();
	}
};


class WeakFlag : public RefCount<WeakFlag>
{
public:
	WeakFlag(void* p) : ptr_(p) {
		checker_.InitThread();
	}
	~WeakFlag() {
		Invalid();
	}

	void Invalid() {
		assert(IsInValidThread());
		ptr_ = nullptr;
	}
	template <class T>
	T* get() { 
		assert(IsInValidThread());
		return static_cast<T*>(ptr_); 
	}

private:
	bool IsInValidThread() const {
		return checker_.IsInValidThread();
	}
	void* ptr_;
	ThreadChecker checker_;
};


class WeakThreadSafeFlag : public RefCountThreadSafe<WeakThreadSafeFlag>
{
public:
	WeakThreadSafeFlag() 
		: ref_count_(0) {		
	}
	~WeakThreadSafeFlag() {		
	}

	bool IsValid() {
		return InterlockedCompareExchange(&ref_count_, 0, 0) > 0;
	}	
	
	LONG AddRef() {
		return InterlockedIncrement(&ref_count_);
	}
	LONG Release() {
		return InterlockedDecrement(&ref_count_);
	}

private:
	LONG ref_count_;
	//mutex
};


typedef Ptr<WeakFlag> WeakRef;
typedef Ptr<WeakThreadSafeFlag> WeakSRef;

template <class T>
class WPtr
{
public:
	WPtr(const WeakRef& ref) : ref_(ref) {}

	T* get() const {
		if (T* p = ref_->get<T>())
			return p;
		return nullptr;
	}	
private:
	WeakRef ref_;
};

template <class T>
class WSPtr
{
public:	
	WSPtr(const WeakSRef& ref, T* ptr) : ref_(ref), ptr_(ptr) {}

	Ptr<T> get() {
		Ptr<T> ptr;
		if (ref->AddRef() >= 1) //add and lock
			ptr.reset(ptr_);		
		ref->Release();
		return ptr;
	}	
private:
	WeakSRef ref_;
	T* ptr_;
};

//一般对象
template <class T>
class WeakOwner
{
public:
	WeakOwner(T* ptr) 
		: ref_(new WeakFlag(ptr))
	{
			
	}
	~WeakOwner()
	{
		ReleaseWeak();
	}

	WPtr<T> GetWeak() {					
		return WPtr<T>(ref_);
	}

	void ReleaseWeak()
	{
		if (ref_)
			ref_->Invalid();
	}

private:
	WeakRef ref_;	
};

//一般对象，IObject对象
template <class T>
class SupportThreadLocalWeak
{
public:
	~SupportThreadLocalWeak() {
		if (ref_)
			ref_->Invalid();
	}
	
	WPtr<T> GetWeak() {
		if (!ref_)
			ref_.reset(new WeakFlag((T*)this));
		return WPtr<T>(ref_);
	}

private:
	WeakRef ref_;
};

template <class T>
class RefCountThreadSafeSupportWeak
{	
	WeakSRef weak_ref_;
public:	
	RefCountThreadSafeSupportWeak() 
		: weak_ref_(new WeakThreadSafeFlag) {
		
	}
	void AddRef() {
		weak_ref_->AddRef();		
	}
	void Release() {
		if (weak_ref_->Release() <= 0) {			
			delete static_cast<const T*> this;
		}
	}
	WSPtr<T> GetWeak() {		
		return WSPtr<T>(weak_ref_, this);
	}	
};

