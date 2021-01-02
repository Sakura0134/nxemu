#pragma once
#include <nxemu-core\hle\Kernel\ResultCode.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>

class CMutex
{
    enum
    {
        MutexHasWaitersFlag = 0x40000000
    };

public:
    CMutex();

    ResultCode Release(uint64_t MutexAddress, CSystemThread * Thread);
    ResultCode Acquire(uint64_t MutexAddress, uint32_t HoldingThreadHandle, CSystemThread * HoldingThread, uint32_t RequestingThreadHandle, CSystemThread * RequestingThread);

private:
    CMutex(const CMutex&);
    CMutex& operator=(const CMutex&);

    typedef std::pair<CSystemThread *, uint32_t> WaitingThread;
        
    WaitingThread GetHighestPriorityMutexWaitingThread(uint64_t Address, CSystemThread * Thread);
    void TransferMutexOwnership(uint64_t MutexAddress, CSystemThread * CurrentThread, CSystemThread * NewOwner);
};
