#pragma once


typedef std::function<void(const Ptr<IObject>&, const Ptr<CollectionChangedArgs>&)> CollectionChangedCallback;

class CollectionChangedCallbackHandler : public ICollectionChangedHandler
{
	IMPLEMENT_IOBJECT_THREADSAFE();
public:	
	CollectionChangedCallbackHandler(const CollectionChangedCallback& func) : func_(func) {}
	virtual void OnCollectionChanged(const Ptr<IObject>& sender, const Ptr<CollectionChangedArgs>& args) override
	{
		func_(sender, args);
	}

private:
	CollectionChangedCallback func_;
};



Ptr<ICollectionChangedHandler> CreateHandler(const CollectionChangedCallback& callback)
{
	return new CollectionChangedCallbackHandler(callback);
}