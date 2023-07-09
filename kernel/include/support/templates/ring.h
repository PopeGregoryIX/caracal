#ifndef __RING__H__
#define __RING__H__

#include <support/templates/container.h>
#include <support/templates/iterator.h>
#include <memory/spinlock.h>

template<class TData>
class Ring : Container<TData>
{
    public:
        class RingIterator : Iterator<Ring<TData>, TData>
        {
            public:
                RingIterator(Ring& container, typename Container<TData>::ContainerItem initial)
                    : Iterator<Ring<TData>, TData>(container, initial), _initial(initial), _wrap(false){}

            private:
                typename Container<TData>::ContainerItem& _initial;
                bool _wrap;
        };

        void Add(TData data)
        {
            RingItem* newItem = new RingItem(data);
            _lock.Acquire();
            if(_current == nullptr)
            {
                _current = newItem;
                _current->_next = _current;   
            }
            else
            {
                newItem->_next = _current->_next;
                _current->_next = newItem;
            }
            Container<TData>::_count++;
            _lock.Release();
        }

        void Remove(TData data)
        {
            auto i = GetIterator();

        }

        inline Iterator<Ring<TData>, TData> GetIterator( void ) {   return RingIterator(*this, *_current); }
    private:
        struct RingItem : public Container<TData>::ContainerItem
        {
            RingItem(TData data) : Container<TData>::ContainerItem::_data(data) {}
            RingItem* _next;

            typename Container<TData>::ContainerItem Next( void ) { return _next; }
        };

        RingItem* _current;
        Spinlock _lock;
};

#endif
