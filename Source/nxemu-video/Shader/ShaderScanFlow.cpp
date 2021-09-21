#include "ShaderScanFlow.h"
#include "ShaderBranch.h"
#include "ShaderOpCode.h"
#include "VideoNotification.h"
#include <list>
#include <stack>
#include <unordered_map>
#include <vector>
#include <algorithm>

struct ShaderScanQuery 
{
public:
    ShaderScanQuery();

    uint32_t Address;
    std::stack<uint32_t> SSYStack;
    std::stack<uint32_t> PBKStack;
};

class ShaderScanState 
{
    enum ParseResult
    {
        ParseResult_ControlCaught,
        ParseResult_BlockEnd,
        ParseResult_AbnormalFlow,
    };

public:
    struct BlockInfo
    {
        uint32_t Start;
        uint32_t End;
        bool Visited;
        ShaderBranch Branch;
    };

    typedef ShaderCharacteristics::Labels Labels;
    typedef std::list<uint32_t> InspectQueries;
    typedef std::vector<BlockInfo> BlockInfos;
    typedef std::list<ShaderScanQuery> ShaderScanQueries;

    ShaderScanState(const ShaderProgramCode & ProgramCode, uint32_t StartAddress);

    bool HasInspectQueries(void);
    bool HasQueries(void);
    bool InspectAddress(void);
    bool InspectQuery(void); 
    Labels && MoveLabels(void);
    void SortBlockInfo(void);

    size_t BlockCount(void) const { return m_BlockInfo.size(); }
    const BlockInfo & Block(size_t Index) const { return m_BlockInfo[Index]; }

private:
    ShaderScanState();
    ShaderScanState(const ShaderScanState&);
    ShaderScanState& operator=(const ShaderScanState&);

    ParseResult ParseCode(uint32_t Address, ShaderBranch & Branch, uint32_t & EndAddress); 
    BlockInfo& CreateBlockInfo(uint32_t Start, uint32_t End);

    const ShaderProgramCode & m_ProgramCode;
    uint32_t m_StartAddress;
    InspectQueries m_InspectQueries;
    BlockInfos m_BlockInfo;
    std::unordered_map<uint32_t, uint32_t> m_Registered;
    ShaderScanQueries m_Queries;
    Labels m_Labels;
    std::unordered_map<uint32_t, ShaderScanQuery> m_Stacks;
};

std::unique_ptr<ShaderCharacteristics> ShaderScanFlow(const ShaderProgramCode & ProgramCode, uint32_t StartAddress) 
{
    std::unique_ptr<ShaderCharacteristics> Characteristics(new ShaderCharacteristics);
    ShaderScanState State(ProgramCode, StartAddress);

    while (State.HasInspectQueries()) 
    {
        if (!State.InspectAddress())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            return Characteristics;
        }
    }

    bool Decompiled = true;
    while (State.HasQueries()) 
    {
        if (!State.InspectQuery())
        {
            Decompiled = false;
            break;
        }
    }
    bool UseFlowStack = !Decompiled;
    State.SortBlockInfo();
 
    Characteristics->Start(StartAddress);
    Characteristics->Depth(Decompiled ? ShaderCompileDepth_NoFlowStack : ShaderCompileDepth_FlowStack);
    for (size_t i = 0, n = State.BlockCount(); i < n; i++)
    {
        const ShaderScanState::BlockInfo & Block = State.Block(i);
        ShaderBlock & NewBlock = Characteristics->AddBlock(Block.Start, Block.End);
        NewBlock.IgnoreBranch(Block.Branch.Type() == ShaderBranchType_Single && Block.Branch.Ignore());
        if (!NewBlock.IgnoreBranch())
        {
            NewBlock.Branch(Block.Branch);
        }
        Characteristics->End(std::max(Characteristics->End(), Block.End));
    }
    if (!UseFlowStack) 
    {
        Characteristics->ReplaceLabels(State.MoveLabels());
        return Characteristics;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return nullptr;
}

ShaderScanState::ShaderScanState(const ShaderProgramCode & ProgramCode, uint32_t StartAddress) : 
    m_ProgramCode(ProgramCode),
    m_StartAddress(StartAddress)
{
    m_InspectQueries.push_back(StartAddress);
    m_Queries.emplace_back();
    ShaderScanQuery & Query = *std::prev(m_Queries.end());    
    Query.Address = StartAddress;
}

bool ShaderScanState::HasInspectQueries(void)
{
    return !m_InspectQueries.empty();
}

bool ShaderScanState::HasQueries(void)
{
    return !m_Queries.empty();
}

bool ShaderScanState::InspectAddress(void) 
{
    if (m_InspectQueries.empty()) 
    {
        return false;
    }

    enum BlockCollision 
    {
        BlockCollision_None, 
        BlockCollision_Found, 
        BlockCollision_Inside,
    };

    uint32_t Address = m_InspectQueries.front();
    m_InspectQueries.pop_front();

    BlockCollision Collision = BlockCollision_None;
    uint32_t BlockIndex = (uint32_t)-1;
    for (uint32_t i = 0, n = (uint32_t)m_BlockInfo.size(); i < n; i++) 
    {
        BlockInfo & Block = m_BlockInfo[i];
        if (Block.Start == Address) 
        {
            Collision = BlockCollision_Found;
            BlockIndex = i;
            break;
        }
        if (Block.Start <= Address && Address <= Block.End) 
        {
            Collision = BlockCollision_Inside;
            BlockIndex = i;
            break;        
        }
    }

    if (Collision == BlockCollision_Found)
    {
        return true;
    }

    if (Collision == BlockCollision_Inside) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return true;
    }

    ShaderBranch Branch;
    uint32_t EndAddress = 0;
    ParseResult Result = ParseCode(Address, Branch, EndAddress);
    if (Result == ParseResult_AbnormalFlow) 
    {
        return false;
    }

    BlockInfo & Block = CreateBlockInfo(Address, EndAddress);
    Block.Branch = Branch;
    if (Branch.Type() == ShaderBranchType_Single) 
    {
        if (Branch.ConditionPredicate() == ShaderPredicate_UnusedIndex && Branch.ConditionCode() == ShaderConditionCode_T)
        {
            return true;
        }
        m_InspectQueries.push_front(EndAddress + 1);
        return true;
    }
    return true;
}

