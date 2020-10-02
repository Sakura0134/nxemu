#pragma once
#include <Common\stdtypes.h>
#include <nxemu-core\Machine\Arm64Opcode.h>
extern "C" {
#include <softfloat.h>
}

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

__interface IRegisters
{
    uint32_t Get32(Arm64Opcode::arm64_reg reg) = 0;
    uint64_t Get64(Arm64Opcode::arm64_reg reg) = 0;
    void Get128(Arm64Opcode::arm64_reg reg, uint64_t& hiValue, uint64_t& loValue) = 0;
    PSTATE GetPstate() const = 0;

    void Set32(Arm64Opcode::arm64_reg reg, uint32_t value) = 0;
    void Set64(Arm64Opcode::arm64_reg reg, uint64_t value) = 0;
    void Set64Float(Arm64Opcode::arm64_reg reg, float64_t value) = 0;
    void Set128(Arm64Opcode::arm64_reg reg, uint64_t hiValue, uint64_t loValue) = 0;
    void Set64Vector(Arm64Opcode::arm64_reg reg, int64_t VectorIndex, Arm64Opcode::arm64_vess Vess, Arm64Opcode::arm64_vas Vas, uint64_t value) = 0;
    void SetFPCR(uint32_t value) = 0;
    void SetFPSR(uint32_t value) = 0;

    void SetConditionFlags(bool n, bool z, bool c, bool v) = 0;
    bool ConditionSet(Arm64Opcode::arm64_cc cc) = 0;
};

class CInterpreterCPU;

class CRegisters :
    public IRegisters
{
    friend CInterpreterCPU;

public:
    
    CRegisters(CInterpreterCPU * Executor);

    uint32_t Get32(Arm64Opcode::arm64_reg reg);
    uint64_t Get64(Arm64Opcode::arm64_reg reg);
    void Get128(Arm64Opcode::arm64_reg reg, uint64_t & hiValue, uint64_t & loValue);
    PSTATE GetPstate() const;

    void Set32(Arm64Opcode::arm64_reg reg, uint32_t value);
    void Set64(Arm64Opcode::arm64_reg reg, uint64_t value);
    void Set64Float(Arm64Opcode::arm64_reg reg, float64_t value);
    void Set128(Arm64Opcode::arm64_reg reg, uint64_t hiValue, uint64_t loValue);
    void Set64Vector(Arm64Opcode::arm64_reg reg, int64_t VectorIndex, Arm64Opcode::arm64_vess Vess, Arm64Opcode::arm64_vas Vas, uint64_t value);
    void SetFPCR(uint32_t value);
    void SetFPSR(uint32_t value);

    void SetConditionFlags(bool n, bool z, bool c, bool v);
    bool ConditionSet(Arm64Opcode::arm64_cc cc);

private:
    CRegisters(void);
    CRegisters(const CRegisters&);
    CRegisters& operator=(const CRegisters&);

    uint64_t m_PROGRAM_COUNTER;
    uint64_t m_xregs[32];
    uint32_t * m_wregs[32];
    uint64_t m_vfp_regs[64];
    uint32_t m_fpcr;
    uint32_t m_fpsr;

    //CP15
    uint64_t m_tpidr_el0; /* User RW Thread register.  */
    uint64_t m_tpidrro_el0; /* User RO Thread register.  */

    PSTATE m_pstate;

    CInterpreterCPU * m_Executor;
};

