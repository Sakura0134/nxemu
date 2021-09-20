#include "ShaderIR.h"
#include "VideoNotification.h"

ShaderConstBuffer::ShaderConstBuffer() :
    m_MaxOffset(0),
    m_IsIndirect(false)
{
}

ShaderConstBuffer::ShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect) :
    m_MaxOffset(MaxOffset),
    m_IsIndirect(IsIndirect)
{
}

ShaderIR::ShaderIR(const ShaderProgramCode & ProgramCode) :
    m_ProgramCode(ProgramCode)
{
    std::memcpy(&m_Header, m_ProgramCode.data(), sizeof(ShaderHeader));
    Decode();
}

ShaderIR::~ShaderIR()
{
}

void ShaderIR::Decode(void)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}
