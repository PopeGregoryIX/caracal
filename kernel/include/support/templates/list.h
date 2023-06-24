/*
 * list.h
 *
 *  Created on: 24 Jun 2023
 *      Author: mail
 */

#ifndef KERNEL_INCLUDE_SUPPORT_TEMPLATES_LIST_H_
#define KERNEL_INCLUDE_SUPPORT_TEMPLATES_LIST_H_

#include <stdint.h>
#include <support/templates/container.h>

template<class T>
class List : public Container<T>
{
	private:
		typedef struct listItem
		{
			T data;
			List<T>::listItem* next;

			listItem(T d) : data(d), next(nullptr){}
		} listItem_t;

	public:
		void Add(T item){ Add(new listItem_t(item)); }

		void Remove(T& item)
		{
			auto i = Find(item);
			if(i == nullptr) FATAL("Attempt to remove item which does not exist in the list.");
			Remove(i);
			delete i;
		}

		T* GetFirst()	{	return &(first_->data);	}

		T* GetNext(T* current)
		{
			auto item = Find(*current);

			if(item != nullptr && item->next != nullptr) return &(item->next->data);

			return nullptr;
		}

		typedef Iterator<List, listItem_t> iterator_t;
	private:
		void Add(listItem_t* item)
		{
			if(first_ == nullptr)
			{
				first_ = last_ =  item;
				item->next = nullptr;
			}
			else
			{
				last_->next = item;
				item->next = nullptr;
				last_ = last_->next;
			}

			this->_count++;
		}

		listItem_t* FindParent(listItem_t* item)
		{
			listItem_t* i = first_;
			listItem_t* p = nullptr;

			while(i != nullptr)
			{
				if(i == item) return p;
				p = i;
				i = i->next;
			}

			return nullptr;
		}

		listItem_t* Find(T& d)
		{
			listItem_t* i = first_;

			while(i != nullptr)
			{
				if(i->data == d) return i;
				i = i->next;
			}

			return nullptr;
		}

		void Remove(listItem_t* item)
		{
			if(first_ == nullptr) FATAL("Attempt to remove item from empty list.");

			if(item == first_)
			{
				first_ = item->next;
				if(item == last_) last_ = item->next;
				item->next = nullptr;
			}
			else
			{
				listItem_t* parent = FindParent(item);
				if(parent == nullptr) FATAL("Attempt to remove item that does not exist in list.");
				parent->next = item->next;
				item->next = nullptr;
				if(item == last_) last_ = parent;
			}

			this->_count--;
		}

		listItem_t* first_;
		listItem_t* last_;
};


#endif /* KERNEL_INCLUDE_SUPPORT_TEMPLATES_LIST_H_ */
