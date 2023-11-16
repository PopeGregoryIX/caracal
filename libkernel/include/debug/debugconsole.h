#ifndef __DEBUG_CONSOLE__H__
#define __DEBUG_CONSOLE__H__

#include <debug/consoleoutput.h>
#include <memory/spinlock.h>

class DebugConsole
{
    public:
        enum NumericBase
        {
            BASE2 = 2,
            BASE10 = 10,
            BASE16 = 16
        };
    public:
        static inline DebugConsole& GetInstance( void ) {   return _instance;   }

        inline void LockConsole( void )     {   _processLock.Acquire();    }
        inline void UnlockConsole( void )   {   _processLock.Release();    }

        void AddOutputDevice(ConsoleOutput& device);
        
        void SetOutputColour(uint32_t foreground, uint32_t background);
	    inline void SetOutputColour(ConsoleColour c) { SetOutputColour(c.Foreground, c.Background); }
	    
        void SetNumericBase(NumericBase n) { _base = n; }

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
        inline DebugConsole& operator<<(uint64_t i) 
        { 
            switch (_base)
            {
                case BASE2:
                    PutBinary(i);
                    break;
                case BASE10:
                    PutDecimal(i);
                    break;
                case BASE16:
                    PutHex(i);
                    break;
            }
            return *this; 
        }

        inline DebugConsole& operator<<(ConsoleColour c) { SetOutputColour(c); return *this; }
        inline DebugConsole& operator<<(NumericBase n) { SetNumericBase(n); return *this; }
    private:
        Spinlock _characterLock;
        Spinlock _processLock;
        Spinlock _stringLock;

        NumericBase _base = BASE16;

        static const int MAXOUTPUTDEVICES = 3;
	    ConsoleOutput* _outputDevices[MAXOUTPUTDEVICES];

        static DebugConsole _instance;
};

#endif
