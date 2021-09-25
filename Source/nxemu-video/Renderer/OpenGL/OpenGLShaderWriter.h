#pragma once
#include <string>
#include <stdint.h>

class OpenGLShaderWriter 
{
public:
    OpenGLShaderWriter();

    void AddCode(_In_z_ _Printf_format_string_ const char* Text, ...);
    const char * Code(void) const;
    void ScopeIncrease();
    void ScopeDecrease();
    std::string TemporaryId();

private:
    OpenGLShaderWriter(const OpenGLShaderWriter&);
    OpenGLShaderWriter& operator=(const OpenGLShaderWriter&);

    void AppendIndentation(void);

    std::string m_Source;
    uint32_t m_Scope;
    uint32_t m_TemporaryIndex;
};
