#include "ShaderBranch.h"

ShaderBranch::ShaderBranch(void) :
    m_Type(ShaderBranchType_Single),
    m_ConditionPredicate(ShaderPredicate_UnusedIndex),
    m_ConditionCode(ShaderConditionCode_T),
    m_Address(ExitBranch),
    m_Kill(false),
    m_IsSync(false),
    m_IsBreak(false),
    m_Ignore(false)
{
}

void ShaderBranch::Type(ShaderBranchType Type) 
{
    m_Type = Type;
}

void ShaderBranch::Address(int32_t Address) 
{
    m_Address = Address;
}

void ShaderBranch::ConditionPredicate(ShaderPredicate ConditionPredicate)
{
    m_ConditionPredicate = ConditionPredicate;
}

void ShaderBranch::ConditionCode(ShaderConditionCode ConditionCode)
{
    m_ConditionCode = ConditionCode;
}

void ShaderBranch::Kill(bool Kill)
{
    m_Kill = Kill;
}

void ShaderBranch::IsSync(bool IsSync)
{
    m_IsSync = IsSync;
}

void ShaderBranch::IsBreak(bool IsBreak)
{
    m_IsBreak = IsBreak;
}

void ShaderBranch::Ignore(bool Ignore)
{
    m_Ignore = Ignore;
}
