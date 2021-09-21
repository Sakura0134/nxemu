#include "ShaderOpCode.h"
#include "VideoNotification.h"

std::vector<ShaderOpCode::Decoder> ShaderOpCode::m_DecodeTable;

ShaderOpCode::ShaderOpCode(const ShaderInstruction & Instruction) :
    m_Decoder(nullptr)
{
    for (size_t i = 0, n = m_DecodeTable.size(); i < n; i++) 
    {
        if ((m_DecodeTable[i].Mask & Instruction.Opcode) != m_DecodeTable[i].Expected) 
        {
            continue;
        }
        m_Decoder = &m_DecodeTable[i];
        break;
    }
    if (m_Decoder == nullptr) 
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
    }
}

const char * ShaderOpCode::Name(void) const
{
    if (m_Decoder == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return "";
    }
    return m_Decoder->Name;
}

ShaderOpCodeType ShaderOpCode::Type(void) const
{
    if (m_Decoder == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return ShaderOpCodeType_Unknown;
    }
    return m_Decoder->Type;
}

ShaderOpCodeId ShaderOpCode::Id(void) const
{
    if (m_Decoder == nullptr)
    {
        g_Notify->BreakPoint(__FILE__, __LINE__);
        return ShaderOpCodeId_NOP;
    }
    return m_Decoder->OpCodeId;
}

bool ShaderOpCode::Valid(void) const
{
    return m_Decoder != nullptr;
}

