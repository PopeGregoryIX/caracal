#include <process/thread.h>
#include <process/process.h>

Thread::Thread(Process& parent) : _parent(parent), _threadId(0), _threadInfo(nullptr) {}