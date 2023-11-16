#ifndef __THREAD__H__
#define __THREAD__H__

#include <archdef.h>

class Thread
{
    public:
        inline threadId_t GetId( void ) { return _threadId; }
        inline threadInfo_t GetInfo( void ) { return _threadInfo; }

    private:
        Thread(class Process& parent);

        threadId_t _threadId;
        threadInfo_t _threadInfo;

        class Process& _parent;
        
    friend class Process;
    friend class TaskManager;
};

#endif