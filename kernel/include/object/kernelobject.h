#ifndef __KERNELOBJECT__H__
#define __KERNELOBJECT__H__

#include <kernel.h>
#include <structures/string.h>

class KernelObject
{
    protected:
        /**
         * \brief Early constructor - *avoid where possible*
         * This allocates no memory, but also requires manual registration with
         * the Object Manager.
        */
        KernelObject( void ) {}

        /**
         * \brief Preferred constructor.
         * 
         * Registers the object and the name with the ObjectManager.
        */
        KernelObject(const String& name);

        ~KernelObject();
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
