#pragma once

enum BufferMethods : unsigned 
{
    BufferMethods_BindObject = 0x0,
    BufferMethods_Nop = 0x2,
    BufferMethods_SemaphoreAddressHigh = 0x4,
    BufferMethods_SemaphoreAddressLow = 0x5,
    BufferMethods_SemaphoreSequence = 0x6,
    BufferMethods_SemaphoreTrigger = 0x7,
    BufferMethods_NotifyIntr = 0x8,
    BufferMethods_WrcacheFlush = 0x9,
    BufferMethods_Unk28 = 0xA,
    BufferMethods_UnkCacheFlush = 0xB,
    BufferMethods_RefCnt = 0x14,
    BufferMethods_SemaphoreAcquire = 0x1A,
    BufferMethods_SemaphoreRelease = 0x1B,
    BufferMethods_FenceValue = 0x1C,
    BufferMethods_FenceAction = 0x1D,
    BufferMethods_WaitForInterrupt = 0x1E,
    BufferMethods_Unk7c = 0x1F,
    BufferMethods_Yield = 0x20,
    BufferMethods_NonPuller = 0x40,
};
