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
    
    initProcess._processId = ++_nextProcessId;
    Thread& initThread = initProcess._threads.GetFirst();
    initThread._threadId = ++_nextThreadId;

    _runningThreads.Add(initThread);
}

Thread& TaskManager::Yield( void )
{
    _scheduleLock.Acquire();
    //  Switch out the current thread and select a new one
    Thread& outgoingThread = arch::Machine::GetInstance().GetCurrentCpu().GetCurrentThread();

    INFO("Outgoing thread: " << (uint64_t)outgoingThread._threadId << " on process: " << outgoingThread.GetParent().GetName());
    
    _runningThreads.Remove(outgoingThread);
    _availableThreads.Add(outgoingThread);

    Thread& incomingThread = _availableThreads.GetFirst();
    _availableThreads.Remove(incomingThread);
    _runningThreads.Add(incomingThread);

    INFO("Incoming thread: " << (uint64_t)incomingThread._threadId << " on process: " << incomingThread.GetParent().GetName());

    _scheduleLock.Release();
    return incomingThread;
}