#ifndef __DEBUG_CONSOLE__H__
#define __DEBUG_CONSOLE__H__

#include <debug/consoleoutput.h>
#include <spinlock.h>

class DebugConsole
{
    public:
        inline void LockConsole( void )     {   _processLock.Acquire();    }
        inline void UnlockConsole( void )   {   _processLock.Release();    }

        void AddOutputDevice(ConsoleOutput& device);
        
        void SetOutputColour(uint32_t foreground, uint32_t background);
	    inline void SetOutputColour(ConsoleColour c) { SetOutputColour(c.Foreground, c.Background); }
	    
        void Cls( void );

        void PutBinary(uint64_t b);
        void PutChar(const char c);
        void PutDecimal(uint64_t d);
        void PutHex(uint64_t h);
        void PutPaddedHex(uint64_t h);
        void PutString(const char* s);

	    ~DebugConsole(){}

        inline DebugConsole& operator<<(const char c) { PutChar(c); return *this; }
        inline DebugConsole& operator<<(const char* s) { PutString(s); return *this; }
        inline DebugConsole& operator<<(uint64_t i) { PutHex(i); return *this; }
        inline DebugConsole& operator<<(ConsoleColour c) { SetOutputColour(c); return *this; }
    private:
        Spinlock _characterLock;
        Spinlock _processLock;
        Spinlock _stringLock;

        static const int MAXOUTPUTDEVICES = 3;
	    ConsoleOutput* _outputDevices[MAXOUTPUTDEVICES];
};

#endif
