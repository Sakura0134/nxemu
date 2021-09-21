#include "ShaderCharacteristics.h"

ShaderCharacteristics::ShaderCharacteristics() : 
    m_Start(0),
    m_End(0),
    m_Depth(ShaderCompileDepth_FlowStack)
{
}

ShaderCharacteristics::~ShaderCharacteristics()
{
}

ShaderBlock & ShaderCharacteristics::AddBlock(uint32_t Start, uint32_t End)
{
    m_Blocks.emplace_back(ShaderBlock(Start, End));
    return m_Blocks[m_Blocks.size() - 1];
}

bool ShaderCharacteristics::HasLabel(uint32_t Label)
{
    return m_Labels.find(Label) != m_Labels.end();
}

void ShaderCharacteristics::ReplaceLabels(Labels && NewLabels)
{
    m_Labels = NewLabels;
}
