#ifndef __ITERATOR__H_
#define __ITERATOR__H_

/**
 * @brief A generic iterator class.
 * 
 * @tparam T The type of the *container* being iterated on.
 * @tparam U The type of the *data* being iterated on.
 */
template <class  T, class U>
class Iterator
{
private:
    size_t  _index;
    U*      _current;
    T&      _container;

public:
    Iterator(T& container)
    : _container(container)
    {
        _index = 0;
        _current = _container.GetFirst();
    }

    inline U& Current( void )  {   return *_current;   }

    bool MoveNext()
    {
        _current = _container.GetNext(_current);
        _index++;
        return (_current == nullptr) ? false : true;
    }
};

#endif
