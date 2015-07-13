#pragma once
#include "ModelCore/PropertyObject.h"

class MODELCORE_API PropertyChangedArgs : public IObject
{
	IMPLEMENT_IOBJECT_THREADSAFE()
public:
	PropertyChangedArgs(const std::string& name);
	const std::string& Name() const;	
protected:
	std::string name_;
};

class IPropertyChangedHandler : public IObject
{
public:
	virtual void OnPropertyChanged(const Ptr<IPropertyObject>& sender, const Ptr<PropertyChangedArgs>& args) = 0;
};

class IPropertyChangedNotifier
{
public:
	virtual void AddPropertyChangedHandler(const Ptr<IPropertyChangedHandler>& handler) = 0;
	virtual void RemovePropertyChangedHandler(const Ptr<IPropertyChangedHandler>& handler) = 0;
	virtual void NotifyPropertyChanged(const Ptr<IPropertyObject>& sender, const Ptr<PropertyChangedArgs>& args) = 0;
};
