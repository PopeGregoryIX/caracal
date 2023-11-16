#include <object/objectmanager.h>
#include <debug.h>

ObjectManager ObjectManager::_instance;

handle_t ObjectManager::RegisterObject(KernelObject& object)
{
    if(object._handle != 0) FATAL("Attempt to register an object which has already been allocated a handle");

    object._handle = ++_nextHandle;
    _objects.Add(object);

    return object._handle;
}