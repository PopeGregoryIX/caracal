#include <object/kernelobject.h>
#include <object/objectmanager.h>
#include <structures/string.h>

KernelObject::KernelObject(const String& name) : _name(name), _handle(0)
{

    ObjectManager::GetInstance().RegisterObject(*this);
}

KernelObject::~KernelObject( void )
{
    
}
