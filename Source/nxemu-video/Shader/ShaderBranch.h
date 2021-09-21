#pragma once
#include "Shader.h"
#include <stdint.h>

enum ShaderBranchType
{
    ShaderBranchType_Single,
    ShaderBranchType_Multi,
};

class ShaderBranch
{
public:
    enum : int32_t
    {
        ExitBranch = -1,
    };

    ShaderBranch();
    ShaderBranchType Type(void) const { return m_Type; }
    int32_t Address(void) const { return m_Address; }
    ShaderPredicate ConditionPredicate(void) const { return m_ConditionPredicate; }
    ShaderConditionCode ConditionCode(void) const { return m_ConditionCode; }
    bool Kill(void) const { return m_Kill; }
    bool IsSync(void) const { return m_IsSync; }
    bool IsBreak(void) const { return m_IsBreak; }
    bool Ignore(void) const { return m_Ignore; }

    void Type(ShaderBranchType Type);
    void Address(int32_t Address);
    void ConditionPredicate(ShaderPredicate ConditionPredicate);
    void ConditionCode(ShaderConditionCode ConditionCode);
    void Kill(bool Kill);
    void IsSync(bool IsSync);
    void IsBreak(bool IsBreak);
    void Ignore(bool Ignore);

private:
    ShaderBranchType m_Type;
    ShaderPredicate m_ConditionPredicate;
    ShaderConditionCode m_ConditionCode;
    int32_t m_Address;
    bool m_Kill;
    bool m_IsSync;
    bool m_IsBreak;
    bool m_Ignore;
};
