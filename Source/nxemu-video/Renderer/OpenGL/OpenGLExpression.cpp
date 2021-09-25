#include "OpenGLExpression.h"
#include "VideoNotification.h"
#include <Common\StdString.h>

OpenGLExpression::OpenGLExpression() :
    m_Type(OpenGLExpressionType_Void)
{
}

OpenGLExpression::OpenGLExpression(const char * Code, OpenGLExpressionType Type) : 
    m_Code(Code),
    m_Type(Type) 
{
}

std::string OpenGLExpression::As(OpenGLExpressionType Type)
{
    switch (Type)
    {
    case OpenGLExpressionType_Float: return AsFloat();
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return m_Code;
}

std::string OpenGLExpression::AsBool() const
{
    switch (m_Type) 
    {
    case OpenGLExpressionType_Bool: return m_Code;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return m_Code;
}

std::string OpenGLExpression::AsFloat() const 
{
    switch (m_Type) 
    {
    case OpenGLExpressionType_Float: return m_Code;
    case OpenGLExpressionType_Uint: return stdstr_f("utof(%s)",m_Code.c_str());
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return m_Code;
}

std::string OpenGLExpression::AsInt() const 
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return m_Code;
}

std::string OpenGLExpression::AsUint() const
{
    switch (m_Type) 
    {
    case OpenGLExpressionType_Float: return stdstr_f("ftou(%s)",m_Code.c_str());
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return m_Code;
}

void OpenGLExpression::CheckVoid() const 
{
    if (m_Type != OpenGLExpressionType_Void)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}
