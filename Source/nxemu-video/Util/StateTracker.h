#pragma once
#include <stdint.h>
#include <vector>

class CStateTracker {
public:
    CStateTracker(uint32_t NumRegisters, uint32_t MaxFlag);

    void OnMemoryWrite(void);

    void FlagSet(uint32_t Index);
    
    void RegisterChanged(uint32_t Method);

private:
    CStateTracker();
    CStateTracker(const CStateTracker&);
    CStateTracker& operator=(const CStateTracker&);

    std::vector<bool> m_Flags;
    std::vector<bool> m_MemoryWriteFlag;
    std::vector<uint32_t> m_RegisterFlag[2];
};
