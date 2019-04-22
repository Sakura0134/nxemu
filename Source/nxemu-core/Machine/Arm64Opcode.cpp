#include <nxemu-core\Machine\Arm64Opcode.h>
#include <capstone\Include\capstone.h>
#include <nxemu-core\SystemGlobals.h>

Arm64Opcode::Arm64Opcode(uint64_t pc, uint32_t insn) :
    m_pc(pc)
{
    csh handle;
    cs_err err = cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
    if (err)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return;
    }
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    cs_insn *results;
    size_t count = cs_disasm(handle, (uint8_t *)&insn, sizeof(insn), pc, 0, &results);
    if (count == 0)
    {
        cs_close(&handle);
        return;
    }

    m_Name = results[0].mnemonic;
    m_Param = results[0].op_str;

    cs_free(results, count);
    cs_close(&handle);
}