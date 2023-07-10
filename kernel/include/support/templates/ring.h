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
                    : Iterator<Ring<TData>, TData>(container, initial), _initial(initial), _wrapped(false){}

                bool IsWrapped( void )  {  return _wrapped; }

                inline Iterator<Ring<TData>,TData> operator++( void )
                {
                    _current = _current.Next();
                    if(_current == _initial) _wrapped = true;
                    return this;
                }
            private:
                typename Container<TData>::ContainerItem& _initial;
                bool _wrapped;
        };

        void Add(TData data)
        {
            RingItem* newItem = new RingItem(data);
            _lock.Acquire();
            if(_current == nullptr)
            {
                _current = newItem;
                _current->_next = _current;  
                _current->_prev = _current; 
            }
            else
            {
                newItem->_next = _current->_next;
                _current->_next = newItem;

                newItem->_prev = _current;
                newItem->_next->_prev = newItem;
            }
            Container<TData>::_count++;
            _lock.Release();
        }

        void Remove(TData& data)
        {
            for(RingIterator i = GetIterator(); !(i.IsWrapped()); i++)
            {
                if(*i == data)
                {
                    _lock.Acquire();
                    RingItem item = static_cast<RingItem>(i.Current());
                    if(item->_next == &item)
                        _current = nullptr;    
                    else
                    {
                        item._next->_prev = item._prev;
                        item._prev->_next = item._next;
                    }

                    delete item;
                    Container<TData>::_count--;
                    _lock.Release();
                }
            }
        }

        inline Iterator<Ring<TData>, TData> GetIterator( void ) {   return RingIterator(*this, *_current); }
    private:
        struct RingItem : public Container<TData>::ContainerItem
        {
            RingItem(TData data) : Container<TData>::ContainerItem(data) {}
            RingItem* _next;
            RingItem* _prev;

            typename Container<TData>::ContainerItem& Next( void ) { return *_next; }
            typename Container<TData>::ContainerItem& Previous( void ) { return *_prev; }
        };

        RingItem* _current;
        Spinlock _lock;
};

#endif
