#include "stdafx.h"
#include "ObservableCollection.h"
#include <algorithm>

ObservableCollection::ObservableCollection()
{
}

ObservableCollection::~ObservableCollection()
{

}

void ObservableCollection::OnCollectionReset()
{
	OnCollectionChanged(new CollectionChangedArgs(NotifyCollectionReset));
}

void ObservableCollection::OnCollectionChanged( CollectionChangedAction action, const Ptr<IObject>& oldItem, const Ptr<IObject>& newItem, int index )
{
	OnCollectionChanged(new CollectionChangedArgs(action, newItem, oldItem, index));
}

void ObservableCollection::OnCollectionChanged( CollectionChangedAction action, const Ptr<IObject>& item, int index, int oldIndex )
{
	OnCollectionChanged(new CollectionChangedArgs(action, item, index, oldIndex));
}

void ObservableCollection::OnCollectionChanged( CollectionChangedAction action, const Ptr<IObject>& item, int index )
{
	OnCollectionChanged(new CollectionChangedArgs(action, item, index));
}

void ObservableCollection::OnCollectionChanged( const Ptr<CollectionChangedArgs>& args )
{
	NotifyCollectionChanged(this, args);
}

void ObservableCollection::OnPropertyCountChanged()
{
	NotifyPropertyChanged(this, new PropertyChangedArgs("Count"));	
}

void ObservableCollection::NotifyCollectionChanged( const Ptr<IObject>& sender, const Ptr<CollectionChangedArgs>& args )
{
	FOR_EACH_HANDLER(ICollectionChangedHandler, collection_observers_, OnCollectionChanged(sender, args));
}

void ObservableCollection::RemoveCollectionChangedHandler( const Ptr<ICollectionChangedHandler>& handler )
{
	collection_observers_.RemoveObserver(handler);
}

void ObservableCollection::AddCollectionChangedHandler( const Ptr<ICollectionChangedHandler>& handler )
{
	collection_observers_.AddObserver(handler);
}

int ObservableCollection::GetItemIndex( const Ptr<IObject>& item )
{
	for (int i = 0; i < items_.size(); ++i)
	{
		if (items_[i] == item)
			return i;
	}
	return -1;
}

int ObservableCollection::Count() const
{
	return items_.size();
}

void ObservableCollection::RemoveLastItem()
{
	if (items_.size())
		RemoveItem(items_.size() - 1);
}

void ObservableCollection::RemoveItem( int index )
{
	Ptr<IObject> item = items_[index];
	items_.erase(items_.begin() + index);
	OnCollectionChanged(NotifyCollectionRemove, item, index);
	OnPropertyCountChanged();
}

void ObservableCollection::InsertItem( const Ptr<IObject>& item )
{
	InsertItem(items_.size(), item);
}

void ObservableCollection::InsertItem( int index, const Ptr<IObject>& item )
{
	items_.insert(items_.begin() + index, item);
	OnCollectionChanged(NotifyCollectionAdd, item, index);
	OnPropertyCountChanged();
}

void ObservableCollection::InsertItems( int index, const std::vector<Ptr<IObject>>& items )
{
	items_.insert(items_.begin() + index, items.begin(), items.end());

	OnCollectionChanged(new CollectionChangedArgs(NotifyCollectionAdd, items, index));	
	OnPropertyCountChanged();
}

void ObservableCollection::InsertItems( const std::vector<Ptr<IObject>>& items )
{
	InsertItems(items_.size(), items);	
}

void ObservableCollection::ClearItems()
{
	items_.clear();
	OnCollectionReset();
	OnPropertyCountChanged();
}

Ptr<IObject> ObservableCollection::GetItem( int index ) const
{
	if (index >= 0 && index < items_.size())
		return items_[index];
	return NULL;
}

void ObservableCollection::SortAscend( const std::function<bool(const Ptr<IObject>& a, const Ptr<IObject>& b)>& func )
{
	std::sort(items_.begin(), items_.end(), func);
	OnCollectionReset();
}

void ObservableCollection::SortDesc( const std::function<bool(const Ptr<IObject>& a, const Ptr<IObject>& b)>& func )
{
	std::sort(items_.begin(), items_.end(), [func](const Ptr<IObject>& a, const Ptr<IObject>& b)->bool
	{
		return !func(a,b);
	});
	OnCollectionReset();
}



Ptr<PropertyValue> ObservableCollection::GetProperty( const std::string& str )
{
	if (str == "Count") {
		return new PropertyValue(Count());
	}
	return nullptr;
}

void ObservableCollection::AddPropertyChangedHandler( const Ptr<IPropertyChangedHandler>& handler )
{
	property_observers_.AddObserver(handler);
}

void ObservableCollection::RemovePropertyChangedHandler( const Ptr<IPropertyChangedHandler>& handler )
{
	property_observers_.RemoveObserver(handler);
}

void ObservableCollection::NotifyPropertyChanged( const Ptr<IPropertyObject>& sender, const Ptr<PropertyChangedArgs>& args )
{
	FOR_EACH_HANDLER(IPropertyChangedHandler, property_observers_, OnPropertyChanged(sender, args));
}

const std::vector<Ptr<IObject>>& ObservableCollection::GetItems() const
{
	return items_;
}


