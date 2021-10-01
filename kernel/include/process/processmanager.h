#ifndef __PROCESSMANAGER__H__
#define __PROCESSMANAGER__H__

#include <process/process.h>
#include <process/thread.h>

class ProcessManager
{
    private:
        static ProcessManager _instance;
        
    public:
        static ProcessManager& GetInstance( void ) { return _instance;  }

        void Initialise( Process& initProcess );

        /**
         * @brief Get the current Process object executing on the current core.
         * 
         * @return A reference to the current process.
         */
        Process& GetCurrentProcess( void );
};

#endif
