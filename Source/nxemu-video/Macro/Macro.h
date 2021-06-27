#pragma once

enum MacroOperation : unsigned
{
    MacroOperation_ALU = 0,
    MacroOperation_AddImmediate = 1,
    MacroOperation_ExtractInsert = 2,
    MacroOperation_ExtractShiftLeftImmediate = 3,
    MacroOperation_ExtractShiftLeftRegister = 4,
    MacroOperation_Read = 5,
    MacroOperation_Unknown = 6,
    MacroOperation_Branch = 7,
};

enum MacroResultOperation : unsigned
{
    MacroResultOperation_IgnoreAndFetch = 0,
    MacroResultOperation_Move = 1,
    MacroResultOperation_MoveAndSetMethod = 2,
    MacroResultOperation_FetchAndSend = 3,
    MacroResultOperation_MoveAndSend = 4,
    MacroResultOperation_FetchAndSetMethod = 5,
    MacroResultOperation_MoveAndSetMethodFetchAndSend = 6,
    MacroResultOperation_MoveAndSetMethodSend = 7
};

enum MacroALUOperation : unsigned
{
    MacroALUOperation_Add = 0,
    MacroALUOperation_AddWithCarry = 1,
    MacroALUOperation_Subtract = 2,
    MacroALUOperation_SubtractWithBorrow = 3,
    MacroALUOperation_Xor = 8,
    MacroALUOperation_Or = 9,
    MacroALUOperation_And = 10,
    MacroALUOperation_AndNot = 11,
    MacroALUOperation_Nand = 12
};

#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union

union MacroOpcode 
{
    uint32_t Value;
    struct 
    {
        MacroOperation Operation : 3;
        unsigned : 1;
        MacroResultOperation ResultOperation : 3;
        unsigned IsExit : 1;
        unsigned Dst : 3;
        unsigned SrcA : 3;
        unsigned SrcB : 3;
    };

    struct 
    {
        unsigned : 4;
        unsigned BranchCondition : 1;
        unsigned BranchAnnul : 1;
        unsigned : 11;
        MacroALUOperation AluOperation : 5;
    };

    struct 
    {
        unsigned : 14;
        signed Immediate : 18;
    };

    struct 
    {
        unsigned : 17;
        unsigned BfSrcBit : 5;
        unsigned BfSize : 5;
        unsigned BfDstBit : 5;
    };

    uint32_t GetBitfieldMask() const 
    {
        return (1 << BfSize) - 1;
    }
};

union MacroMethodAddress 
{
    uint32_t Value;
    struct 
    {
        unsigned Address : 12;
        unsigned Increment : 6;
    };
};

#pragma warning(pop)