ShaderScanState::ParseResult ShaderScanState::ParseCode(uint32_t Address, ShaderBranch & Branch, uint32_t & EndAddress)
{
    Branch.Type(ShaderBranchType_Single);
    EndAddress = (uint32_t)m_ProgramCode.size();
    uint32_t Offset = Address;

    for (;;)
    {
        if (Offset >= EndAddress)
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }
        if (m_Registered.find(Offset) != m_Registered.end())
        {
            Branch.Address(Offset);
            Branch.Ignore(true);
            break;
        }
        if (IsShaderSchedInstruction(Offset, m_StartAddress)) 
        {
            Offset += 1;
            continue;
        }
        const ShaderInstruction & Instruction = m_ProgramCode[Offset];
        ShaderOpCode Opcode(Instruction);
        if (!Opcode.Valid() || Opcode.Type() != ShaderOpCodeType_Flow)
        {
            Offset += 1;
            continue;
        }

        switch (Opcode.Id()) 
        {
        case ShaderOpCodeId_EXIT:
            Branch.ConditionPredicate((ShaderPredicate)((uint64_t)Instruction.Pred.Index + (Instruction.NegatePred != 0 ? 8ULL : 0ULL)));
            if (Branch.ConditionPredicate() == ShaderPredicate_NeverExecute)
            {
                Offset += 1;
                continue;
            }
            Branch.ConditionCode(Instruction.FlowConditionCode);
            if (Instruction.FlowConditionCode == ShaderConditionCode_F) 
            {
                Offset += 1;
                continue;
            }
            Branch.Address(ShaderBranch::ExitBranch);
            Branch.Kill(false);
            Branch.IsSync(false);
            Branch.IsBreak(false);
            Branch.Ignore(false);
            EndAddress = Offset;
            return ParseResult_ControlCaught;
        default:
            g_Notify->BreakPoint(__FILE__, __LINE__);
            break;
        }

        Offset += 1;
    }
    Branch.Kill(false);
    Branch.IsSync(false);
    Branch.IsBreak(false);
    EndAddress = Offset - 1;
    return ParseResult_BlockEnd;
}

ShaderScanState::BlockInfo & ShaderScanState::CreateBlockInfo(uint32_t Start, uint32_t End) 
{
    m_BlockInfo.emplace_back();
    BlockInfo & Block = *std::prev(m_BlockInfo.end());
    Block.Start = Start;
    Block.End = End;
    m_Registered.insert(std::pair<uint32_t, uint32_t>(Start, (uint32_t)(m_BlockInfo.size() - 1)));
    return Block;
}

bool ShaderScanState::InspectQuery(void)
{
    if (m_Queries.empty())
    {
        return false;
    }
    ShaderScanQuery Query = m_Queries.front();
    m_Queries.pop_front();

    uint32_t BlockIndex = m_Registered[Query.Address];
    BlockInfo & Block = m_BlockInfo[BlockIndex];
    if (Block.Visited) 
    {
        ShaderScanQuery & Stack = m_Stacks[Query.Address];
        return (Stack.SSYStack.empty() || Query.SSYStack == Stack.SSYStack) &&
               (Stack.PBKStack.empty() || Query.PBKStack == Stack.PBKStack);
    }
    Block.Visited = true;
    m_Stacks.insert_or_assign(Query.Address, Query);

    ShaderBranch & Branch = Block.Branch;
    if (Branch.Type() == ShaderBranchType_Single) 
    {  
        if (Branch.ConditionPredicate() != ShaderPredicate_UnusedIndex || Branch.ConditionCode() != ShaderConditionCode_T)
        {
            Query.Address = Block.End + 1;
            m_Queries.push_back(Query);
        }

        m_Queries.emplace_back();
        ShaderScanQuery & ConditionalQuery = *std::prev(m_Queries.end());
        if (Branch.IsSync()) 
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        if (Branch.IsBreak())
        {
            g_Notify->BreakPoint(__FILE__, __LINE__);
        }
        ConditionalQuery.Address = Branch.Address();
        return true;
    }
    g_Notify->BreakPoint(__FILE__, __LINE__);
    return true;
}

ShaderScanState::Labels && ShaderScanState::MoveLabels(void) 
{
    return std::move(m_Labels);
}

bool CompareBlockInfo(const ShaderScanState::BlockInfo & a, const ShaderScanState::BlockInfo & b)
{
    return a.Start < b.Start;
}

void ShaderScanState::SortBlockInfo(void)
{
    std::sort(m_BlockInfo.begin(), m_BlockInfo.end(), CompareBlockInfo);
}

ShaderScanQuery::ShaderScanQuery() :
    Address(0)
{
}
