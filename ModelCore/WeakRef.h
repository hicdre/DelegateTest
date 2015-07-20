#pragma once
#include <memory>

template <class T>
struct WeakFlag {
	T* ptr;
	WeakFlag(T* p) : ptr(p){}
	void Reset() {
		ptr = nullptr;
	}
};



template <class T>
class WeakPtr
{	
	typedef std::shared_ptr<WeakFlag<T>> WeakRef;
public:
	WeakPtr(const WeakRef& ref) : ref_(ref) {}

	T* get() {
		if (ref_->ptr)
			return ref_->ptr;
		return nullptr;
	}
	
private:
	WeakRef ref_;
};

template <class T>
class WeakOwner
{
	typedef std::shared_ptr<WeakFlag<T>> WeakRef;
public:
	~WeakOwner()
	{
		ReleaseWeak();
	}

	WeakPtr<T> GetWeak() {
		if (!ref_)
			ref_ = std::make_shared<WeakFlag<T>>((T*)this);
		return WeakPtr<T>(ref_);
	}

	void ReleaseWeak()
	{
		if (ref_)
			ref_->Reset();
	}

private:
	WeakRef ref_;
};