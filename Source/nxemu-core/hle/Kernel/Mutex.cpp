#include <nxemu-core\hle\Kernel\Mutex.h>
#include <nxemu-core\hle\Kernel\SystemThread.h>
#include <nxemu-core\SystemGlobals.h>

CMutex::CMutex()
{
}

ResultCode CMutex::Release(uint64_t MutexAddress, CSystemThread * HoldingThread)
{
    if ((MutexAddress % sizeof(uint32_t)) != 0)
    {
        return ERR_INVALID_ADDRESS;
    }

    WaitingThread Waiting = GetHighestPriorityMutexWaitingThread(MutexAddress, HoldingThread);
    CSystemThread * Thread = Waiting.first;
    uint32_t NumWaiters = Waiting.second;
    if (Thread == nullptr)
    {
        HoldingThread->ThreadMemory().Write32(MutexAddress, 0);
        return RESULT_SUCCESS;
    }

    TransferMutexOwnership(MutexAddress, HoldingThread, Thread);
    uint32_t MutexValue = Thread->GetWaitHandle();
    if (NumWaiters > 1)
    {
        MutexValue |= MutexHasWaitersFlag;
    }

    Thread->ThreadMemory().Write32(MutexAddress, MutexValue);
    if (Thread->GetState() != CSystemThread::ThreadState_WaitMutex)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return RESULT_SUCCESS;
    }
    Thread->SetLockOwner(nullptr);
    Thread->SetCondVarWaitAddress(0);
    Thread->SetMutexWaitAddress(0);
    Thread->SetWaitHandle(0);
    Thread->SetState(CSystemThread::ThreadState_Ready);
    Thread->TriggerSyncEvent();
    return RESULT_SUCCESS;
}

ResultCode CMutex::Acquire(uint64_t MutexAddress, uint32_t HoldingThreadHandle, CSystemThread * HoldingThread, uint32_t RequestingThreadHandle, CSystemThread * CurrentThread)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return RESULT_SUCCESS;
}


CMutex::WaitingThread CMutex::GetHighestPriorityMutexWaitingThread(uint64_t Address, CSystemThread * Thread)
{
    const CSystemThread::MutexWaitingThreads & WaitingThreads = Thread->GetWaitMutexThreads();
    CSystemThread * HighestPriorityThread = nullptr;
    uint32_t Waiting = 0;

    for (CSystemThread::MutexWaitingThreads::const_iterator itr = WaitingThreads.begin(); itr != WaitingThreads.end(); itr++)
    {
        CSystemThread * WaitingThread = *itr;
        if (WaitingThread->GetMutexWaitAddress() != Address)
        {
            continue;
        }

        if (WaitingThread->GetState() != CSystemThread::ThreadState_WaitMutex)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        Waiting += 1;
        if (HighestPriorityThread == nullptr || WaitingThread->GetPriority() < HighestPriorityThread->GetPriority())
        {
            HighestPriorityThread = WaitingThread;
        }
    }
    return WaitingThread(HighestPriorityThread, Waiting);
}

void CMutex::TransferMutexOwnership(uint64_t MutexAddress, CSystemThread * CurrentThread, CSystemThread * NewOwner)
{
    const CSystemThread::MutexWaitingThreads WaitingThreads = CurrentThread->GetWaitMutexThreads();
    for (CSystemThread::MutexWaitingThreads::const_iterator itr = WaitingThreads.begin(); itr != WaitingThreads.end(); itr++)
    {
        CSystemThread * WaitingThread = *itr;
        if (WaitingThread->GetMutexWaitAddress() != MutexAddress)
        {
            continue;
        }

        if (WaitingThread->GetLockOwner() != CurrentThread)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        CurrentThread->RemoveMutexWaiter(WaitingThread);
        if (NewOwner != WaitingThread)
        {
            NewOwner->AddMutexWaiter(WaitingThread);
        }
    }
}
