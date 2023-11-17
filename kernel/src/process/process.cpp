#include <process/process.h>
#include <structures/string.h>

Process::Process( const String& name )
: _processId(0), _processInfo(0)
{
    _name = name;
    Thread* initialThread = new Thread(*this);
    _threads.Add(*initialThread);
}