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

        void Remove(T& object)
        {
            LinkedListNode* parent = FindParent(object);
            if(parent == nullptr)
            {
                LinkedListNode* node = Find(object);
                _firstNode = node->next;
                if(node == _lastNode) _lastNode = node->next;
                _count--;
            }
            else
            {
                parent->next = parent->next->next;
                if(parent->next == nullptr) _lastNode = parent;
                _count--;
            }
        }

    private:
        struct LinkedListNode
        {
            LinkedListNode(T& data) : data(data), next(nullptr){ }

            T& data;
            LinkedListNode* next;
        };
        
        LinkedListNode* Find(T& object) const
        {
            LinkedListNode* node = _firstNode;
            while(node != nullptr)
            {
                if(node->data == object) return node;
                node = node-> next;
            }

            FATAL("Linked List item not found.");
        }

        LinkedListNode* FindParent(T& object) const
        {
            LinkedListNode* node = _firstNode;
            LinkedListNode* parent = nullptr;
            while(node != nullptr)
            {
                if(node->data == object) return parent;
                parent = node;
                node = node-> next;
            }

            FATAL("Linked List item parent not found.");
        }

        Spinlock _lock;

        LinkedListNode* _firstNode;
        LinkedListNode* _lastNode;

        size_t _count;
};

#endif
