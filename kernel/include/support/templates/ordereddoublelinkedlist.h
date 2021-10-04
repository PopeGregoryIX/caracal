#ifndef _ORDEREDDOUBLELINKEDLIST_H_
#define _ORDEREDDOUBLELINKEDLIST_H_

#include <stdint.h>
#include <stddef.h>
#include "iterator.h"
#include "container.h"

template<class T>
class OrderedDoubleLinkedList : public Container<T>
{
    public:
        typedef struct  doubleLinkedListItem
        {
            T data;
            OrderedDoubleLinkedList<T>::doubleLinkedListItem* next;
            OrderedDoubleLinkedList<T>::doubleLinkedListItem* prev;

            doubleLinkedListItem(T dataItem) : data(dataItem){}
        } listItem_t;

        OrderedDoubleLinkedList( void ) : _first(nullptr), _last(nullptr) {}

        inline void Add(T item) {   Add(new listItem_t(item));   }

        void Add(OrderedDoubleLinkedList<T>::doubleLinkedListItem item)
        {
            if(_first == nullptr)
            {
                _first = &item;
                _last = &item;
                item.prev = item.next = nullptr;
            }
            else
            {
                _last->next = &item;
                item.prev = _last;
                item.next = nullptr;
                _last = &item;
            }

            this->_count++;
        }

        inline size_t Bytes( void ) { return this->Count() * sizeof(OrderedDoubleLinkedList<T>::doubleLinkedListItem); }

        inline T* GetFirst( void ) {   return &_first->data;    }
        
        inline T* GetNext(T* current) 
        { 
            if(current == nullptr || current == &(_last->data)) return nullptr;
            return &(((OrderedDoubleLinkedList<T>::doubleLinkedListItem*)(((uintptr_t)current) + sizeof(T)))->next->data); 
        }

        inline OrderedDoubleLinkedList<T>::doubleLinkedListItem* GetListItem(T* data)
        {
            return (OrderedDoubleLinkedList<T>::doubleLinkedListItem*)data;
        }

        void Remove(T item)
        {
            OrderedDoubleLinkedList<T>::doubleLinkedListItem* i = Find(item);
            if(i == nullptr) FATAL("Ordered Double linked list item not found.");
            Remove(*i);
            delete *i;
        }

        void Remove(OrderedDoubleLinkedList<T>::doubleLinkedListItem item)
        {
            if((item.prev) != nullptr)
            {
                item.prev->next = item.next;
                if(_last == &item) _last = item.prev;
            }
            if((item.next) != nullptr)
            {
                item.next->prev = item.prev;
                if(_first == &item) _first = item.next;
            }

            item.prev = item.next = nullptr;
            this->_count--;
        }

        typedef Iterator<OrderedDoubleLinkedList<T>, T> iterator_t;
        inline iterator_t GetIterator( void ) { return iterator_t(*this);   }
    protected:
        OrderedDoubleLinkedList<T>::doubleLinkedListItem* Find(T value)
        {
            if(this->Count() > 0)
            {
                iterator_t i = GetIterator();
                do
                {
                    if(i.Current().data == value) return i.Current();
                } while (i.MoveNext());
            }
            return nullptr;
        }

        OrderedDoubleLinkedList<T>::doubleLinkedListItem* _first;
        OrderedDoubleLinkedList<T>::doubleLinkedListItem* _last;
};

#endif
