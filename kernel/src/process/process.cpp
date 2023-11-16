#include <process/process.h>

Process::Process( void )
: _processId(0), _processInfo(0)
{
    Thread* initialThread = new Thread(*this);
    _threads.Add(*initialThread);
}