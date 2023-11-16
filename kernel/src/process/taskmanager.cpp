#include <process/taskmanager.h>
#include <object/objectmanager.h>
#include <structures/string.h>
#include <arch/glue.h>
#include <process/process.h>

TaskManager TaskManager::_instance;

void TaskManager::Initialise( void )
{
    _name = "Task Manager";
    ObjectManager::GetInstance().RegisterObject(*this);

    Process& initProcess = Glue::GenerateInitialProcess();
    initProcess._processId = ++_nextProcessId;
    initProcess._threads.GetFirst()._threadId = ++_nextThreadId;
}
