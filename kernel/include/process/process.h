#ifndef __PROCESS__H__
#define __PROCESS__H__

#include <archdef.h>
#include <structures/linkedlist.h>
#include <process/thread.h>

class Process
{
    public:
        inline processId_t GetProcessId( void ) { return _processId; }
        inline processInfo_t GetProcessInfo( void ) { return _processInfo; }

    private:
        Process( void );

        processId_t _processId;
        processInfo_t _processInfo;

        LinkedList<class Thread> _threads;

        friend class TaskManager;
        friend class Glue;
};

#endif