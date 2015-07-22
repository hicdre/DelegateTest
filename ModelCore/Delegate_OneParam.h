#pragma once
#include "DelegateBase.h"

#if 1 //接口定义

template <class ParamOneType>
class IMethodDelegateBase_OneParam : public IDelegateInstanceBase
{
public:	
	virtual ~IMethodDelegateBase_OneParam() {}
	virtual void Execute(ParamOneType param1) const = 0;
};

#endif

#if 1 //接口实现

template <class ParamOneType>
class MethodDelegateBase_OneParam : public IMethodDelegateBase_OneParam<ParamOneType>
{
public:
	MethodDelegateBase_OneParam() : handle_(GenerateNewHandle()) {}

	virtual DelegateHandle GetHandle() const override {
		return handle_;
	}
private:
	DelegateHandle handle_;
};

template<class UserClass, class ParamOneType>
class SPMethodDelegateBase_OneParam_NoVar : public MethodDelegateBase_OneParam<ParamOneType>
{
public:
	typedef void (UserClass::*FMethodPtr)( ParamOneType );
	SPMethodDelegateBase_OneParam_NoVar(const std::shared_ptr<UserClass>& object, 
		FMethodPtr method) : weakObject_(object), methodPtr_(method) {}

	virtual void Execute(ParamOneType param1) const override
	{
		std::shared_ptr<UserClass> object = weakObject_.lock();
		if (!object)
			return;

		UserClass* rawObject = object.get();

		return (rawObject->*methodPtr_)(param1);
	}
		
private:
	std::weak_ptr<UserClass> weakObject_;
	FMethodPtr methodPtr_;	
};

template<class UserClass, class ParamOneType, class VarOneType>
class SPMethodDelegateBase_OneParam_OneVar : public MethodDelegateBase_OneParam<ParamOneType>
{
public:
	typedef void (UserClass::*FMethodPtr)( ParamOneType, VarOneType );
	SPMethodDelegateBase_OneParam_OneVar(const std::shared_ptr<UserClass>& object, 
		FMethodPtr method,
		VarOneType var1) : weakObject_(object), methodPtr_(method)
		, var1_(var1)
	{}

	void Execute(ParamOneType param1) const
	{
		std::shared_ptr<UserClass> object = weakObject_.lock();
		if (!object)
			return;

		UserClass* rawObject = object.get();

		return (rawObject->*methodPtr_)(param1, var1_);
	}	
private:
	std::weak_ptr<UserClass> weakObject_;
	FMethodPtr methodPtr_;
	VarOneType var1_;
};

template<class UserClass, class ParamOneType, class VarOneType, class VarTwoType>
class SPMethodDelegateBase_OneParam_TwoVar : public MethodDelegateBase_OneParam<ParamOneType>
{
public:
	typedef void (UserClass::*FMethodPtr)( ParamOneType, VarOneType, VarTwoType );
	SPMethodDelegateBase_OneParam_TwoVar(const std::shared_ptr<UserClass>& object, 
		FMethodPtr method,
		VarOneType var1, VarTwoType var2) 
		: weakObject_(object), methodPtr_(method)
		, var1_(var1), var2_(var2)
	{}

	void Execute(ParamOneType param1) const
	{
		std::shared_ptr<UserClass> object = weakObject_.lock();
		if (!object)
			return;

		UserClass* rawObject = object.get();

		return (rawObject->*methodPtr_)(param1, var1_, var2_);
	}

private:
	std::weak_ptr<UserClass> weakObject_;
	FMethodPtr methodPtr_;
	VarOneType var1_;
	VarTwoType var2_;
};


template<class FunctorType, class ParamOneType>
class FunctorMethodDelegateBase_OneParam_NoVar : public MethodDelegateBase_OneParam<ParamOneType>
{
public:
	FunctorMethodDelegateBase_OneParam_NoVar(const FunctorType& functor)
		: functor_(functor) {}

	virtual void Execute(ParamOneType param1) const override
	{
		return functor_(param1);
	}
private:
	FunctorType functor_;
};

template<class FunctorType, class ParamOneType, class VarOneType>
class FunctorMethodDelegateBase_OneParam_OneVar : public MethodDelegateBase_OneParam<ParamOneType>
{
public:
	FunctorMethodDelegateBase_OneParam_OneVar(const FunctorType& functor,
		VarOneType var1)
		: functor_(functor), var1_(var1) {}

	virtual void Execute(ParamOneType param1) const override
	{
		return functor_(param1, var1_);
	}
private:
	FunctorType functor_;
	VarOneType var1_;
};

template<class FunctorType, class ParamOneType, class VarOneType, class VarTwoType>
class FunctorMethodDelegateBase_OneParam_TwoVar : public MethodDelegateBase_OneParam<ParamOneType>
{
public:
	FunctorMethodDelegateBase_OneParam_TwoVar(const FunctorType& functor,
		VarOneType var1, VarOneType var2)
		: functor_(functor), var1_(var1), var2_(var2) {}

	virtual void Execute(ParamOneType param1) const override
	{
		return functor_(param1, var1_, var2_);
	}
private:
	FunctorType functor_;
	VarOneType var1_;
	VarTwoType var2_;
};


#endif

template <class ParamOneType>
class Delegate_OneParam
{
public:	
	typedef IMethodDelegateBase_OneParam<ParamOneType> IMethodDelegateInstanceType;
	Delegate_OneParam() : instance_(NULL) {}
	Delegate_OneParam(IMethodDelegateInstanceType* instance) : instance_(instance) {}
	~Delegate_OneParam() { Unbind(); }
	template< class UserClass>
	void Bind(const std::shared_ptr< UserClass >& object, typename SPMethodDelegateBase_OneParam_NoVar< UserClass, ParamOneType >::FMethodPtr func)
	{
		Unbind();
		instance_ = new SPMethodDelegateBase_OneParam_NoVar<UserClass, ParamOneType>(object, func);
	}

