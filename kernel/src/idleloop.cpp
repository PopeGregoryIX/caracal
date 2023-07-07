#include <caracal.h>
#include <debug/debug.h>
#include <debug/debugconsole.h>
#include <process/userfunctions.h>
#include <process/processmanager.h>

void IdleLoop( void )
{
    DebugConsole& console = DebugConsole::GetInstance();
    ProcessManager processManager = ProcessManager::GetInstance();

    INFO("Idle loop entered for current thread.");
    console.PutHex(processManager.GetRunningThread()->GetId());
    console.PutChar('\n');
    for(;;) 
    { 
        //UserFunctions::GetInstance().DoSyscall(CALL_YIELD);
    }
}