#include <process/process.h>
#include <structures/string.h>
#include <object/objectmanager.h>
#include <object/kernelobject.h>

Process::Process( const String& name )
: KernelObject(name), _processId(0), _processInfo(0)
{
    Thread* initialThread = new Thread(*this);
    
    _threads.Add(*initialThread);
}