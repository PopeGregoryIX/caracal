#include <stdint.h>
#include <stddef.h>
#include <debug.h>

namespace arch
{
    void PrintRegisters(const char* name1, uint64_t value1, const char* name2, uint64_t value2)
    {
        DebugConsole& debug = DebugConsole::GetInstance();

        debug.SetOutputColour(0x00FF00, 0x000000);
        debug.PutString(name1);
        debug.PutChar('\t');
        debug.SetOutputColour(0xFFFFFF, 0x000000);
        debug.PutPaddedHex(value1);
        debug.PutString("\t\t");
        debug.SetOutputColour(0x00FF00, 0x000000);
        debug.PutString(name2);
        debug.PutChar('\t');
        debug.SetOutputColour(0xFFFFFF, 0x000000);
        debug.PutPaddedHex(value2);
        debug.PutChar('\n');
    }
}