void ShaderOpCode::CreateDecodeTable(void)
{
#define INST(BitString, Name, OpcodeID, Type) m_DecodeTable.emplace_back(CreateDecoder(BitString, Name, OpcodeID, Type))
    INST("111000110011----", "KIL", ShaderOpCodeId_KIL, ShaderOpCodeType_Flow);
    INST("111000101001----", "SSY", ShaderOpCodeId_SSY, ShaderOpCodeType_Flow);
    INST("111000101010----", "PBK", ShaderOpCodeId_PBK, ShaderOpCodeType_Flow);
    INST("111000100100----", "BRA", ShaderOpCodeId_BRA, ShaderOpCodeType_Flow);
    INST("111000100101----", "BRX", ShaderOpCodeId_BRX, ShaderOpCodeType_Flow);
    INST("1111000011111---", "SYNC", ShaderOpCodeId_SYNC, ShaderOpCodeType_Flow);
    INST("111000110100----", "BRK", ShaderOpCodeId_BRK, ShaderOpCodeType_Flow);
    INST("111000110000----", "EXIT", ShaderOpCodeId_EXIT, ShaderOpCodeType_Flow);
    INST("1111000011110---", "DEPBAR", ShaderOpCodeId_DEPBAR, ShaderOpCodeType_Synch);
    INST("0101000011011---", "VOTE", ShaderOpCodeId_VOTE, ShaderOpCodeType_Warp);
    INST("0101000011100---", "VOTE_VTG", ShaderOpCodeId_VOTE_VTG, ShaderOpCodeType_Warp);
    INST("1110111100010---", "SHFL", ShaderOpCodeId_SHFL, ShaderOpCodeType_Warp);
    INST("0101000011111---", "FSWZADD", ShaderOpCodeId_FSWZADD, ShaderOpCodeType_Warp);
    INST("1110111111011---", "LD_A", ShaderOpCodeId_LD_A, ShaderOpCodeType_Memory);
    INST("1110111101001---", "LD_S", ShaderOpCodeId_LD_S, ShaderOpCodeType_Memory);
    INST("1110111101000---", "LD_L", ShaderOpCodeId_LD_L, ShaderOpCodeType_Memory);
    INST("1110111110010---", "LD_C", ShaderOpCodeId_LD_C, ShaderOpCodeType_Memory);
    INST("100-------------", "LD", ShaderOpCodeId_LD, ShaderOpCodeType_Memory);
    INST("1110111011010---", "LDG", ShaderOpCodeId_LDG, ShaderOpCodeType_Memory);
    INST("1110111111110---", "ST_A", ShaderOpCodeId_ST_A, ShaderOpCodeType_Memory);
    INST("1110111101011---", "ST_S", ShaderOpCodeId_ST_S, ShaderOpCodeType_Memory);
    INST("1110111101010---", "ST_L", ShaderOpCodeId_ST_L, ShaderOpCodeType_Memory);
    INST("101-------------", "ST", ShaderOpCodeId_ST, ShaderOpCodeType_Memory);
    INST("1110111011011---", "STG", ShaderOpCodeId_STG, ShaderOpCodeType_Memory);
    INST("1110101111111---", "RED", ShaderOpCodeId_RED, ShaderOpCodeType_Memory);
    INST("11101101--------", "ATOM", ShaderOpCodeId_ATOM, ShaderOpCodeType_Memory);
    INST("11101100--------", "ATOMS", ShaderOpCodeId_ATOMS, ShaderOpCodeType_Memory);
    INST("1110111110100---", "AL2P", ShaderOpCodeId_AL2P, ShaderOpCodeType_Memory);
    INST("110000----111---", "TEX", ShaderOpCodeId_TEX, ShaderOpCodeType_Texture);
    INST("1101111010111---", "TEX_B", ShaderOpCodeId_TEX_B, ShaderOpCodeType_Texture);
    INST("1101111101001---", "TXQ", ShaderOpCodeId_TXQ, ShaderOpCodeType_Texture);
    INST("1101111101010---", "TXQ_B", ShaderOpCodeId_TXQ_B, ShaderOpCodeType_Texture);
    INST("1101-00---------", "TEXS", ShaderOpCodeId_TEXS, ShaderOpCodeType_Texture);
    INST("11011100--11----", "TLD", ShaderOpCodeId_TLD, ShaderOpCodeType_Texture);
    INST("1101-01---------", "TLDS", ShaderOpCodeId_TLDS, ShaderOpCodeType_Texture);
    INST("110010----111---", "TLD4", ShaderOpCodeId_TLD4, ShaderOpCodeType_Texture);
    INST("1101111011111---", "TLD4_B", ShaderOpCodeId_TLD4_B, ShaderOpCodeType_Texture);
    INST("11011111-0------", "TLD4S", ShaderOpCodeId_TLD4S, ShaderOpCodeType_Texture);
    INST("110111110110----", "TMML_B", ShaderOpCodeId_TMML_B, ShaderOpCodeType_Texture);
    INST("1101111101011---", "TMML", ShaderOpCodeId_TMML, ShaderOpCodeType_Texture);
    INST("11011110011110--", "TXD_B", ShaderOpCodeId_TXD_B, ShaderOpCodeType_Texture);
    INST("11011110001110--", "TXD", ShaderOpCodeId_TXD, ShaderOpCodeType_Texture);
    INST("11101011001-----", "SUST", ShaderOpCodeId_SUST, ShaderOpCodeType_Image);
    INST("11101011000-----", "SULD", ShaderOpCodeId_SULD, ShaderOpCodeType_Image);
    INST("1110101000------", "SUATOM_D", ShaderOpCodeId_SUATOM, ShaderOpCodeType_Image);
    INST("0101000010110---", "NOP", ShaderOpCodeId_NOP, ShaderOpCodeType_Trivial);
    INST("11100000--------", "IPA", ShaderOpCodeId_IPA, ShaderOpCodeType_Trivial);
    INST("1111101111100---", "OUT_R", ShaderOpCodeId_OUT_R, ShaderOpCodeType_Trivial);
    INST("1110111111010---", "ISBERD", ShaderOpCodeId_ISBERD, ShaderOpCodeType_Trivial);
    INST("1111000010101---", "BAR", ShaderOpCodeId_BAR, ShaderOpCodeType_Trivial);
    INST("1110111110011---", "MEMBAR", ShaderOpCodeId_MEMBAR, ShaderOpCodeType_Trivial);
    INST("01011111--------", "VMAD", ShaderOpCodeId_VMAD, ShaderOpCodeType_Video);
    INST("0101000011110---", "VSETP", ShaderOpCodeId_VSETP, ShaderOpCodeType_Video);
    INST("0011101---------", "VMNMX", ShaderOpCodeId_VMNMX, ShaderOpCodeType_Video);
    INST("0011001-1-------", "FFMA_IMM", ShaderOpCodeId_FFMA_IMM, ShaderOpCodeType_Ffma);
    INST("010010011-------", "FFMA_CR", ShaderOpCodeId_FFMA_CR, ShaderOpCodeType_Ffma);
    INST("010100011-------", "FFMA_RC", ShaderOpCodeId_FFMA_RC, ShaderOpCodeType_Ffma);
    INST("010110011-------", "FFMA_RR", ShaderOpCodeId_FFMA_RR, ShaderOpCodeType_Ffma);
    INST("0100110001011---", "FADD_C", ShaderOpCodeId_FADD_C, ShaderOpCodeType_Arithmetic);
    INST("0101110001011---", "FADD_R", ShaderOpCodeId_FADD_R, ShaderOpCodeType_Arithmetic);
    INST("0011100-01011---", "FADD_IMM", ShaderOpCodeId_FADD_IMM, ShaderOpCodeType_Arithmetic);
    INST("000010----------", "FADD32I", ShaderOpCodeId_FADD32I, ShaderOpCodeType_ArithmeticImmediate);
    INST("0100110001101---", "FMUL_C", ShaderOpCodeId_FMUL_C, ShaderOpCodeType_Arithmetic);
    INST("0101110001101---", "FMUL_R", ShaderOpCodeId_FMUL_R, ShaderOpCodeType_Arithmetic);
    INST("0011100-01101---", "FMUL_IMM", ShaderOpCodeId_FMUL_IMM, ShaderOpCodeType_Arithmetic);
    INST("00011110--------", "FMUL32_IMM", ShaderOpCodeId_FMUL32_IMM, ShaderOpCodeType_ArithmeticImmediate);
    INST("0100110000010---", "IADD_C", ShaderOpCodeId_IADD_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110000010---", "IADD_R", ShaderOpCodeId_IADD_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-00010---", "IADD_IMM", ShaderOpCodeId_IADD_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("010011001100----", "IADD3_C", ShaderOpCodeId_IADD3_C, ShaderOpCodeType_ArithmeticInteger);
    INST("010111001100----", "IADD3_R", ShaderOpCodeId_IADD3_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-1100----", "IADD3_IMM", ShaderOpCodeId_IADD3_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0001110---------", "IADD32I", ShaderOpCodeId_IADD32I, ShaderOpCodeType_ArithmeticIntegerImmediate);
    INST("0100110000011---", "ISCADD_C", ShaderOpCodeId_ISCADD_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110000011---", "ISCADD_R", ShaderOpCodeId_ISCADD_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-00011---", "ISCADD_IMM", ShaderOpCodeId_ISCADD_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0100110000001---", "POPC_C", ShaderOpCodeId_POPC_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110000001---", "POPC_R", ShaderOpCodeId_POPC_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-00001---", "POPC_IMM", ShaderOpCodeId_POPC_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0100110010100---", "SEL_C", ShaderOpCodeId_SEL_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110010100---", "SEL_R", ShaderOpCodeId_SEL_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-10100---", "SEL_IMM", ShaderOpCodeId_SEL_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("010100110100----", "ICMP_RC", ShaderOpCodeId_ICMP_RC, ShaderOpCodeType_ArithmeticInteger);
    INST("010110110100----", "ICMP_R", ShaderOpCodeId_ICMP_R, ShaderOpCodeType_ArithmeticInteger);
    INST("010010110100----", "ICMP_CR", ShaderOpCodeId_ICMP_CR, ShaderOpCodeType_ArithmeticInteger);
    INST("0011011-0100----", "ICMP_IMM", ShaderOpCodeId_ICMP_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110000110---", "FLO_R", ShaderOpCodeId_FLO_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0100110000110---", "FLO_C", ShaderOpCodeId_FLO_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-00110---", "FLO_IMM", ShaderOpCodeId_FLO_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0101101111011---", "LEA_R2", ShaderOpCodeId_LEA_R2, ShaderOpCodeType_ArithmeticInteger);
    INST("0101101111010---", "LEA_R1", ShaderOpCodeId_LEA_R1, ShaderOpCodeType_ArithmeticInteger);
    INST("001101101101----", "LEA_IMM", ShaderOpCodeId_LEA_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("010010111101----", "LEA_RZ", ShaderOpCodeId_LEA_RZ, ShaderOpCodeType_ArithmeticInteger);
    INST("00011000--------", "LEA_HI", ShaderOpCodeId_LEA_HI, ShaderOpCodeType_ArithmeticInteger);
    INST("0111101-1-------", "HADD2_C", ShaderOpCodeId_HADD2_C, ShaderOpCodeType_ArithmeticHalf);
    INST("0101110100010---", "HADD2_R", ShaderOpCodeId_HADD2_R, ShaderOpCodeType_ArithmeticHalf);
    INST("0111101-0-------", "HADD2_IMM", ShaderOpCodeId_HADD2_IMM, ShaderOpCodeType_ArithmeticHalfImmediate);
    INST("0111100-1-------", "HMUL2_C", ShaderOpCodeId_HMUL2_C, ShaderOpCodeType_ArithmeticHalf);
    INST("0101110100001---", "HMUL2_R", ShaderOpCodeId_HMUL2_R, ShaderOpCodeType_ArithmeticHalf);
    INST("0111100-0-------", "HMUL2_IMM", ShaderOpCodeId_HMUL2_IMM, ShaderOpCodeType_ArithmeticHalfImmediate);
    INST("01110---1-------", "HFMA2_CR", ShaderOpCodeId_HFMA2_CR, ShaderOpCodeType_Hfma2);
    INST("01100---1-------", "HFMA2_RC", ShaderOpCodeId_HFMA2_RC, ShaderOpCodeType_Hfma2);
    INST("0101110100000---", "HFMA2_RR", ShaderOpCodeId_HFMA2_RR, ShaderOpCodeType_Hfma2);
    INST("01110---0-------", "HFMA2_R_IMM", ShaderOpCodeId_HFMA2_IMM_R, ShaderOpCodeType_Hfma2);
    INST("0111111-1-------", "HSETP2_C", ShaderOpCodeId_HSETP2_C, ShaderOpCodeType_HalfSetPredicate);
    INST("0101110100100---", "HSETP2_R", ShaderOpCodeId_HSETP2_R, ShaderOpCodeType_HalfSetPredicate);
    INST("0111111-0-------", "HSETP2_IMM", ShaderOpCodeId_HSETP2_IMM, ShaderOpCodeType_HalfSetPredicate);
    INST("0111110-1-------", "HSET2_C", ShaderOpCodeId_HSET2_C, ShaderOpCodeType_HalfSet);
    INST("0101110100011---", "HSET2_R", ShaderOpCodeId_HSET2_R, ShaderOpCodeType_HalfSet);
    INST("0111110-0-------", "HSET2_IMM", ShaderOpCodeId_HSET2_IMM, ShaderOpCodeType_HalfSet);
    INST("010110111010----", "FCMP_RR", ShaderOpCodeId_FCMP_RR, ShaderOpCodeType_Arithmetic);
    INST("010010111010----", "FCMP_RC", ShaderOpCodeId_FCMP_RC, ShaderOpCodeType_Arithmetic);
    INST("0011011-1010----", "FCMP_IMMR", ShaderOpCodeId_FCMP_IMMR, ShaderOpCodeType_Arithmetic);
    INST("0101000010000---", "MUFU", ShaderOpCodeId_MUFU, ShaderOpCodeType_Arithmetic);
    INST("0100110010010---", "RRO_C", ShaderOpCodeId_RRO_C, ShaderOpCodeType_Arithmetic);
    INST("0101110010010---", "RRO_R", ShaderOpCodeId_RRO_R, ShaderOpCodeType_Arithmetic);
    INST("0011100-10010---", "RRO_IMM", ShaderOpCodeId_RRO_IMM, ShaderOpCodeType_Arithmetic);
    INST("0100110010101---", "F2F_C", ShaderOpCodeId_F2F_C, ShaderOpCodeType_Conversion);
    INST("0101110010101---", "F2F_R", ShaderOpCodeId_F2F_R, ShaderOpCodeType_Conversion);
    INST("0011100-10101---", "F2F_IMM", ShaderOpCodeId_F2F_IMM, ShaderOpCodeType_Conversion);
    INST("0100110010110---", "F2I_C", ShaderOpCodeId_F2I_C, ShaderOpCodeType_Conversion);
    INST("0101110010110---", "F2I_R", ShaderOpCodeId_F2I_R, ShaderOpCodeType_Conversion);
    INST("0011100-10110---", "F2I_IMM", ShaderOpCodeId_F2I_IMM, ShaderOpCodeType_Conversion);
    INST("0100110010011---", "MOV_C", ShaderOpCodeId_MOV_C, ShaderOpCodeType_Arithmetic);
    INST("0101110010011---", "MOV_R", ShaderOpCodeId_MOV_R, ShaderOpCodeType_Arithmetic);
    INST("0011100-10011---", "MOV_IMM", ShaderOpCodeId_MOV_IMM, ShaderOpCodeType_Arithmetic);
    INST("1111000011001---", "S2R", ShaderOpCodeId_S2R, ShaderOpCodeType_Trivial);
    INST("000000010000----", "MOV32_IMM", ShaderOpCodeId_MOV32_IMM, ShaderOpCodeType_ArithmeticImmediate);
    INST("0100110001100---", "FMNMX_C", ShaderOpCodeId_FMNMX_C, ShaderOpCodeType_Arithmetic);
    INST("0101110001100---", "FMNMX_R", ShaderOpCodeId_FMNMX_R, ShaderOpCodeType_Arithmetic);
    INST("0011100-01100---", "FMNMX_IMM", ShaderOpCodeId_FMNMX_IMM, ShaderOpCodeType_Arithmetic);
    INST("0100110000100---", "IMNMX_C", ShaderOpCodeId_IMNMX_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110000100---", "IMNMX_R", ShaderOpCodeId_IMNMX_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-00100---", "IMNMX_IMM", ShaderOpCodeId_IMNMX_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0100110000000---", "BFE_C", ShaderOpCodeId_BFE_C, ShaderOpCodeType_Bfe);
    INST("0101110000000---", "BFE_R", ShaderOpCodeId_BFE_R, ShaderOpCodeType_Bfe);
    INST("0011100-00000---", "BFE_IMM", ShaderOpCodeId_BFE_IMM, ShaderOpCodeType_Bfe);
    INST("0101001111110---", "BFI_RC", ShaderOpCodeId_BFI_RC, ShaderOpCodeType_Bfi);
    INST("0011011-11110---", "BFI_IMM_R", ShaderOpCodeId_BFI_IMM_R, ShaderOpCodeType_Bfi);
    INST("0100110001000---", "LOP_C", ShaderOpCodeId_LOP_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101110001000---", "LOP_R", ShaderOpCodeId_LOP_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011100-01000---", "LOP_IMM", ShaderOpCodeId_LOP_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("000001----------", "LOP32I", ShaderOpCodeId_LOP32I, ShaderOpCodeType_ArithmeticIntegerImmediate);
    INST("0000001---------", "LOP3_C", ShaderOpCodeId_LOP3_C, ShaderOpCodeType_ArithmeticInteger);
    INST("0101101111100---", "LOP3_R", ShaderOpCodeId_LOP3_R, ShaderOpCodeType_ArithmeticInteger);
    INST("0011110---------", "LOP3_IMM", ShaderOpCodeId_LOP3_IMM, ShaderOpCodeType_ArithmeticInteger);
    INST("0100110001001---", "SHL_C", ShaderOpCodeId_SHL_C, ShaderOpCodeType_Shift);
    INST("0101110001001---", "SHL_R", ShaderOpCodeId_SHL_R, ShaderOpCodeType_Shift);
    INST("0011100-01001---", "SHL_IMM", ShaderOpCodeId_SHL_IMM, ShaderOpCodeType_Shift);
    INST("0100110000101---", "SHR_C", ShaderOpCodeId_SHR_C, ShaderOpCodeType_Shift);
    INST("0101110000101---", "SHR_R", ShaderOpCodeId_SHR_R, ShaderOpCodeType_Shift);
    INST("0011100-00101---", "SHR_IMM", ShaderOpCodeId_SHR_IMM, ShaderOpCodeType_Shift);
    INST("0101110011111---", "SHF_RIGHT_R", ShaderOpCodeId_SHF_RIGHT_R, ShaderOpCodeType_Shift);
    INST("0011100-11111---", "SHF_RIGHT_IMM", ShaderOpCodeId_SHF_RIGHT_IMM, ShaderOpCodeType_Shift);
    INST("0101101111111---", "SHF_LEFT_R", ShaderOpCodeId_SHF_LEFT_R, ShaderOpCodeType_Shift);
    INST("0011011-11111---", "SHF_LEFT_IMM", ShaderOpCodeId_SHF_LEFT_IMM, ShaderOpCodeType_Shift);
    INST("0100110011100---", "I2I_C", ShaderOpCodeId_I2I_C, ShaderOpCodeType_Conversion);
    INST("0101110011100---", "I2I_R", ShaderOpCodeId_I2I_R, ShaderOpCodeType_Conversion);
    INST("0011100-11100---", "I2I_IMM", ShaderOpCodeId_I2I_IMM, ShaderOpCodeType_Conversion);
    INST("0100110010111---", "I2F_C", ShaderOpCodeId_I2F_C, ShaderOpCodeType_Conversion);
    INST("0101110010111---", "I2F_R", ShaderOpCodeId_I2F_R, ShaderOpCodeType_Conversion);
    INST("0011100-10111---", "I2F_IMM", ShaderOpCodeId_I2F_IMM, ShaderOpCodeType_Conversion);
    INST("01011000--------", "FSET_R", ShaderOpCodeId_FSET_R, ShaderOpCodeType_FloatSet);
    INST("0100100---------", "FSET_C", ShaderOpCodeId_FSET_C, ShaderOpCodeType_FloatSet);
    INST("0011000---------", "FSET_IMM", ShaderOpCodeId_FSET_IMM, ShaderOpCodeType_FloatSet);
    INST("010010111011----", "FSETP_C", ShaderOpCodeId_FSETP_C, ShaderOpCodeType_FloatSetPredicate);
    INST("010110111011----", "FSETP_R", ShaderOpCodeId_FSETP_R, ShaderOpCodeType_FloatSetPredicate);
    INST("0011011-1011----", "FSETP_IMM", ShaderOpCodeId_FSETP_IMM, ShaderOpCodeType_FloatSetPredicate);
    INST("010010110110----", "ISETP_C", ShaderOpCodeId_ISETP_C, ShaderOpCodeType_IntegerSetPredicate);
    INST("010110110110----", "ISETP_R", ShaderOpCodeId_ISETP_R, ShaderOpCodeType_IntegerSetPredicate);
    INST("0011011-0110----", "ISETP_IMM", ShaderOpCodeId_ISETP_IMM, ShaderOpCodeType_IntegerSetPredicate);
    INST("010110110101----", "ISET_R", ShaderOpCodeId_ISET_R, ShaderOpCodeType_IntegerSet);
    INST("010010110101----", "ISET_C", ShaderOpCodeId_ISET_C, ShaderOpCodeType_IntegerSet);
    INST("0011011-0101----", "ISET_IMM", ShaderOpCodeId_ISET_IMM, ShaderOpCodeType_IntegerSet);
    INST("0101000010001---", "PSET", ShaderOpCodeId_PSET, ShaderOpCodeType_PredicateSetRegister);
    INST("0101000010010---", "PSETP", ShaderOpCodeId_PSETP, ShaderOpCodeType_PredicateSetPredicate);
    INST("010100001010----", "CSETP", ShaderOpCodeId_CSETP, ShaderOpCodeType_PredicateSetPredicate);
    INST("0011100-11110---", "R2P_IMM", ShaderOpCodeId_R2P_IMM, ShaderOpCodeType_RegisterSetPredicate);
    INST("0011100-11101---", "P2R_IMM", ShaderOpCodeId_P2R_IMM, ShaderOpCodeType_RegisterSetPredicate);
    INST("0011011-00------", "XMAD_IMM", ShaderOpCodeId_XMAD_IMM, ShaderOpCodeType_Xmad);
    INST("0100111---------", "XMAD_CR", ShaderOpCodeId_XMAD_CR, ShaderOpCodeType_Xmad);
    INST("010100010-------", "XMAD_RC", ShaderOpCodeId_XMAD_RC, ShaderOpCodeType_Xmad);
    INST("0101101100------", "XMAD_RR", ShaderOpCodeId_XMAD_RR, ShaderOpCodeType_Xmad);
}

ShaderOpCode::Decoder ShaderOpCode::CreateDecoder(const char * BitString, const char * Name, ShaderOpCodeId OpCodeId, ShaderOpCodeType Type) 
{
    enum 
    {
        OpcodeBitSize = 16,
    };

    Decoder Item;
    Item.Name = Name;
    Item.OpCodeId = OpCodeId;
    Item.Type = Type;
    Item.Mask = 0;
    Item.Expected = 0;
    for (uint32_t i = 0; i < OpcodeBitSize; i++) 
    {
        uint32_t BitPosition = OpcodeBitSize - i - 1;
        if (BitString[i] == '\0') 
        {
            break;
        } 
        else if (BitString[i] == '0') 
        {
            Item.Mask |= (uint16_t)(1U << BitPosition);        
        }
        else if (BitString[i] == '1') 
        {
            Item.Expected |= (uint16_t)(1U << BitPosition);
            Item.Mask |= (uint16_t)(1U << BitPosition);
        }
    }

    return Item;
}
