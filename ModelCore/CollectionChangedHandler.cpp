#include "stdafx.h"
#include "CollectionChangedHandler.h"

void CollectionChangedArgs::InitializeMoveOrReplace( CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems, int startingIndex, int oldStartingIndex )
{
	InitializeAdd(action, newItems, startingIndex);
	InitializeRemove(action, oldItems, oldStartingIndex);
}

void CollectionChangedArgs::InitializeRemove( CollectionChangedAction action, const std::vector<Ptr<IObject>>& oldItems, int oldStartingIndex )
{
	action_ = action;
	oldItems_ = oldItems;
	oldStartingIndex_ = oldStartingIndex;
}

void CollectionChangedArgs::InitializeAdd( CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, int newStartingIndex )
{
	action_ = action;
	newItems_ = newItems;		
	newStartingIndex_ = newStartingIndex;
}

void CollectionChangedArgs::InitializeAdd( CollectionChangedAction action )
{
	action_ = action;
}

void CollectionChangedArgs::InitializeAddOrRemove( CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int startingIndex )
{
	if (action == NotifyCollectionAdd)
		InitializeAdd(action, changedItems, startingIndex);
	else if (action == NotifyCollectionRemove)
		InitializeRemove(action, changedItems, startingIndex);
	else
		assert(0);
}

int CollectionChangedArgs::OldStartingIndex() const
{
	return oldStartingIndex_;
}

int CollectionChangedArgs::NewStartingIndex() const
{
	return newStartingIndex_;
}

const std::vector<Ptr<IObject>>& CollectionChangedArgs::OldItems() const
{
	return oldItems_;
}

const std::vector<Ptr<IObject>>& CollectionChangedArgs::NewItems() const
{
	return newItems_;
}

CollectionChangedAction CollectionChangedArgs::Action() const
{
	return action_;
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int index, int oldIndex )
{
	assert(action == NotifyCollectionMove && index >= 0);

	InitializeMoveOrReplace(action, changedItems, changedItems, index, oldIndex);
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const Ptr<IObject>& changedItem, int index, int oldIndex )
{
	assert(action == NotifyCollectionMove && index >= 0);

	std::vector<Ptr<IObject>> changedItems;
	changedItems.push_back(changedItem);		
	InitializeMoveOrReplace(action, changedItems, changedItems, index, oldIndex);
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems, int startingIndex )
{
	assert(action == NotifyCollectionReplace && !newItems.empty() && !oldItems.empty());

	InitializeMoveOrReplace(action, newItems, oldItems, startingIndex, startingIndex);
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems )
{
	assert(action == NotifyCollectionReplace && !newItems.empty() && !oldItems.empty());

	InitializeMoveOrReplace(action, newItems, oldItems, -1, -1);
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem, int index )
{
	assert(action == NotifyCollectionReplace);

	int oldStartingIndex = index;
	std::vector<Ptr<IObject>> oldItems, newItems;
	newItems.push_back(newItem);
	oldItems.push_back(oldItem);
	InitializeMoveOrReplace(action, newItems, oldItems, index, oldStartingIndex);
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem )
{
	assert(action == NotifyCollectionReplace);		

	std::vector<Ptr<IObject>> oldItems, newItems;
	newItems.push_back(newItem);
	oldItems.push_back(oldItem);
	InitializeMoveOrReplace(action, newItems, oldItems, -1, -1);
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int startingIndex )
{
	assert((action == NotifyCollectionAdd) || (action == NotifyCollectionRemove)
		|| (action == NotifyCollectionReset));

	if (action == NotifyCollectionReset)
	{
		assert(changedItems.empty() && startingIndex == -1);

		InitializeAdd(action);
	}
	else
	{
		assert(!changedItems.empty() && startingIndex >= 0);			

		InitializeAddOrRemove(action, changedItems, startingIndex);
	}
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems )
{
	assert((action == NotifyCollectionAdd) || (action == NotifyCollectionRemove)
		|| (action == NotifyCollectionReset));

	if (action == NotifyCollectionReset)
	{
		assert(changedItems.empty());

		InitializeAdd(action);
	}
	else
	{
		assert(!changedItems.empty());

		InitializeAddOrRemove(action, changedItems, -1);
	}
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const Ptr<IObject>& changedItem, int index )
{
	assert((action == NotifyCollectionAdd) || (action == NotifyCollectionRemove)
		|| (action == NotifyCollectionReset));


	if (action == NotifyCollectionReset)
	{
		assert(!changedItem && index == -1);			

		InitializeAdd(action);
	}
	else
	{
		std::vector<Ptr<IObject>> changedItems;
		changedItems.push_back(changedItem);
		InitializeAddOrRemove(action, changedItems, index);
	}
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action, const Ptr<IObject>& changedItem )
{
	assert((action == NotifyCollectionAdd) || (action == NotifyCollectionRemove)
		|| (action == NotifyCollectionReset));

	if (action == NotifyCollectionReset)
	{
		assert(!changedItem);				

		InitializeAdd(action);
	}
	else
	{
		std::vector<Ptr<IObject>> changedItems;
		changedItems.push_back(changedItem);
		InitializeAddOrRemove(action, changedItems, -1);
	}
}

CollectionChangedArgs::CollectionChangedArgs( CollectionChangedAction action )
{
	assert(action == NotifyCollectionReset);

	InitializeAdd(action);
}


// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action)
// {
// 	return new CollectionChangedArgs(action);
// }
// 
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem)
// {
// 	return new CollectionChangedArgs(action, changedItem);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem, int index)
// {
// 	return new CollectionChangedArgs(action, changedItem, index);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems)
// {
// 	return new CollectionChangedArgs(action, changedItems);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int startingIndex)
// {
// 	return new CollectionChangedArgs(action, changedItems, startingIndex);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem)
// {
// 	return new CollectionChangedArgs(action, newItem, oldItem);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem, int index)
// {
// 	return new CollectionChangedArgs(action, newItem, oldItem, index);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems)
// {
// 	return new CollectionChangedArgs(action, newItems, oldItems);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems, int startingIndex)
// {
// 	return new CollectionChangedArgs(action, newItems, oldItems, startingIndex);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem, int index, int oldIndex)
// {
// 	return new CollectionChangedArgs(action, changedItem, index, oldIndex);
// }
// Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int index, int oldIndex)
// {
// 	return new CollectionChangedArgs(action, changedItems, index, oldIndex);
// }

class CollectionChangedCallbackHandler : public IObjectImpl<CollectionChangedCallbackHandler, ICollectionChangedHandler>
{
	//IMPLEMENT_IOBJECT_THREADSAFE();
public:	
	CollectionChangedCallbackHandler(const CollectionChangedCallback& func) : func_(func) {}
	virtual void OnCollectionChanged(const Ptr<IObject>& sender, const Ptr<CollectionChangedArgs>& args) override
	{
		func_(sender, args);
	}

private:
	CollectionChangedCallback func_;
};


Ptr<ICollectionChangedHandler> CollectionChangedHandler( const CollectionChangedCallback& callback )
{
	return new CollectionChangedCallbackHandler(callback);
}
