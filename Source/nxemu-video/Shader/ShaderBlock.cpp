#include "ShaderBlock.h"

ShaderBlock::ShaderBlock(uint32_t Start, uint32_t End) :
    m_Start(Start),
    m_End(End),
    m_IgnoreBranch(true)
{
}
