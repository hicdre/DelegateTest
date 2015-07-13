#pragma once
#include "ModelCore/ModelCoreTypes.h"
#include <vector>
#include <cassert>
#include <functional>

enum CollectionChangedAction
{
	/// <summary> One or more items were added to the collection. </summary>
	NotifyCollectionAdd,
	/// <summary> One or more items were removed from the collection. </summary>
	NotifyCollectionRemove,
	/// <summary> One or more items were replaced in the collection. </summary>
	NotifyCollectionReplace,
	/// <summary> One or more items were moved within the collection. </summary>
	NotifyCollectionMove,
	/// <summary> The contents of the collection changed dramatically. </summary>
	NotifyCollectionReset,//这种情况需要重新获取
};

class MODELCORE_API CollectionChangedArgs : public IObject
{
	IMPLEMENT_IOBJECT_THREADSAFE()
public:
	CollectionChangedArgs(CollectionChangedAction action);

	CollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem);

	CollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem, int index);

	CollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems);

	CollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int startingIndex);

	CollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem);

	CollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem, int index);

	CollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems);

	CollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems, int startingIndex);

	CollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem, int index, int oldIndex);

	CollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int index, int oldIndex);

	CollectionChangedAction Action() const;

	/// <summary>
	/// The items affected by the change.
	/// </summary>
	const std::vector<Ptr<IObject>>& NewItems() const;

	/// <summary>
	/// The old items affected by the change (for Replace events).
	/// </summary>
	const std::vector<Ptr<IObject>>& OldItems() const;

	/// <summary>
	/// The index where the change occurred.
	/// </summary>
	int NewStartingIndex() const;	

	/// <summary>
	/// The old index where the change occurred (for Move events).
	/// </summary>
	int OldStartingIndex() const;
private:

	void InitializeAddOrRemove(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int startingIndex);

	void InitializeAdd(CollectionChangedAction action);

	void InitializeAdd(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, int newStartingIndex);

	void InitializeRemove(CollectionChangedAction action, const std::vector<Ptr<IObject>>& oldItems, int oldStartingIndex);

	void InitializeMoveOrReplace(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems, int startingIndex, int oldStartingIndex);
	CollectionChangedAction action_;
	std::vector<Ptr<IObject>> newItems_;
	std::vector<Ptr<IObject>> oldItems_;
	int newStartingIndex_;
	int oldStartingIndex_;
};

class ICollectionChangedHandler : public IObject
{
public:
	virtual void OnCollectionChanged(const Ptr<IObject>&, const Ptr<CollectionChangedArgs>&) = 0;
};


class ICollectionChangedNotifier
{
public:
	virtual void AddCollectionChangedHandler(const Ptr<ICollectionChangedHandler>& handler) = 0;
	virtual void RemoveCollectionChangedHandler(const Ptr<ICollectionChangedHandler>& handler) = 0;
	virtual void NotifyCollectionChanged(const Ptr<IObject>& sender, const Ptr<CollectionChangedArgs>& args) = 0;
};

typedef std::function<void(const Ptr<IObject>&, const Ptr<CollectionChangedArgs>&)> CollectionChangedCallback;
MODELCORE_API Ptr<ICollectionChangedHandler> CollectionChangedHandler(const CollectionChangedCallback& callback);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem, int index);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int startingIndex);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& newItem, const Ptr<IObject>& oldItem, int index);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& newItems, const std::vector<Ptr<IObject>>& oldItems, int startingIndex);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const Ptr<IObject>& changedItem, int index, int oldIndex);
// EVENTCORE_API Ptr<ICollectionChangedArgs> CreateCollectionChangedArgs(CollectionChangedAction action, const std::vector<Ptr<IObject>>& changedItems, int index, int oldIndex);


class WeakCollectionChangedHandler : public ICollectionChangedHandler
{
public:
	virtual void OnCollectionChanged(const Ptr<IObject>&, const Ptr<CollectionChangedArgs>&)
	{
	}
};