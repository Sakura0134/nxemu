#include "OpenGLShaderDecompiler.h"
#include "VideoNotification.h"

OpenGLShaderConstBuffer::OpenGLShaderConstBuffer(uint32_t MaxOffset, bool IsIndirect, uint32_t Index) :
    ShaderConstBuffer(MaxOffset, IsIndirect),
    m_Index(Index)
{
}

std::string OpenGLDecompileShader(const OpenGLDevice & /*Device*/, const ShaderIR & /*IR*/, ShaderType /*Stage*/, const char * /*Identifier*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return "";
}