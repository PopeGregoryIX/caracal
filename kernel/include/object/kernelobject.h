#ifndef __KERNELOBJECT__H__
#define __KERNELOBJECT__H__

#include <kernel.h>
#include <structures/string.h>

class KernelObject
{
    public:
        handle_t GetHandle( void ) { return _handle; }
        String& GetName( void ) { return _name; }

    protected:
        String _name;
        
    private:
        handle_t _handle;

        friend class ObjectManager;
};

#endif
