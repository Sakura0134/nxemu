#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\Machine\Arm64Opcode.h>

class CPUExecutor;

class CRegisters
{
    friend CPUExecutor;

public:
    
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
    typedef union
    {
        uint32_t value;

        struct
        {
            unsigned SP : 1;
            unsigned nRW : 1;
            unsigned EL : 2;
            unsigned IL : 1;
            unsigned SS : 1;
            unsigned F : 1;
            unsigned I : 1;
            unsigned A : 1;
            unsigned D : 1;
            unsigned V : 1;
            unsigned C : 1;
            unsigned Z : 1;
            unsigned N : 1;
        };
    } PSTATE;
#pragma warning(pop)

    CRegisters(CPUExecutor * Executor);

    uint32_t Get32(Arm64Opcode::arm64_reg reg);
    uint64_t Get64(Arm64Opcode::arm64_reg reg);

    void Set64(Arm64Opcode::arm64_reg reg, uint64_t value);

    void SetConditionFlags(bool n, bool z, bool c, bool v);
    bool ConditionSet(Arm64Opcode::arm64_cc cc);

    static bool Is64bitReg(Arm64Opcode::arm64_reg reg);

private:
    CRegisters(void);                         // Disable default constructor
    CRegisters(const CRegisters&);            // Disable copy constructor
    CRegisters& operator=(const CRegisters&); // Disable assignment

    uint64_t m_PROGRAM_COUNTER;
    uint64_t m_xregs[32];
    uint32_t * m_wregs[32];

    PSTATE m_pstate;

    CPUExecutor * m_Executor;
};

