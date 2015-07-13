#pragma once

template <class T>
class Ptr
{
public:
	Ptr() : ptr_(nullptr) {}
	Ptr(T* p) : ptr_(p) { AddRef(ptr_); }
	Ptr(const Ptr<T>& other) : ptr_(nullptr) {
		reset(other.ptr_);
	}
	template <class Q>
	Ptr(const Ptr<Q>& other) : ptr_(nullptr) {
		reset(dynamic_cast<T*>(other.get()));
	}

	~Ptr() {
		Release(ptr_);		
	}

	operator T*() const
	{
		return ptr_;
	}

	Ptr& operator=(const Ptr &other)
	{
		reset(other.ptr_);
		return *this;
	}

	template <class Q>
	Ptr<T>& operator=(const Ptr<Q> &other)
	{
		reset(dynamic_cast<T*>(other.get()));
		return *this;
	}

	T* operator->() const{
		return ptr_;
	}

	T** operator&() {
		return &ptr_;
	}
	T& operator*() {
		return *ptr_;
	}
	bool operator!() const throw()
	{
		return (NULL == ptr_);
	}

	bool operator<(T* pT) const throw()
	{
		return ptr_ < pT;
	}

	bool operator!=(T* pT) const
	{
		return !operator==(pT);
	}

	bool operator==(T* pT) const throw()
	{
		return ptr_ == pT;
	}
	void reset(T* p) {
		T* d = ptr_;		
		ptr_ = p;
		AddRef(ptr_);
		Release(d);
	}
	T* get() const {
		return ptr_;
	}
	template <class U>
	U* get() const {
		return dynamic_cast<U*>(ptr_);
	}
private:
	void Release(T* p) {
		if (p) {
			p->Release();
			p = nullptr;
		}
	}
	void AddRef(T* p) {
		if (p)
			p->AddRef();
	}
	T* ptr_;
};

template <class T, class U>
T* PtrCast(const Ptr<U>& other) {
	return dynamic_cast<T*>((U*)other);
}