#ifndef __CONTAINER_H
#define __CONTAINER_H

#include <stdint.h>
#include <stddef.h>

template<class TData>
class Container
{
protected:
    size_t _count;
public:
    inline Container() : _count(0){}
    inline virtual  size_t Count( void ) { return _count; }

    struct ContainerItem
    {
        TData _data;
        virtual ContainerItem Next( void ) = 0;
    };
    
};

#endif