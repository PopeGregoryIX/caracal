#ifndef __THREAD__H__
#define __THREAD__H__

#include <archdef.h>

class Process;

class Thread
{
    public:
        inline threadId_t GetId( void ) { return _threadId; }
        inline threadInfo_t GetInfo( void ) { return _threadInfo; }

        inline Process& GetParent( void ) { return _parent; }

        inline bool operator==(Thread& thread) { return _threadId == thread.GetId(); } 
        inline bool operator!=(Thread& thread) { return !(*this == thread); }
    private:
        Thread(class Process& parent);

        threadId_t _threadId;
        threadInfo_t _threadInfo;

        Process& _parent;
        
    friend class Process;
    friend class TaskManager;
    friend class Glue;
};

#endif