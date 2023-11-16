#ifndef __OBJECTMANAGER_H__
#define __OBJECTMANAGER_H__

#include <structures/linkedlist.h>
#include <object/kernelobject.h>

class ObjectManager
{
    public:
        static inline ObjectManager& GetInstance( void ) { return _instance; }

        ObjectManager() : _nextHandle(0) {}

        handle_t RegisterObject(KernelObject& object);
    private:
        static ObjectManager _instance;
        
        handle_t _nextHandle;
        LinkedList<KernelObject> _objects;
};

#endif
