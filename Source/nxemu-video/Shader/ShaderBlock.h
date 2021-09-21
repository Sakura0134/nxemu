#pragma once
#include "ShaderBranch.h"
#include <stdint.h>
#include <vector>

class ShaderBlock 
{
public:
    ShaderBlock(uint32_t Start, uint32_t End);

    uint32_t Start(void) const { return m_Start; }
    uint32_t End(void) const { return m_End; }
    uint32_t IgnoreBranch(void) const { return m_IgnoreBranch; }
    const ShaderBranch & Branch(void) const { return m_Branch; }

    void IgnoreBranch(bool IgnoreBranch) { m_IgnoreBranch = IgnoreBranch; }
    void Branch(const ShaderBranch & Branch) { m_Branch = Branch; }

private:
    ShaderBlock(void);
    ShaderBlock& operator=(const ShaderBlock&);

    uint32_t m_Start;
    uint32_t m_End;
    bool m_IgnoreBranch;
    ShaderBranch m_Branch;
};
typedef std::vector<ShaderBlock> ShaderBlocks;
