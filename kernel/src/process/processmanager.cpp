#include <stdint.h>
#include <stddef.h>
#include <process/processmanager.h>
#include <process/process.h>
#include <process/thread.h>

ProcessManager ProcessManager::_instance;

void ProcessManager::Initialise( Process& initProcess )
{
    (void)initProcess;
}
