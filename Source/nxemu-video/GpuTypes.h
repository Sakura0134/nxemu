#pragma once

enum 
{
    NumRenderTargets = 8,
};

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

enum EngineID 
{
    EngineID_FERMI_TWOD_A = 0x902D,
    EngineID_MAXWELL_B = 0xB197,
    EngineID_KEPLER_COMPUTE_B = 0xB1C0,
    EngineID_KEPLER_INLINE_TO_MEMORY_B = 0xA140,
    EngineID_MAXWELL_DMA_COPY_A = 0xB0B5,
};
