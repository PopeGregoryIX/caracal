#ifndef _DOUBLELINKEDLIST_H_
#define _DOUBLELINKEDLIST_H_

#include <stdint.h>
#include <stddef.h>
#include <support/templates/container.h>
#include <support/templates/iterator.h>

template<class T>
class DoubleLinkedList : public Container<T>
{
    public:
        typedef struct  doubleLinkedListItem
        {
            T data;
            DoubleLinkedList<T>::doubleLinkedListItem* next;
            DoubleLinkedList<T>::doubleLinkedListItem* prev;

            doubleLinkedListItem(T dataItem) : data(dataItem){}
        } listItem_t;

        DoubleLinkedList( void ) : _first(nullptr), _last(nullptr) {}

        inline void Add(T item) {   Add(new listItem_t(item));   }

        void Add(DoubleLinkedList<T>::doubleLinkedListItem item)
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

        inline size_t Bytes( void ) { return this->Count() * sizeof(DoubleLinkedList<T>::doubleLinkedListItem); }

        inline T* GetFirst( void ) {   return &_first->data;    }
        
        inline T* GetNext(T* current) 
        { 
            return &(((DoubleLinkedList<T>::doubleLinkedListItem*)(((uintptr_t)current) + sizeof(T)))->next->data); 
        }

        void Remove(T item)
        {
            DoubleLinkedList<T>::doubleLinkedListItem* i = Find(item);
            if(i == nullptr) FATAL("Double linked list item not found.");
            Remove(*i);
            delete *i;
        }

        void Remove(DoubleLinkedList<T>::doubleLinkedListItem item)
        {
            if((item.prev) != nullptr)
            {
                item.prev->next = item.next;
                if(_last == item) _last = item.prev;
            }
            if((item.next) != nullptr)
            {
                item.next->prev = item.prev;
                if(_first == item) _first = item.next;
            }

            item.prev = item.next = nullptr;
            this->_count--;
        }

        typedef Iterator<DoubleLinkedList, DoubleLinkedList<T>::doubleLinkedListItem> iterator_t;
        inline iterator_t GetIterator( void ) { return iterator_t(*this);   }
    protected:
        DoubleLinkedList<T>::doubleLinkedListItem* Find(T value)
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

        DoubleLinkedList<T>::doubleLinkedListItem* _first;
        DoubleLinkedList<T>::doubleLinkedListItem* _last;
};

#endif
