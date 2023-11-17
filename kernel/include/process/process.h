#ifndef __PROCESS__H__
#define __PROCESS__H__

#include <archdef.h>
#include <structures/linkedlist.h>
#include <process/thread.h>
#include <structures/string.h>

class Process
{
    public:
        inline processId_t GetId( void ) { return _processId; }
        inline processInfo_t GetInfo( void ) { return _processInfo; }
        inline String GetName( void ) { return _name; }
        
    private:
        Process( const String& name );

        processId_t _processId;
        processInfo_t _processInfo;

        LinkedList<class Thread> _threads;

        String _name;

        friend class TaskManager;
        friend class Glue;
};

#endif