#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__

#include <object/kernelobject.h>
#include <structures/linkedlist.h>
#include <archdef.h>

class TaskManager : public KernelObject
{
    public:
        static inline TaskManager& GetInstance( void ) { return _instance; }

        TaskManager( void ) : _nextProcessId(0), _nextThreadId(0) {}

        void Initialise( void );

        class Thread& Yield( void );
    private:
        static TaskManager _instance;

        processId_t _nextProcessId;
        threadId_t _nextThreadId;

        LinkedList<class Process> _processes;
};

#endif