	template< class UserClass, class VarOneType>
	void Bind(const std::shared_ptr< UserClass >& object, typename SPMethodDelegateBase_OneParam_OneVar< UserClass, ParamOneType, VarOneType >::FMethodPtr func
		, VarOneType var1)
	{
		Unbind();
		instance_ = new SPMethodDelegateBase_OneParam_OneVar<UserClass, ParamOneType, VarOneType>(object, func, var1);
	}

	template< class UserClass, class VarOneType, class VarTwoType>
	void Bind(const std::shared_ptr< UserClass >& object, typename SPMethodDelegateBase_OneParam_TwoVar< UserClass, ParamOneType, VarOneType, VarTwoType >::FMethodPtr func
		, VarOneType var1, VarTwoType var2)
	{
		Unbind();
		instance_ = new SPMethodDelegateBase_OneParam_TwoVar<UserClass, ParamOneType, VarOneType, VarTwoType>(object, func, var1, var2);
	}

	template< class FunctorType>
	void BindF(const FunctorType& functor)
	{
		Unbind();
		instance_ = new FunctorMethodDelegateBase_OneParam_NoVar<FunctorType>(functor);
	}

	template< class FunctorType, class VarOneType>
	void BindF(const FunctorType& functor, VarOneType var1)
	{
		Unbind();
		instance_ = new FunctorMethodDelegateBase_OneParam_OneVar<FunctorType, ParamOneType, VarOneType>(functor, var1);
	}

	template< class FunctorType, class VarOneType, class VarTwoType>
	void BindF(const FunctorType& functor, VarOneType var1, VarTwoType var2)
	{
		Unbind();
		instance_ = new FunctorMethodDelegateBase_OneParam_TwoVar<FunctorType, ParamOneType, VarOneType, VarTwoType>(functor, var1, var2);
	}
// 	template<class UserClass>
// 	class SPMethodDelegate : public SPMethodDelegateBase_OneParam_NoVar<UserClass>
// 	{
// 	public:
// 		SPMethodDelegate(const std::shared_ptr< UserClass >& InUserObject, 
// 			typename SPMethodDelegateBase_OneParam_NoVar<UserClass>::FMethodPtr InMethodPtr)
// 			: SPMethodDelegateBase_OneParam_NoVar<UserClass>(InUserObject, InMethodPtr);
// 	};
	void Unbind()
	{
		if (instance_)
		{
			delete instance_;
			instance_ = NULL;
		}
	}

	void Execute(ParamOneType param1) const 
	{
		if (!instance_)
			return;
		return instance_->Execute();
	}

private:
	IMethodDelegateInstanceType* instance_;
};

template <class ParamOneType>
class MultiDelegate_OneParam 
	: public MultiDelegateBase<IMethodDelegateBase_OneParam<ParamOneType>>
{
public:	
	typedef IMethodDelegateBase_OneParam<ParamOneType> IMethodDelegateInstanceType;	
	template< class UserClass>
	DelegateHandle Add(const std::shared_ptr< UserClass >& object, typename SPMethodDelegateBase_OneParam_NoVar< UserClass, ParamOneType >::FMethodPtr func)
	{		
		return AddInternal(new SPMethodDelegateBase_OneParam_NoVar<UserClass, ParamOneType>(object, func));
	}

	template< class UserClass, class VarOneType>
	DelegateHandle Add(const std::shared_ptr< UserClass >& object, typename SPMethodDelegateBase_OneParam_OneVar< UserClass, ParamOneType, VarOneType >::FMethodPtr func,
		VarOneType var1)
	{		
		return AddInternal(new SPMethodDelegateBase_OneParam_OneVar<UserClass, ParamOneType, VarOneType>(object, func, var1));
	}

	template< class UserClass, class VarOneType, class VarTwoType>
	DelegateHandle Add(const std::shared_ptr< UserClass >& object, typename SPMethodDelegateBase_OneParam_TwoVar< UserClass, ParamOneType, VarOneType, VarTwoType >::FMethodPtr func,
		VarOneType var1, VarTwoType var2)
	{		
		return AddInternal(new SPMethodDelegateBase_OneParam_TwoVar<UserClass, ParamOneType, VarOneType, VarTwoType>(object, func, var1, var2));
	}

	template< class FunctorType>
	DelegateHandle AddF(const FunctorType& functor)
	{
		return AddInternal(new FunctorMethodDelegateBase_OneParam_NoVar<FunctorType>(functor));
	}

	template< class FunctorType, class VarOneType>
	DelegateHandle AddF(const FunctorType& functor, VarOneType var1)
	{
		return AddInternal(new FunctorMethodDelegateBase_OneParam_OneVar<FunctorType, ParamOneType, VarOneType>(functor, var1));
	}

	template< class FunctorType, class VarOneType, class VarTwoType>
	DelegateHandle AddF(const FunctorType& functor, VarOneType var1, VarTwoType var2)
	{
		return AddInternal(new FunctorMethodDelegateBase_OneParam_TwoVar<FunctorType, ParamOneType, VarOneType, VarTwoType>(functor, var1, var2));
	}

	void Execute(ParamOneType param1)
	{
		const std::vector<IMethodDelegateInstanceType*>& instacneList = GetInstanceList();
		if (instacneList.empty())
			return;
		for (int index = instacneList.size() - 1; index >=0; index--)
		{
			IMethodDelegateInstanceType* instance = instacneList[index];
			instance->Execute();
		}
	}
};