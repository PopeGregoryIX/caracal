#ifndef __LINKEDLIST__H__
#define __LINKEDLIST__H__

#include <stdint.h>
#include <stddef.h>
#include <memory/spinlock.h>
#include <debug.h>

template <class T>
class LinkedList
{
    public:
        LinkedList( void ) : _firstNode(nullptr), _count(0) {}

        void Add(T& object)
        {
            LinkedListNode* newNode = new LinkedListNode(object);
            _lock.Acquire();
            if(_lastNode == nullptr && _firstNode == nullptr)
                _firstNode = newNode;
            else
                _lastNode->next = newNode;

            _lastNode = newNode;
            _count++;
            _lock.Release();
        }

        inline size_t Count( void ) { return _count; }
        inline T& GetFirst( void ) { if(_count == 0) FATAL("GetFirst used on an empty linked list."); return _firstNode->data; }

    private:
        struct LinkedListNode
        {
            LinkedListNode(T& data) : data(data), next(nullptr){ }

            T& data;
            LinkedListNode* next;
        };
        
        Spinlock _lock;

        LinkedListNode* _firstNode;
        LinkedListNode* _lastNode;

        size_t _count;
};

#endif
