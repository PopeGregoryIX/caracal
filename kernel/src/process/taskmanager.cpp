#include <process/taskmanager.h>
#include <object/objectmanager.h>
#include <structures/string.h>
#include <arch/glue.h>
#include <process/process.h>
#include <arch/machine.h>
#include <arch/cpu.h>

TaskManager TaskManager::_instance;

void TaskManager::Initialise( void )
{
    _name = "Task Manager";
    ObjectManager::GetInstance().RegisterObject(*this);

    Process& initProcess = Glue::GenerateInitialProcess();
    FATAL("Init");
    initProcess._processId = ++_nextProcessId;
    initProcess._threads.GetFirst()._threadId = ++_nextThreadId;
}

Thread& TaskManager::Yield( void )
{
    //  Switch out the current thread and select a new one
    Thread& outgoingThread = arch::Machine::GetInstance().GetCurrentCpu().GetCurrentThread();

    INFO("Outgoing thread: " << outgoingThread.GetId() << " on process: " << outgoingThread.GetParent().GetName());

    return outgoingThread;
}