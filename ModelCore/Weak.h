#pragma once

class WeakFlag
{
public:
	WeakFlag(void* p) : ptr_(p) {}
	~WeakFlag() {
		Invalid();
	}

	void Invalid() {
		ptr_ = nullptr;
	}
	template <class T>
	T* get() { return static_cast<T*>(ptr_); }

private:
	void* ptr_;
};

template <class T>
class WeakPtr
{
public:
	//WeakPtr()
};

template <class T>
class WeakOwner
{

};