#pragma once
#include "ModelCore/CollectionChangedHandler.h"
#include "ModelCore/PropertyChangedHandler.h"
#include "ModelCore/HandlerList.h"
#include <functional>


class MODELCORE_API ObservableCollection 
	: public ICollectionChangedNotifier
	, public IPropertyChangedNotifier
	, public IPropertyObject
{
	IMPLEMENT_IOBJECT_THREADSAFE();
public:
	ObservableCollection();
	~ObservableCollection();
	
	void ClearItems();

	void InsertItem(int index, const Ptr<IObject>& item);
	void InsertItem(const Ptr<IObject>& item);
	void InsertItems(int index, const std::vector<Ptr<IObject>>& items);
	void InsertItems(const std::vector<Ptr<IObject>>& item);

	void RemoveItem(int index);
	void RemoveLastItem();

	void SortAscend(const std::function<bool(const Ptr<IObject>& a, const Ptr<IObject>& b)>& func);
	void SortDesc(const std::function<bool(const Ptr<IObject>& a, const Ptr<IObject>& b)>& func);

	int Count() const;
	Ptr<IObject> GetItem(int index) const;
	const std::vector<Ptr<IObject>>& GetItems() const;

	template<class T>
	Ptr<T> GetItem(int index) {
		return PtrCast<T>(items_[index]);
	}
	int GetItemIndex(const Ptr<IObject>& item);

	virtual Ptr<PropertyValue> GetProperty(const std::string& str) override;
	
	virtual void AddCollectionChangedHandler(const Ptr<ICollectionChangedHandler>& handler) override;
	virtual void RemoveCollectionChangedHandler(const Ptr<ICollectionChangedHandler>& handler) override;

	virtual void AddPropertyChangedHandler(const Ptr<IPropertyChangedHandler>& handler) override;
	virtual void RemovePropertyChangedHandler(const Ptr<IPropertyChangedHandler>& handler) override;
protected:
	virtual void NotifyCollectionChanged(const Ptr<IObject>& sender, const Ptr<CollectionChangedArgs>& args) override;
	virtual void NotifyPropertyChanged(const Ptr<IPropertyObject>& sender, const Ptr<PropertyChangedArgs>& args) override;

	void OnPropertyCountChanged();

	void OnCollectionChanged(const Ptr<CollectionChangedArgs>& args);
	void OnCollectionChanged(CollectionChangedAction action, const Ptr<IObject>& item, int index);
	void OnCollectionChanged(CollectionChangedAction action, const Ptr<IObject>& item, int index, int oldIndex);
	void OnCollectionChanged(CollectionChangedAction action, const Ptr<IObject>& oldItem, const Ptr<IObject>& newItem, int index);
	void OnCollectionReset();

	std::vector<Ptr<IObject>> items_;
	HandlerList<ICollectionChangedHandler> collection_observers_;
	HandlerList<IPropertyChangedHandler> property_observers_;
};