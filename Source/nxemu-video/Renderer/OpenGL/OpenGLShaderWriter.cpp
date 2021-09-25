#include "OpenGLShaderWriter.h"
#include "VideoNotification.h"
#include <stdarg.h>
#include <Common/StdString.h>

OpenGLShaderWriter::OpenGLShaderWriter() :
    m_Scope(0),
    m_TemporaryIndex(1)
{
}

void OpenGLShaderWriter::AddCode(_In_z_ _Printf_format_string_ const char* Text, ...)
{
    va_list args;
    va_start(args, Text);
#pragma warning(push)
#pragma warning(disable : 4996)
    size_t nlen = _vscprintf(Text, args) + 1;
    char* buffer = (char*)alloca(nlen * sizeof(char));
    buffer[nlen - 1] = 0;
    if (buffer != nullptr)
    {
        vsprintf(buffer, Text, args);
        if (strlen(buffer) > 0) 
        {
            AppendIndentation();
        }
        m_Source += buffer;
        m_Source += "\n";
    }
#pragma warning(pop)
    va_end(args);
}

const char * OpenGLShaderWriter::Code(void) const
{
    return m_Source.c_str();
}

void OpenGLShaderWriter::ScopeIncrease()
{
    m_Scope += 1;
}

void OpenGLShaderWriter::ScopeDecrease()
{
    if (m_Scope == 0) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
    m_Scope -= 1;
}

std::string OpenGLShaderWriter::TemporaryId()
{
    return stdstr_f("tmp%d", m_TemporaryIndex++);
}

void OpenGLShaderWriter::AppendIndentation()
{
    m_Source.append(m_Scope * 4, ' ');
}
