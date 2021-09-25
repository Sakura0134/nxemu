#pragma once
#include <string>

enum OpenGLExpressionType 
{
    OpenGLExpressionType_Void,
    OpenGLExpressionType_Bool,
    OpenGLExpressionType_Float,
    OpenGLExpressionType_Int,
    OpenGLExpressionType_Uint,
};

class OpenGLExpression 
{
public:
    OpenGLExpression();
    OpenGLExpression(const char * Code, OpenGLExpressionType Type);

    OpenGLExpressionType GetType() const { return m_Type; }
    const char * GetCode() const { return m_Code.c_str(); }

    std::string As(OpenGLExpressionType Type);
    std::string AsBool() const;
    std::string AsFloat() const;
    std::string AsInt() const;
    std::string AsUint() const;
    void CheckVoid() const;

private:
    std::string m_Code;
    OpenGLExpressionType m_Type;
};
