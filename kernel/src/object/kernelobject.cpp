#include <object/kernelobject.h>
#include <object/objectmanager.h>
#include <structures/string.h>

KernelObject::KernelObject(const String& name)
{
    _name = name;
    ObjectManager::GetInstance().RegisterObject(*this);
}

KernelObject::~KernelObject( void )
{
    
}
