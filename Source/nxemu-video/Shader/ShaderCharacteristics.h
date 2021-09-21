#pragma once
#include "ShaderBlock.h"
#include <stdint.h>
#include <set>

enum ShaderCompileDepth
{
    ShaderCompileDepth_FlowStack,
    ShaderCompileDepth_NoFlowStack,
};

class ShaderCharacteristics 
{
public:
    typedef std::set<uint32_t> Labels;

    ShaderCharacteristics();
    ~ShaderCharacteristics();

    ShaderBlock & AddBlock(uint32_t Start, uint32_t End);
    bool HasLabel(uint32_t Label);
    void ReplaceLabels(Labels && NewLabels);

    void Start(uint32_t Start) { m_Start = Start; }
    void End(uint32_t End) { m_End = End; }
    void Depth(ShaderCompileDepth Depth) { m_Depth = Depth; }

    const ShaderBlocks & Blocks(void) { return m_Blocks; }
    uint32_t End() const { return m_End; }
    ShaderCompileDepth Depth() const { return m_Depth; }

private:
    ShaderCharacteristics(const ShaderCharacteristics&);
    ShaderCharacteristics& operator=(const ShaderCharacteristics&);

    ShaderBlocks m_Blocks;
    Labels m_Labels;
    uint32_t m_Start;
    uint32_t m_End;
    ShaderCompileDepth m_Depth;
};
