#pragma once
#include <vector>
#include "ModelCore/Ptr.h"

template <class HandlerType>
class HandlerList
{
public:
	typedef Ptr<HandlerType> HandlerPtr;
	// An iterator class that can be used to access the list of observers.  See
	// also the FOR_EACH_OBSERVER macro defined below.
	class Iterator {
	public:
		Iterator(HandlerList<HandlerType>& list)
			: list_(&list),
			index_(0),
			max_index_(list.size()) {
				++list_->notify_depth_;
		}

		~Iterator() {
			if (list_ && --list_->notify_depth_ == 0)
				list_->Compact();
		}

		HandlerPtr GetNext() {
			if (!list_)
				return NULL;
			ListType& observers = list_->observers_;
			// Advance if the current element is null
			size_t max_index = max_index_;
			while (index_ < max_index && !observers[index_])
				++index_;
			return index_ < max_index ? observers[index_++] : NULL;
		}

	private:
		HandlerList<HandlerType>* list_;
		size_t index_;
		size_t max_index_;
	};

	explicit HandlerList() : notify_depth_(0) {}

	~HandlerList() {}

	// Add an observer to the list.  An observer should not be added to
	// the same list more than once.
	void AddObserver(HandlerPtr obs) {
		if (std::find(observers_.begin(), observers_.end(), obs)
			!= observers_.end()) {
				//assert(0);
				//"Observers can only be added once!";
				return;
		}
		observers_.push_back(obs);
	}

	// Remove an observer from the list if it is in the list.
	void RemoveObserver(HandlerPtr obs) {
		typename ListType::iterator it =
			std::find(observers_.begin(), observers_.end(), obs);
		if (it != observers_.end()) {
			if (notify_depth_) {
				*it = 0;
			} else {
				observers_.erase(it);
			}
		}
	}

	bool HasObserver(HandlerPtr observer) const {
		for (size_t i = 0; i < observers_.size(); ++i) {
			if (observers_[i] == observer)
				return true;
		}
		return false;
	}

	void Clear() {
		if (notify_depth_) {
			for (typename ListType::iterator it = observers_.begin();
				it != observers_.end(); ++it) {
					*it = 0;
			}
		} else {
			observers_.clear();
		}
	}

	size_t size() const { return observers_.size(); }

protected:
	void Compact() {
		observers_.erase(
			std::remove(observers_.begin(), observers_.end(),
			static_cast<HandlerType*>(NULL)), observers_.end());
	}

private:
	typedef std::vector<HandlerPtr> ListType;

	ListType observers_;
	int notify_depth_;
	friend class HandlerList::Iterator;

	HandlerList(const HandlerList&);
	void operator=(const HandlerList&);	
};


#define FOR_EACH_HANDLER(HandlerType, observer_list, func)			\
do {																	\
	if ((observer_list).size()) {										\
		HandlerList<HandlerType>::Iterator it(observer_list);	\
		Ptr<HandlerType> obs;												\
		while ((obs = it.GetNext()) != NULL)							\
			obs->func;													\
	}																	\
} while (0)