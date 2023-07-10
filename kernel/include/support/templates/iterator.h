#ifndef __ITERATOR__H_
#define __ITERATOR__H_

#include <support/templates/container.h>

/**
 * @brief A generic iterator class.
 * 
 * @tparam U The type of the *container* being iterated on.
 * @tparam T The type of the *data* being iterated on.
 */
template <class TContainer, class TData>
class Iterator
{
    protected:
        TContainer& _container;
        typename Container<TData>::ContainerItem& _current;

    public:
        Iterator(TContainer& container, typename Container<TData>::ContainerItem initial)
        : _container(container), _current(initial)
        {
        }

        inline size_t Count( void ) {   return _container.Count();  }

        inline typename Container<TData>::ContainerItem& Current( void ) {  return _current;    }

        inline TData& operator*( void ) {   return Current()._data;  }
        
        inline virtual Iterator<TContainer,TData> operator++( void )
        {
            _current = _current.Next();
            return this;
        }
};

#endif
