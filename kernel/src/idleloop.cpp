#include <caracal.h>
#include <debug/debug.h>
#include <process/userfunctions.h>

void IdleLoop( void )
{
    INFO("Idle loop entered for current thread.");
    for(;;) { UserFunctions::GetInstance().DoSyscall(CALL_YIELD); }
}