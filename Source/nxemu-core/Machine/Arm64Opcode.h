#pragma once
#include <Common\CriticalSection.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>

class Arm64OpcodeCache;

class Arm64Opcode
{
    friend Arm64OpcodeCache;

public:
    typedef enum instruct_t
    {
        ARM64_INS_INVALID = 0,

        ARM64_INS_ABS,
        ARM64_INS_ADC,
        ARM64_INS_ADDHN,
        ARM64_INS_ADDHN2,
        ARM64_INS_ADDP,
        ARM64_INS_ADD,
        ARM64_INS_ADDV,
        ARM64_INS_ADR,
        ARM64_INS_ADRP,
        ARM64_INS_AESD,
        ARM64_INS_AESE,
        ARM64_INS_AESIMC,
        ARM64_INS_AESMC,
        ARM64_INS_AND,
        ARM64_INS_ASR,
        ARM64_INS_B,
        ARM64_INS_BFM,
        ARM64_INS_BIC,
        ARM64_INS_BIF,
        ARM64_INS_BIT,
        ARM64_INS_BL,
        ARM64_INS_BLR,
        ARM64_INS_BR,
        ARM64_INS_BRK,
        ARM64_INS_BSL,
        ARM64_INS_CBNZ,
        ARM64_INS_CBZ,
        ARM64_INS_CCMN,
        ARM64_INS_CCMP,
        ARM64_INS_CLREX,
        ARM64_INS_CLS,
        ARM64_INS_CLZ,
        ARM64_INS_CMEQ,
        ARM64_INS_CMGE,
        ARM64_INS_CMGT,
        ARM64_INS_CMHI,
        ARM64_INS_CMHS,
        ARM64_INS_CMLE,
        ARM64_INS_CMLT,
        ARM64_INS_CMTST,
        ARM64_INS_CNT,
        ARM64_INS_MOV,
        ARM64_INS_CRC32B,
        ARM64_INS_CRC32CB,
        ARM64_INS_CRC32CH,
        ARM64_INS_CRC32CW,
        ARM64_INS_CRC32CX,
        ARM64_INS_CRC32H,
        ARM64_INS_CRC32W,
        ARM64_INS_CRC32X,
        ARM64_INS_CSEL,
        ARM64_INS_CSINC,
        ARM64_INS_CSINV,
        ARM64_INS_CSNEG,
        ARM64_INS_DCPS1,
        ARM64_INS_DCPS2,
        ARM64_INS_DCPS3,
        ARM64_INS_DMB,
        ARM64_INS_DRPS,
        ARM64_INS_DSB,
        ARM64_INS_DUP,
        ARM64_INS_EON,
        ARM64_INS_EOR,
        ARM64_INS_ERET,
        ARM64_INS_EXTR,
        ARM64_INS_EXT,
        ARM64_INS_FABD,
        ARM64_INS_FABS,
        ARM64_INS_FACGE,
        ARM64_INS_FACGT,
        ARM64_INS_FADD,
        ARM64_INS_FADDP,
        ARM64_INS_FCCMP,
        ARM64_INS_FCCMPE,
        ARM64_INS_FCMEQ,
        ARM64_INS_FCMGE,
        ARM64_INS_FCMGT,
        ARM64_INS_FCMLE,
        ARM64_INS_FCMLT,
        ARM64_INS_FCMP,
        ARM64_INS_FCMPE,
        ARM64_INS_FCSEL,
        ARM64_INS_FCVTAS,
        ARM64_INS_FCVTAU,
        ARM64_INS_FCVT,
        ARM64_INS_FCVTL,
        ARM64_INS_FCVTL2,
        ARM64_INS_FCVTMS,
        ARM64_INS_FCVTMU,
        ARM64_INS_FCVTNS,
        ARM64_INS_FCVTNU,
        ARM64_INS_FCVTN,
        ARM64_INS_FCVTN2,
        ARM64_INS_FCVTPS,
        ARM64_INS_FCVTPU,
        ARM64_INS_FCVTXN,
        ARM64_INS_FCVTXN2,
        ARM64_INS_FCVTZS,
        ARM64_INS_FCVTZU,
        ARM64_INS_FDIV,
        ARM64_INS_FMADD,
        ARM64_INS_FMAX,
        ARM64_INS_FMAXNM,
        ARM64_INS_FMAXNMP,
        ARM64_INS_FMAXNMV,
        ARM64_INS_FMAXP,
        ARM64_INS_FMAXV,
        ARM64_INS_FMIN,
        ARM64_INS_FMINNM,
        ARM64_INS_FMINNMP,
        ARM64_INS_FMINNMV,
        ARM64_INS_FMINP,
        ARM64_INS_FMINV,
        ARM64_INS_FMLA,
        ARM64_INS_FMLS,
        ARM64_INS_FMOV,
        ARM64_INS_FMSUB,
        ARM64_INS_FMUL,
        ARM64_INS_FMULX,
        ARM64_INS_FNEG,
        ARM64_INS_FNMADD,
        ARM64_INS_FNMSUB,
        ARM64_INS_FNMUL,
        ARM64_INS_FRECPE,
        ARM64_INS_FRECPS,
        ARM64_INS_FRECPX,
        ARM64_INS_FRINTA,
        ARM64_INS_FRINTI,
        ARM64_INS_FRINTM,
        ARM64_INS_FRINTN,
        ARM64_INS_FRINTP,
        ARM64_INS_FRINTX,
        ARM64_INS_FRINTZ,
        ARM64_INS_FRSQRTE,
        ARM64_INS_FRSQRTS,
        ARM64_INS_FSQRT,
        ARM64_INS_FSUB,
        ARM64_INS_HINT,
        ARM64_INS_HLT,
        ARM64_INS_HVC,
        ARM64_INS_INS,

        ARM64_INS_ISB,
        ARM64_INS_LD1,
        ARM64_INS_LD1R,
        ARM64_INS_LD2R,
        ARM64_INS_LD2,
        ARM64_INS_LD3R,
        ARM64_INS_LD3,
        ARM64_INS_LD4,
        ARM64_INS_LD4R,

        ARM64_INS_LDARB,
        ARM64_INS_LDARH,
        ARM64_INS_LDAR,
        ARM64_INS_LDAXP,
        ARM64_INS_LDAXRB,
        ARM64_INS_LDAXRH,
        ARM64_INS_LDAXR,
        ARM64_INS_LDNP,
        ARM64_INS_LDP,
        ARM64_INS_LDPSW,
        ARM64_INS_LDRB,
        ARM64_INS_LDR,
        ARM64_INS_LDRH,
        ARM64_INS_LDRSB,
        ARM64_INS_LDRSH,
        ARM64_INS_LDRSW,
        ARM64_INS_LDTRB,
        ARM64_INS_LDTRH,
        ARM64_INS_LDTRSB,

        ARM64_INS_LDTRSH,
        ARM64_INS_LDTRSW,
        ARM64_INS_LDTR,
        ARM64_INS_LDURB,
        ARM64_INS_LDUR,
        ARM64_INS_LDURH,
        ARM64_INS_LDURSB,
        ARM64_INS_LDURSH,
        ARM64_INS_LDURSW,
        ARM64_INS_LDXP,
        ARM64_INS_LDXRB,
        ARM64_INS_LDXRH,
        ARM64_INS_LDXR,
        ARM64_INS_LSL,
        ARM64_INS_LSR,
        ARM64_INS_MADD,
        ARM64_INS_MLA,
        ARM64_INS_MLS,
        ARM64_INS_MOVI,
        ARM64_INS_MOVK,
        ARM64_INS_MOVN,
        ARM64_INS_MOVZ,
        ARM64_INS_MRS,
        ARM64_INS_MSR,
        ARM64_INS_MSUB,
        ARM64_INS_MUL,
        ARM64_INS_MVNI,
        ARM64_INS_NEG,
        ARM64_INS_NOT,
        ARM64_INS_ORN,
        ARM64_INS_ORR,
        ARM64_INS_PMULL2,
        ARM64_INS_PMULL,
        ARM64_INS_PMUL,
        ARM64_INS_PRFM,
        ARM64_INS_PRFUM,
        ARM64_INS_RADDHN,
        ARM64_INS_RADDHN2,
        ARM64_INS_RBIT,
        ARM64_INS_RET,
        ARM64_INS_REV16,
        ARM64_INS_REV32,
        ARM64_INS_REV64,
        ARM64_INS_REV,
        ARM64_INS_ROR,
        ARM64_INS_RSHRN2,
        ARM64_INS_RSHRN,
        ARM64_INS_RSUBHN,
        ARM64_INS_RSUBHN2,
        ARM64_INS_SABAL2,
        ARM64_INS_SABAL,

        ARM64_INS_SABA,
        ARM64_INS_SABDL2,
        ARM64_INS_SABDL,
        ARM64_INS_SABD,
        ARM64_INS_SADALP,
        ARM64_INS_SADDLP,
        ARM64_INS_SADDLV,
        ARM64_INS_SADDL2,
        ARM64_INS_SADDL,
        ARM64_INS_SADDW2,
        ARM64_INS_SADDW,
        ARM64_INS_SBC,
        ARM64_INS_SBFM,
        ARM64_INS_SCVTF,
        ARM64_INS_SDIV,
        ARM64_INS_SHA1C,
        ARM64_INS_SHA1H,
        ARM64_INS_SHA1M,
        ARM64_INS_SHA1P,
        ARM64_INS_SHA1SU0,
        ARM64_INS_SHA1SU1,
        ARM64_INS_SHA256H2,
        ARM64_INS_SHA256H,
        ARM64_INS_SHA256SU0,
        ARM64_INS_SHA256SU1,
        ARM64_INS_SHADD,
        ARM64_INS_SHLL2,
        ARM64_INS_SHLL,
        ARM64_INS_SHL,
        ARM64_INS_SHRN2,
        ARM64_INS_SHRN,
        ARM64_INS_SHSUB,
        ARM64_INS_SLI,
        ARM64_INS_SMADDL,
        ARM64_INS_SMAXP,
        ARM64_INS_SMAXV,
        ARM64_INS_SMAX,
        ARM64_INS_SMC,
        ARM64_INS_SMINP,
        ARM64_INS_SMINV,
        ARM64_INS_SMIN,
        ARM64_INS_SMLAL2,
        ARM64_INS_SMLAL,
        ARM64_INS_SMLSL2,
        ARM64_INS_SMLSL,
        ARM64_INS_SMOV,
        ARM64_INS_SMSUBL,
        ARM64_INS_SMULH,
        ARM64_INS_SMULL2,
        ARM64_INS_SMULL,
        ARM64_INS_SQABS,
        ARM64_INS_SQADD,
        ARM64_INS_SQDMLAL,
        ARM64_INS_SQDMLAL2,
        ARM64_INS_SQDMLSL,
        ARM64_INS_SQDMLSL2,
        ARM64_INS_SQDMULH,
        ARM64_INS_SQDMULL,
        ARM64_INS_SQDMULL2,
        ARM64_INS_SQNEG,
        ARM64_INS_SQRDMULH,
        ARM64_INS_SQRSHL,
        ARM64_INS_SQRSHRN,
        ARM64_INS_SQRSHRN2,
        ARM64_INS_SQRSHRUN,
        ARM64_INS_SQRSHRUN2,
        ARM64_INS_SQSHLU,
        ARM64_INS_SQSHL,
        ARM64_INS_SQSHRN,
        ARM64_INS_SQSHRN2,
        ARM64_INS_SQSHRUN,
        ARM64_INS_SQSHRUN2,
        ARM64_INS_SQSUB,
        ARM64_INS_SQXTN2,
        ARM64_INS_SQXTN,
        ARM64_INS_SQXTUN2,
        ARM64_INS_SQXTUN,
        ARM64_INS_SRHADD,
        ARM64_INS_SRI,
        ARM64_INS_SRSHL,
        ARM64_INS_SRSHR,
        ARM64_INS_SRSRA,
        ARM64_INS_SSHLL2,
        ARM64_INS_SSHLL,
        ARM64_INS_SSHL,
        ARM64_INS_SSHR,
        ARM64_INS_SSRA,
        ARM64_INS_SSUBL2,
        ARM64_INS_SSUBL,
        ARM64_INS_SSUBW2,
        ARM64_INS_SSUBW,
        ARM64_INS_ST1,
        ARM64_INS_ST2,
        ARM64_INS_ST3,
        ARM64_INS_ST4,
        ARM64_INS_STLRB,
        ARM64_INS_STLRH,
        ARM64_INS_STLR,
        ARM64_INS_STLXP,
        ARM64_INS_STLXRB,
        ARM64_INS_STLXRH,
        ARM64_INS_STLXR,
        ARM64_INS_STNP,
        ARM64_INS_STP,
        ARM64_INS_STRB,
        ARM64_INS_STR,
        ARM64_INS_STRH,
        ARM64_INS_STTRB,
        ARM64_INS_STTRH,
        ARM64_INS_STTR,
        ARM64_INS_STURB,
        ARM64_INS_STUR,
        ARM64_INS_STURH,
        ARM64_INS_STXP,
        ARM64_INS_STXRB,
        ARM64_INS_STXRH,
        ARM64_INS_STXR,
        ARM64_INS_SUBHN,
        ARM64_INS_SUBHN2,
        ARM64_INS_SUB,
        ARM64_INS_SUQADD,
        ARM64_INS_SVC,
        ARM64_INS_SYSL,
        ARM64_INS_SYS,
        ARM64_INS_TBL,
        ARM64_INS_TBNZ,
        ARM64_INS_TBX,
        ARM64_INS_TBZ,
        ARM64_INS_TRN1,
        ARM64_INS_TRN2,
        ARM64_INS_UABAL2,
        ARM64_INS_UABAL,
        ARM64_INS_UABA,
        ARM64_INS_UABDL2,
        ARM64_INS_UABDL,
        ARM64_INS_UABD,
        ARM64_INS_UADALP,
        ARM64_INS_UADDLP,
        ARM64_INS_UADDLV,
        ARM64_INS_UADDL2,
        ARM64_INS_UADDL,
        ARM64_INS_UADDW2,
        ARM64_INS_UADDW,
        ARM64_INS_UBFM,
        ARM64_INS_UCVTF,
        ARM64_INS_UDIV,
        ARM64_INS_UHADD,
        ARM64_INS_UHSUB,
        ARM64_INS_UMADDL,
        ARM64_INS_UMAXP,
        ARM64_INS_UMAXV,
        ARM64_INS_UMAX,
        ARM64_INS_UMINP,
        ARM64_INS_UMINV,
        ARM64_INS_UMIN,
        ARM64_INS_UMLAL2,
        ARM64_INS_UMLAL,
        ARM64_INS_UMLSL2,
        ARM64_INS_UMLSL,
        ARM64_INS_UMOV,
        ARM64_INS_UMSUBL,
        ARM64_INS_UMULH,
        ARM64_INS_UMULL2,
        ARM64_INS_UMULL,
        ARM64_INS_UQADD,
        ARM64_INS_UQRSHL,
        ARM64_INS_UQRSHRN,
        ARM64_INS_UQRSHRN2,
        ARM64_INS_UQSHL,
        ARM64_INS_UQSHRN,
        ARM64_INS_UQSHRN2,
        ARM64_INS_UQSUB,
        ARM64_INS_UQXTN2,
        ARM64_INS_UQXTN,
        ARM64_INS_URECPE,
        ARM64_INS_URHADD,
        ARM64_INS_URSHL,
        ARM64_INS_URSHR,
        ARM64_INS_URSQRTE,
        ARM64_INS_URSRA,
        ARM64_INS_USHLL2,
        ARM64_INS_USHLL,
        ARM64_INS_USHL,
        ARM64_INS_USHR,
        ARM64_INS_USQADD,
        ARM64_INS_USRA,
        ARM64_INS_USUBL2,
        ARM64_INS_USUBL,
        ARM64_INS_USUBW2,
        ARM64_INS_USUBW,
        ARM64_INS_UZP1,
        ARM64_INS_UZP2,
        ARM64_INS_XTN2,
        ARM64_INS_XTN,
        ARM64_INS_ZIP1,
        ARM64_INS_ZIP2,

        // alias insn
        ARM64_INS_MNEG,
        ARM64_INS_UMNEGL,
        ARM64_INS_SMNEGL,
        ARM64_INS_NOP,
        ARM64_INS_YIELD,
        ARM64_INS_WFE,
        ARM64_INS_WFI,
        ARM64_INS_SEV,
        ARM64_INS_SEVL,
        ARM64_INS_NGC,
        ARM64_INS_SBFIZ,
        ARM64_INS_UBFIZ,
        ARM64_INS_SBFX,
        ARM64_INS_UBFX,
        ARM64_INS_BFI,
        ARM64_INS_BFXIL,
        ARM64_INS_CMN,
        ARM64_INS_MVN,
        ARM64_INS_TST,
        ARM64_INS_CSET,
        ARM64_INS_CINC,
        ARM64_INS_CSETM,
        ARM64_INS_CINV,
        ARM64_INS_CNEG,
        ARM64_INS_SXTB,
        ARM64_INS_SXTH,
        ARM64_INS_SXTW,
        ARM64_INS_CMP,
        ARM64_INS_UXTB,
        ARM64_INS_UXTH,
        ARM64_INS_UXTW,
        ARM64_INS_IC,
        ARM64_INS_DC,
        ARM64_INS_AT,
        ARM64_INS_TLBI,

        ARM64_INS_ENDING,  // <-- mark the end of the list of insn
    } arm64_insn;

    typedef enum
    {
        ARM64_REG_INVALID = 0,
        ARM64_REG_W0,
        ARM64_REG_W1,
        ARM64_REG_W2,
        ARM64_REG_W3,
        ARM64_REG_W4,
        ARM64_REG_W5,
        ARM64_REG_W6,
        ARM64_REG_W7,
        ARM64_REG_W8,
        ARM64_REG_W9,
        ARM64_REG_W10,
        ARM64_REG_W11,
        ARM64_REG_W12,
        ARM64_REG_W13,
        ARM64_REG_W14,
        ARM64_REG_W15,
        ARM64_REG_W16,
        ARM64_REG_W17,
        ARM64_REG_W18,
        ARM64_REG_W19,
        ARM64_REG_W20,
        ARM64_REG_W21,
        ARM64_REG_W22,
        ARM64_REG_W23,
        ARM64_REG_W24,
        ARM64_REG_W25,
        ARM64_REG_W26,
        ARM64_REG_W27,
        ARM64_REG_W28,
        ARM64_REG_W29,
        ARM64_REG_W30,
        ARM64_REG_WZR,
        ARM64_REG_X0,
        ARM64_REG_X1,
        ARM64_REG_X2,
        ARM64_REG_X3,
        ARM64_REG_X4,
        ARM64_REG_X5,
        ARM64_REG_X6,
        ARM64_REG_X7,
        ARM64_REG_X8,
        ARM64_REG_X9,
        ARM64_REG_X10,
        ARM64_REG_X11,
        ARM64_REG_X12,
        ARM64_REG_X13,
        ARM64_REG_X14,
        ARM64_REG_X15,
        ARM64_REG_X16,
        ARM64_REG_X17,
        ARM64_REG_X18,
        ARM64_REG_X19,
        ARM64_REG_X20,
        ARM64_REG_X21,
        ARM64_REG_X22,
        ARM64_REG_X23,
        ARM64_REG_X24,
        ARM64_REG_X25,
        ARM64_REG_X26,
        ARM64_REG_X27,
        ARM64_REG_X28,
        ARM64_REG_X29,
        ARM64_REG_X30,
        ARM64_REG_XZR,
        ARM64_REG_Q0,
        ARM64_REG_Q1,
        ARM64_REG_Q2,
        ARM64_REG_Q3,
        ARM64_REG_Q4,
        ARM64_REG_Q5,
        ARM64_REG_Q6,
        ARM64_REG_Q7,
        ARM64_REG_Q8,
        ARM64_REG_Q9,
        ARM64_REG_Q10,
        ARM64_REG_Q11,
        ARM64_REG_Q12,
        ARM64_REG_Q13,
        ARM64_REG_Q14,
        ARM64_REG_Q15,
        ARM64_REG_Q16,
        ARM64_REG_Q17,
        ARM64_REG_Q18,
        ARM64_REG_Q19,
        ARM64_REG_Q20,
        ARM64_REG_Q21,
        ARM64_REG_Q22,
        ARM64_REG_Q23,
        ARM64_REG_Q24,
        ARM64_REG_Q25,
        ARM64_REG_Q26,
        ARM64_REG_Q27,
        ARM64_REG_Q28,
        ARM64_REG_Q29,
        ARM64_REG_Q30,
        ARM64_REG_Q31,
        ARM64_REG_V0,
        ARM64_REG_V1,
        ARM64_REG_V2,
        ARM64_REG_V3,
        ARM64_REG_V4,
        ARM64_REG_V5,
        ARM64_REG_V6,
        ARM64_REG_V7,
        ARM64_REG_V8,
        ARM64_REG_V9,
        ARM64_REG_V10,
        ARM64_REG_V11,
        ARM64_REG_V12,
        ARM64_REG_V13,
        ARM64_REG_V14,
        ARM64_REG_V15,
        ARM64_REG_V16,
        ARM64_REG_V17,
        ARM64_REG_V18,
        ARM64_REG_V19,
        ARM64_REG_V20,
        ARM64_REG_V21,
        ARM64_REG_V22,
        ARM64_REG_V23,
        ARM64_REG_V24,
        ARM64_REG_V25,
        ARM64_REG_V26,
        ARM64_REG_V27,
        ARM64_REG_V28,
        ARM64_REG_V29,
        ARM64_REG_V30,
        ARM64_REG_V31,
        ARM64_REG_PC,
        ARM64_REG_SP,
        ARM64_REG_D0,
        ARM64_REG_D1,
        ARM64_REG_D2,
        ARM64_REG_D3,
        ARM64_REG_D4,
        ARM64_REG_D5,
        ARM64_REG_D6,
        ARM64_REG_D7,
        ARM64_REG_D8,
        ARM64_REG_D9,
        ARM64_REG_D10,
        ARM64_REG_D11,
        ARM64_REG_D12,
        ARM64_REG_D13,
        ARM64_REG_D14,
        ARM64_REG_D15,
        ARM64_REG_D16,
        ARM64_REG_D17,
        ARM64_REG_D18,
        ARM64_REG_D19,
        ARM64_REG_D20,
        ARM64_REG_D21,
        ARM64_REG_D22,
        ARM64_REG_D23,
        ARM64_REG_D24,
        ARM64_REG_D25,
        ARM64_REG_D26,
        ARM64_REG_D27,
        ARM64_REG_D28,
        ARM64_REG_D29,
        ARM64_REG_D30,
        ARM64_REG_D31,
        ARM64_REG_S0,
        ARM64_REG_S1,
        ARM64_REG_S2,
        ARM64_REG_S3,
        ARM64_REG_S4,
        ARM64_REG_S5,
        ARM64_REG_S6,
        ARM64_REG_S7,
        ARM64_REG_S8,
        ARM64_REG_S9,
        ARM64_REG_S10,
        ARM64_REG_S11,
        ARM64_REG_S12,
        ARM64_REG_S13,
        ARM64_REG_S14,
        ARM64_REG_S15,
        ARM64_REG_S16,
        ARM64_REG_S17,
        ARM64_REG_S18,
        ARM64_REG_S19,
        ARM64_REG_S20,
        ARM64_REG_S21,
        ARM64_REG_S22,
        ARM64_REG_S23,
        ARM64_REG_S24,
        ARM64_REG_S25,
        ARM64_REG_S26,
        ARM64_REG_S27,
        ARM64_REG_S28,
        ARM64_REG_S29,
        ARM64_REG_S30,
        ARM64_REG_S31,
        ARM64_REG_H0,
        ARM64_REG_H1,
        ARM64_REG_H2,
        ARM64_REG_H3,
        ARM64_REG_H4,
        ARM64_REG_H5,
        ARM64_REG_H6,
        ARM64_REG_H7,
        ARM64_REG_H8,
        ARM64_REG_H9,
        ARM64_REG_H10,
        ARM64_REG_H11,
        ARM64_REG_H12,
        ARM64_REG_H13,
        ARM64_REG_H14,
        ARM64_REG_H15,
        ARM64_REG_H16,
        ARM64_REG_H17,
        ARM64_REG_H18,
        ARM64_REG_H19,
        ARM64_REG_H20,
        ARM64_REG_H21,
        ARM64_REG_H22,
        ARM64_REG_H23,
        ARM64_REG_H24,
        ARM64_REG_H25,
        ARM64_REG_H26,
        ARM64_REG_H27,
        ARM64_REG_H28,
        ARM64_REG_H29,
        ARM64_REG_H30,
        ARM64_REG_H31,

        ARM64_REG_TPIDR_EL0,
        ARM64_REG_TPIDRRO_EL0,

        //Alias
        ARM64_REG_LR = ARM64_REG_X30,
    } arm64_reg;

    //> ARM64 condition code
    typedef enum arm64_cc 
    {
        ARM64_CC_INVALID = 0,
        ARM64_CC_EQ = 1,     // Equal
        ARM64_CC_NE = 2,     // Not equal:                 Not equal, or unordered
        ARM64_CC_HS = 3,     // Unsigned higher or same:   >, ==, or unordered
        ARM64_CC_LO = 4,     // Unsigned lower or same:    Less than
        ARM64_CC_MI = 5,     // Minus, negative:           Less than
        ARM64_CC_PL = 6,     // Plus, positive or zero:    >, ==, or unordered
        ARM64_CC_VS = 7,     // Overflow:                  Unordered
        ARM64_CC_VC = 8,     // No overflow:               Ordered
        ARM64_CC_HI = 9,     // Unsigned higher:           Greater than, or unordered
        ARM64_CC_LS = 10,     // Unsigned lower or same:    Less than or equal
        ARM64_CC_GE = 11,     // Greater than or equal:     Greater than or equal
        ARM64_CC_LT = 12,     // Less than:                 Less than, or unordered
        ARM64_CC_GT = 13,     // Signed greater than:       Greater than
        ARM64_CC_LE = 14,     // Signed less than or equal: <, ==, or unordered
        ARM64_CC_AL = 15,     // Always (unconditional):    Always (unconditional)
        ARM64_CC_NV = 16,     // Always (unconditional):   Always (unconditional)
                              // Note the NV exists purely to disassemble 0b1111. Execution
                              // is "always".
    } arm64_cc;

    enum arm64_op_type 
    {
        ARM64_OP_INVALID = 0, // = CS_OP_INVALID (Uninitialized).
        ARM64_OP_REG, // = CS_OP_REG (Register operand).
        ARM64_OP_IMM, // = CS_OP_IMM (Immediate operand).
        ARM64_OP_MEM, // = CS_OP_MEM (Memory operand).
        ARM64_OP_FP,  // = CS_OP_FP (Floating-Point operand).
        ARM64_OP_CIMM = 64, // C-Immediate
        ARM64_OP_REG_MRS, // MRS register operand.
        ARM64_OP_REG_MSR, // MSR register operand.
        ARM64_OP_PSTATE, // PState operand.
        ARM64_OP_SYS, // SYS operand for IC/DC/AT/TLBI instructions.
        ARM64_OP_PREFETCH, // Prefetch operand (PRFM).
        ARM64_OP_BARRIER, // Memory barrier operand (ISB/DMB/DSB instructions).
    };

    enum A64SysRegValues 
    {
        A64SysReg_Invalid = -1,               // Op0 Op1  CRn   CRm   Op2
        A64SysReg_PAN = 0xc213, // 11  000  0100  0010  011
        A64SysReg_UAO = 0xc214, // 11  000  0100  0010  100
        A64SysReg_OSDTRRX_EL1 = 0x8002, // 10  000  0000  0000  010
        A64SysReg_OSDTRTX_EL1 = 0x801a, // 10  000  0000  0011  010
        A64SysReg_TEECR32_EL1 = 0x9000, // 10  010  0000  0000  000
        A64SysReg_MDCCINT_EL1 = 0x8010, // 10  000  0000  0010  000
        A64SysReg_MDSCR_EL1 = 0x8012, // 10  000  0000  0010  010
        A64SysReg_DBGDTR_EL0 = 0x9820, // 10  011  0000  0100  000
        A64SysReg_OSECCR_EL1 = 0x8032, // 10  000  0000  0110  010
        A64SysReg_DBGVCR32_EL2 = 0xa038, // 10  100  0000  0111  000
        A64SysReg_DBGBVR0_EL1 = 0x8004, // 10  000  0000  0000  100
        A64SysReg_DBGBVR1_EL1 = 0x800c, // 10  000  0000  0001  100
        A64SysReg_DBGBVR2_EL1 = 0x8014, // 10  000  0000  0010  100
        A64SysReg_DBGBVR3_EL1 = 0x801c, // 10  000  0000  0011  100
        A64SysReg_DBGBVR4_EL1 = 0x8024, // 10  000  0000  0100  100
        A64SysReg_DBGBVR5_EL1 = 0x802c, // 10  000  0000  0101  100
        A64SysReg_DBGBVR6_EL1 = 0x8034, // 10  000  0000  0110  100
        A64SysReg_DBGBVR7_EL1 = 0x803c, // 10  000  0000  0111  100
        A64SysReg_DBGBVR8_EL1 = 0x8044, // 10  000  0000  1000  100
        A64SysReg_DBGBVR9_EL1 = 0x804c, // 10  000  0000  1001  100
        A64SysReg_DBGBVR10_EL1 = 0x8054, // 10  000  0000  1010  100
        A64SysReg_DBGBVR11_EL1 = 0x805c, // 10  000  0000  1011  100
        A64SysReg_DBGBVR12_EL1 = 0x8064, // 10  000  0000  1100  100
        A64SysReg_DBGBVR13_EL1 = 0x806c, // 10  000  0000  1101  100
        A64SysReg_DBGBVR14_EL1 = 0x8074, // 10  000  0000  1110  100
        A64SysReg_DBGBVR15_EL1 = 0x807c, // 10  000  0000  1111  100
        A64SysReg_DBGBCR0_EL1 = 0x8005, // 10  000  0000  0000  101
        A64SysReg_DBGBCR1_EL1 = 0x800d, // 10  000  0000  0001  101
        A64SysReg_DBGBCR2_EL1 = 0x8015, // 10  000  0000  0010  101
        A64SysReg_DBGBCR3_EL1 = 0x801d, // 10  000  0000  0011  101
        A64SysReg_DBGBCR4_EL1 = 0x8025, // 10  000  0000  0100  101
        A64SysReg_DBGBCR5_EL1 = 0x802d, // 10  000  0000  0101  101
        A64SysReg_DBGBCR6_EL1 = 0x8035, // 10  000  0000  0110  101
        A64SysReg_DBGBCR7_EL1 = 0x803d, // 10  000  0000  0111  101
        A64SysReg_DBGBCR8_EL1 = 0x8045, // 10  000  0000  1000  101
        A64SysReg_DBGBCR9_EL1 = 0x804d, // 10  000  0000  1001  101
        A64SysReg_DBGBCR10_EL1 = 0x8055, // 10  000  0000  1010  101
        A64SysReg_DBGBCR11_EL1 = 0x805d, // 10  000  0000  1011  101
        A64SysReg_DBGBCR12_EL1 = 0x8065, // 10  000  0000  1100  101
        A64SysReg_DBGBCR13_EL1 = 0x806d, // 10  000  0000  1101  101
        A64SysReg_DBGBCR14_EL1 = 0x8075, // 10  000  0000  1110  101
        A64SysReg_DBGBCR15_EL1 = 0x807d, // 10  000  0000  1111  101
        A64SysReg_DBGWVR0_EL1 = 0x8006, // 10  000  0000  0000  110
        A64SysReg_DBGWVR1_EL1 = 0x800e, // 10  000  0000  0001  110
        A64SysReg_DBGWVR2_EL1 = 0x8016, // 10  000  0000  0010  110
        A64SysReg_DBGWVR3_EL1 = 0x801e, // 10  000  0000  0011  110
        A64SysReg_DBGWVR4_EL1 = 0x8026, // 10  000  0000  0100  110
        A64SysReg_DBGWVR5_EL1 = 0x802e, // 10  000  0000  0101  110
        A64SysReg_DBGWVR6_EL1 = 0x8036, // 10  000  0000  0110  110
        A64SysReg_DBGWVR7_EL1 = 0x803e, // 10  000  0000  0111  110
        A64SysReg_DBGWVR8_EL1 = 0x8046, // 10  000  0000  1000  110
        A64SysReg_DBGWVR9_EL1 = 0x804e, // 10  000  0000  1001  110
        A64SysReg_DBGWVR10_EL1 = 0x8056, // 10  000  0000  1010  110
        A64SysReg_DBGWVR11_EL1 = 0x805e, // 10  000  0000  1011  110
        A64SysReg_DBGWVR12_EL1 = 0x8066, // 10  000  0000  1100  110
        A64SysReg_DBGWVR13_EL1 = 0x806e, // 10  000  0000  1101  110
        A64SysReg_DBGWVR14_EL1 = 0x8076, // 10  000  0000  1110  110
        A64SysReg_DBGWVR15_EL1 = 0x807e, // 10  000  0000  1111  110
        A64SysReg_DBGWCR0_EL1 = 0x8007, // 10  000  0000  0000  111
        A64SysReg_DBGWCR1_EL1 = 0x800f, // 10  000  0000  0001  111
        A64SysReg_DBGWCR2_EL1 = 0x8017, // 10  000  0000  0010  111
        A64SysReg_DBGWCR3_EL1 = 0x801f, // 10  000  0000  0011  111
        A64SysReg_DBGWCR4_EL1 = 0x8027, // 10  000  0000  0100  111
        A64SysReg_DBGWCR5_EL1 = 0x802f, // 10  000  0000  0101  111
        A64SysReg_DBGWCR6_EL1 = 0x8037, // 10  000  0000  0110  111
        A64SysReg_DBGWCR7_EL1 = 0x803f, // 10  000  0000  0111  111
        A64SysReg_DBGWCR8_EL1 = 0x8047, // 10  000  0000  1000  111
        A64SysReg_DBGWCR9_EL1 = 0x804f, // 10  000  0000  1001  111
        A64SysReg_DBGWCR10_EL1 = 0x8057, // 10  000  0000  1010  111
        A64SysReg_DBGWCR11_EL1 = 0x805f, // 10  000  0000  1011  111
        A64SysReg_DBGWCR12_EL1 = 0x8067, // 10  000  0000  1100  111
        A64SysReg_DBGWCR13_EL1 = 0x806f, // 10  000  0000  1101  111
        A64SysReg_DBGWCR14_EL1 = 0x8077, // 10  000  0000  1110  111
        A64SysReg_DBGWCR15_EL1 = 0x807f, // 10  000  0000  1111  111
        A64SysReg_TEEHBR32_EL1 = 0x9080, // 10  010  0001  0000  000
        A64SysReg_OSDLR_EL1 = 0x809c, // 10  000  0001  0011  100
        A64SysReg_DBGPRCR_EL1 = 0x80a4, // 10  000  0001  0100  100
        A64SysReg_DBGCLAIMSET_EL1 = 0x83c6, // 10  000  0111  1000  110
        A64SysReg_DBGCLAIMCLR_EL1 = 0x83ce, // 10  000  0111  1001  110
        A64SysReg_CSSELR_EL1 = 0xd000, // 11  010  0000  0000  000
        A64SysReg_VPIDR_EL2 = 0xe000, // 11  100  0000  0000  000
        A64SysReg_VMPIDR_EL2 = 0xe005, // 11  100  0000  0000  101
        A64SysReg_CPACR_EL1 = 0xc082, // 11  000  0001  0000  010
        A64SysReg_CPACR_EL12 = 0xe882, // 11  101  0001  0000  010
        A64SysReg_SCTLR_EL1 = 0xc080, // 11  000  0001  0000  000
        A64SysReg_SCTLR_EL12 = 0xe880, // 11  101  0001  0000  000
        A64SysReg_SCTLR_EL2 = 0xe080, // 11  100  0001  0000  000
        A64SysReg_SCTLR_EL3 = 0xf080, // 11  110  0001  0000  000
        A64SysReg_ACTLR_EL1 = 0xc081, // 11  000  0001  0000  001
        A64SysReg_ACTLR_EL2 = 0xe081, // 11  100  0001  0000  001
        A64SysReg_ACTLR_EL3 = 0xf081, // 11  110  0001  0000  001
        A64SysReg_HCR_EL2 = 0xe088, // 11  100  0001  0001  000
        A64SysReg_SCR_EL3 = 0xf088, // 11  110  0001  0001  000
        A64SysReg_MDCR_EL2 = 0xe089, // 11  100  0001  0001  001
        A64SysReg_SDER32_EL3 = 0xf089, // 11  110  0001  0001  001
        A64SysReg_CPTR_EL2 = 0xe08a, // 11  100  0001  0001  010
        A64SysReg_CPTR_EL3 = 0xf08a, // 11  110  0001  0001  010
        A64SysReg_HSTR_EL2 = 0xe08b, // 11  100  0001  0001  011
        A64SysReg_HACR_EL2 = 0xe08f, // 11  100  0001  0001  111
        A64SysReg_MDCR_EL3 = 0xf099, // 11  110  0001  0011  001
        A64SysReg_TTBR0_EL1 = 0xc100, // 11  000  0010  0000  000
        A64SysReg_TTBR0_EL12 = 0xe900, // 11  101  0010  0000  000
        A64SysReg_TTBR0_EL2 = 0xe100, // 11  100  0010  0000  000
        A64SysReg_TTBR0_EL3 = 0xf100, // 11  110  0010  0000  000
        A64SysReg_TTBR1_EL1 = 0xc101, // 11  000  0010  0000  001
        A64SysReg_TTBR1_EL12 = 0xe901, // 11  101  0010  0000  001
        A64SysReg_TTBR1_EL2 = 0xe101, // 11  100  0010  0000  001
        A64SysReg_TCR_EL1 = 0xc102, // 11  000  0010  0000  010
        A64SysReg_TCR_EL12 = 0xe902, // 11  101  0010  0000  010
        A64SysReg_TCR_EL2 = 0xe102, // 11  100  0010  0000  010
        A64SysReg_TCR_EL3 = 0xf102, // 11  110  0010  0000  010
        A64SysReg_VTTBR_EL2 = 0xe108, // 11  100  0010  0001  000
        A64SysReg_VTCR_EL2 = 0xe10a, // 11  100  0010  0001  010
        A64SysReg_DACR32_EL2 = 0xe180, // 11  100  0011  0000  000
        A64SysReg_SPSR_EL1 = 0xc200, // 11  000  0100  0000  000
        A64SysReg_SPSR_EL12 = 0xea00, // 11  101  0100  0000  000
        A64SysReg_SPSR_EL2 = 0xe200, // 11  100  0100  0000  000
        A64SysReg_SPSR_EL3 = 0xf200, // 11  110  0100  0000  000
        A64SysReg_ELR_EL1 = 0xc201, // 11  000  0100  0000  001
        A64SysReg_ELR_EL12 = 0xea01, // 11  101  0100  0000  001
        A64SysReg_ELR_EL2 = 0xe201, // 11  100  0100  0000  001
        A64SysReg_ELR_EL3 = 0xf201, // 11  110  0100  0000  001
        A64SysReg_SP_EL0 = 0xc208, // 11  000  0100  0001  000
        A64SysReg_SP_EL1 = 0xe208, // 11  100  0100  0001  000
        A64SysReg_SP_EL2 = 0xf208, // 11  110  0100  0001  000
        A64SysReg_SPSel = 0xc210, // 11  000  0100  0010  000
        A64SysReg_NZCV = 0xda10, // 11  011  0100  0010  000
        A64SysReg_DAIF = 0xda11, // 11  011  0100  0010  001
        A64SysReg_CurrentEL = 0xc212, // 11  000  0100  0010  010
        A64SysReg_SPSR_irq = 0xe218, // 11  100  0100  0011  000
        A64SysReg_SPSR_abt = 0xe219, // 11  100  0100  0011  001
        A64SysReg_SPSR_und = 0xe21a, // 11  100  0100  0011  010
        A64SysReg_SPSR_fiq = 0xe21b, // 11  100  0100  0011  011
        A64SysReg_FPCR = 0xda20, // 11  011  0100  0100  000
        A64SysReg_FPSR = 0xda21, // 11  011  0100  0100  001
        A64SysReg_DSPSR_EL0 = 0xda28, // 11  011  0100  0101  000
        A64SysReg_DLR_EL0 = 0xda29, // 11  011  0100  0101  001
        A64SysReg_IFSR32_EL2 = 0xe281, // 11  100  0101  0000  001
        A64SysReg_AFSR0_EL1 = 0xc288, // 11  000  0101  0001  000
        A64SysReg_AFSR0_EL12 = 0xea88, // 11  101  0101  0001  000
        A64SysReg_AFSR0_EL2 = 0xe288, // 11  100  0101  0001  000
        A64SysReg_AFSR0_EL3 = 0xf288, // 11  110  0101  0001  000
        A64SysReg_AFSR1_EL1 = 0xc289, // 11  000  0101  0001  001
        A64SysReg_AFSR1_EL12 = 0xea89, // 11  101  0101  0001  001
        A64SysReg_AFSR1_EL2 = 0xe289, // 11  100  0101  0001  001
        A64SysReg_AFSR1_EL3 = 0xf289, // 11  110  0101  0001  001
        A64SysReg_ESR_EL1 = 0xc290, // 11  000  0101  0010  000
        A64SysReg_ESR_EL12 = 0xea90, // 11  101  0101  0010  000
        A64SysReg_ESR_EL2 = 0xe290, // 11  100  0101  0010  000
        A64SysReg_ESR_EL3 = 0xf290, // 11  110  0101  0010  000
        A64SysReg_FPEXC32_EL2 = 0xe298, // 11  100  0101  0011  000
        A64SysReg_FAR_EL1 = 0xc300, // 11  000  0110  0000  000
        A64SysReg_FAR_EL12 = 0xeb00, // 11  101  0110  0000  000
        A64SysReg_FAR_EL2 = 0xe300, // 11  100  0110  0000  000
        A64SysReg_FAR_EL3 = 0xf300, // 11  110  0110  0000  000
        A64SysReg_HPFAR_EL2 = 0xe304, // 11  100  0110  0000  100
        A64SysReg_PAR_EL1 = 0xc3a0, // 11  000  0111  0100  000
        A64SysReg_PMCR_EL0 = 0xdce0, // 11  011  1001  1100  000
        A64SysReg_PMCNTENSET_EL0 = 0xdce1, // 11  011  1001  1100  001
        A64SysReg_PMCNTENCLR_EL0 = 0xdce2, // 11  011  1001  1100  010
        A64SysReg_PMOVSCLR_EL0 = 0xdce3, // 11  011  1001  1100  011
        A64SysReg_PMSELR_EL0 = 0xdce5, // 11  011  1001  1100  101
        A64SysReg_PMCCNTR_EL0 = 0xdce8, // 11  011  1001  1101  000
        A64SysReg_PMXEVTYPER_EL0 = 0xdce9, // 11  011  1001  1101  001
        A64SysReg_PMXEVCNTR_EL0 = 0xdcea, // 11  011  1001  1101  010
        A64SysReg_PMUSERENR_EL0 = 0xdcf0, // 11  011  1001  1110  000
        A64SysReg_PMINTENSET_EL1 = 0xc4f1, // 11  000  1001  1110  001
        A64SysReg_PMINTENCLR_EL1 = 0xc4f2, // 11  000  1001  1110  010
        A64SysReg_PMOVSSET_EL0 = 0xdcf3, // 11  011  1001  1110  011
        A64SysReg_MAIR_EL1 = 0xc510, // 11  000  1010  0010  000
        A64SysReg_MAIR_EL12 = 0xed10, // 11  101  1010  0010  000
        A64SysReg_MAIR_EL2 = 0xe510, // 11  100  1010  0010  000
        A64SysReg_MAIR_EL3 = 0xf510, // 11  110  1010  0010  000
        A64SysReg_AMAIR_EL1 = 0xc518, // 11  000  1010  0011  000
        A64SysReg_AMAIR_EL12 = 0xed18, // 11  101  1010  0011  000
        A64SysReg_AMAIR_EL2 = 0xe518, // 11  100  1010  0011  000
        A64SysReg_AMAIR_EL3 = 0xf518, // 11  110  1010  0011  000
        A64SysReg_VBAR_EL1 = 0xc600, // 11  000  1100  0000  000
        A64SysReg_VBAR_EL12 = 0xee00, // 11  101  1100  0000  000
        A64SysReg_VBAR_EL2 = 0xe600, // 11  100  1100  0000  000
        A64SysReg_VBAR_EL3 = 0xf600, // 11  110  1100  0000  000
        A64SysReg_RMR_EL1 = 0xc602, // 11  000  1100  0000  010
        A64SysReg_RMR_EL2 = 0xe602, // 11  100  1100  0000  010
        A64SysReg_RMR_EL3 = 0xf602, // 11  110  1100  0000  010
        A64SysReg_CONTEXTIDR_EL1 = 0xc681, // 11  000  1101  0000  001
        A64SysReg_CONTEXTIDR_EL12 = 0xee81, // 11  101  1101  0000  001
        A64SysReg_CONTEXTIDR_EL2 = 0xe681, // 11  100  1101  0000  001
        A64SysReg_TPIDR_EL0 = 0xde82, // 11  011  1101  0000  010
        A64SysReg_TPIDR_EL2 = 0xe682, // 11  100  1101  0000  010
        A64SysReg_TPIDR_EL3 = 0xf682, // 11  110  1101  0000  010
        A64SysReg_TPIDRRO_EL0 = 0xde83, // 11  011  1101  0000  011
        A64SysReg_TPIDR_EL1 = 0xc684, // 11  000  1101  0000  100
        A64SysReg_CNTFRQ_EL0 = 0xdf00, // 11  011  1110  0000  000
        A64SysReg_CNTVOFF_EL2 = 0xe703, // 11  100  1110  0000  011
        A64SysReg_CNTKCTL_EL1 = 0xc708, // 11  000  1110  0001  000
        A64SysReg_CNTKCTL_EL12 = 0xef08, // 11  101  1110  0001  000
        A64SysReg_CNTHCTL_EL2 = 0xe708, // 11  100  1110  0001  000
        A64SysReg_CNTHVCTL_EL2 = 0xe719, // 11  100  1110  0011  001
        A64SysReg_CNTHV_CVAL_EL2 = 0xe71a, // 11  100  1110  0011  010
        A64SysReg_CNTHV_TVAL_EL2 = 0xe718, // 11  100  1110  0011  000
        A64SysReg_CNTP_TVAL_EL0 = 0xdf10, // 11  011  1110  0010  000
        A64SysReg_CNTP_TVAL_EL02 = 0xef10, // 11  101  1110  0010  000
        A64SysReg_CNTHP_TVAL_EL2 = 0xe710, // 11  100  1110  0010  000
        A64SysReg_CNTPS_TVAL_EL1 = 0xff10, // 11  111  1110  0010  000
        A64SysReg_CNTP_CTL_EL0 = 0xdf11, // 11  011  1110  0010  001
        A64SysReg_CNTHP_CTL_EL2 = 0xe711, // 11  100  1110  0010  001
        A64SysReg_CNTPS_CTL_EL1 = 0xff11, // 11  111  1110  0010  001
        A64SysReg_CNTP_CVAL_EL0 = 0xdf12, // 11  011  1110  0010  010
        A64SysReg_CNTP_CVAL_EL02 = 0xef12, // 11  101  1110  0010  010
        A64SysReg_CNTHP_CVAL_EL2 = 0xe712, // 11  100  1110  0010  010
        A64SysReg_CNTPS_CVAL_EL1 = 0xff12, // 11  111  1110  0010  010
        A64SysReg_CNTV_TVAL_EL0 = 0xdf18, // 11  011  1110  0011  000
        A64SysReg_CNTV_TVAL_EL02 = 0xef18, // 11  101  1110  0011  000
        A64SysReg_CNTV_CTL_EL0 = 0xdf19, // 11  011  1110  0011  001
        A64SysReg_CNTV_CTL_EL02 = 0xef19, // 11  101  1110  0011  001
        A64SysReg_CNTV_CVAL_EL0 = 0xdf1a, // 11  011  1110  0011  010
        A64SysReg_CNTV_CVAL_EL02 = 0xef1a, // 11  101  1110  0011  010
        A64SysReg_PMEVCNTR0_EL0 = 0xdf40, // 11  011  1110  1000  000
        A64SysReg_PMEVCNTR1_EL0 = 0xdf41, // 11  011  1110  1000  001
        A64SysReg_PMEVCNTR2_EL0 = 0xdf42, // 11  011  1110  1000  010
        A64SysReg_PMEVCNTR3_EL0 = 0xdf43, // 11  011  1110  1000  011
        A64SysReg_PMEVCNTR4_EL0 = 0xdf44, // 11  011  1110  1000  100
        A64SysReg_PMEVCNTR5_EL0 = 0xdf45, // 11  011  1110  1000  101
        A64SysReg_PMEVCNTR6_EL0 = 0xdf46, // 11  011  1110  1000  110
        A64SysReg_PMEVCNTR7_EL0 = 0xdf47, // 11  011  1110  1000  111
        A64SysReg_PMEVCNTR8_EL0 = 0xdf48, // 11  011  1110  1001  000
        A64SysReg_PMEVCNTR9_EL0 = 0xdf49, // 11  011  1110  1001  001
        A64SysReg_PMEVCNTR10_EL0 = 0xdf4a, // 11  011  1110  1001  010
        A64SysReg_PMEVCNTR11_EL0 = 0xdf4b, // 11  011  1110  1001  011
        A64SysReg_PMEVCNTR12_EL0 = 0xdf4c, // 11  011  1110  1001  100
        A64SysReg_PMEVCNTR13_EL0 = 0xdf4d, // 11  011  1110  1001  101
        A64SysReg_PMEVCNTR14_EL0 = 0xdf4e, // 11  011  1110  1001  110
        A64SysReg_PMEVCNTR15_EL0 = 0xdf4f, // 11  011  1110  1001  111
        A64SysReg_PMEVCNTR16_EL0 = 0xdf50, // 11  011  1110  1010  000
        A64SysReg_PMEVCNTR17_EL0 = 0xdf51, // 11  011  1110  1010  001
        A64SysReg_PMEVCNTR18_EL0 = 0xdf52, // 11  011  1110  1010  010
        A64SysReg_PMEVCNTR19_EL0 = 0xdf53, // 11  011  1110  1010  011
        A64SysReg_PMEVCNTR20_EL0 = 0xdf54, // 11  011  1110  1010  100
        A64SysReg_PMEVCNTR21_EL0 = 0xdf55, // 11  011  1110  1010  101
        A64SysReg_PMEVCNTR22_EL0 = 0xdf56, // 11  011  1110  1010  110
        A64SysReg_PMEVCNTR23_EL0 = 0xdf57, // 11  011  1110  1010  111
        A64SysReg_PMEVCNTR24_EL0 = 0xdf58, // 11  011  1110  1011  000
        A64SysReg_PMEVCNTR25_EL0 = 0xdf59, // 11  011  1110  1011  001
        A64SysReg_PMEVCNTR26_EL0 = 0xdf5a, // 11  011  1110  1011  010
        A64SysReg_PMEVCNTR27_EL0 = 0xdf5b, // 11  011  1110  1011  011
        A64SysReg_PMEVCNTR28_EL0 = 0xdf5c, // 11  011  1110  1011  100
        A64SysReg_PMEVCNTR29_EL0 = 0xdf5d, // 11  011  1110  1011  101
        A64SysReg_PMEVCNTR30_EL0 = 0xdf5e, // 11  011  1110  1011  110
        A64SysReg_PMCCFILTR_EL0 = 0xdf7f, // 11  011  1110  1111  111
        A64SysReg_PMEVTYPER0_EL0 = 0xdf60, // 11  011  1110  1100  000
        A64SysReg_PMEVTYPER1_EL0 = 0xdf61, // 11  011  1110  1100  001
        A64SysReg_PMEVTYPER2_EL0 = 0xdf62, // 11  011  1110  1100  010
        A64SysReg_PMEVTYPER3_EL0 = 0xdf63, // 11  011  1110  1100  011
        A64SysReg_PMEVTYPER4_EL0 = 0xdf64, // 11  011  1110  1100  100
        A64SysReg_PMEVTYPER5_EL0 = 0xdf65, // 11  011  1110  1100  101
        A64SysReg_PMEVTYPER6_EL0 = 0xdf66, // 11  011  1110  1100  110
        A64SysReg_PMEVTYPER7_EL0 = 0xdf67, // 11  011  1110  1100  111
        A64SysReg_PMEVTYPER8_EL0 = 0xdf68, // 11  011  1110  1101  000
        A64SysReg_PMEVTYPER9_EL0 = 0xdf69, // 11  011  1110  1101  001
        A64SysReg_PMEVTYPER10_EL0 = 0xdf6a, // 11  011  1110  1101  010
        A64SysReg_PMEVTYPER11_EL0 = 0xdf6b, // 11  011  1110  1101  011
        A64SysReg_PMEVTYPER12_EL0 = 0xdf6c, // 11  011  1110  1101  100
        A64SysReg_PMEVTYPER13_EL0 = 0xdf6d, // 11  011  1110  1101  101
        A64SysReg_PMEVTYPER14_EL0 = 0xdf6e, // 11  011  1110  1101  110
        A64SysReg_PMEVTYPER15_EL0 = 0xdf6f, // 11  011  1110  1101  111
        A64SysReg_PMEVTYPER16_EL0 = 0xdf70, // 11  011  1110  1110  000
        A64SysReg_PMEVTYPER17_EL0 = 0xdf71, // 11  011  1110  1110  001
        A64SysReg_PMEVTYPER18_EL0 = 0xdf72, // 11  011  1110  1110  010
        A64SysReg_PMEVTYPER19_EL0 = 0xdf73, // 11  011  1110  1110  011
        A64SysReg_PMEVTYPER20_EL0 = 0xdf74, // 11  011  1110  1110  100
        A64SysReg_PMEVTYPER21_EL0 = 0xdf75, // 11  011  1110  1110  101
        A64SysReg_PMEVTYPER22_EL0 = 0xdf76, // 11  011  1110  1110  110
        A64SysReg_PMEVTYPER23_EL0 = 0xdf77, // 11  011  1110  1110  111
        A64SysReg_PMEVTYPER24_EL0 = 0xdf78, // 11  011  1110  1111  000
        A64SysReg_PMEVTYPER25_EL0 = 0xdf79, // 11  011  1110  1111  001
        A64SysReg_PMEVTYPER26_EL0 = 0xdf7a, // 11  011  1110  1111  010
        A64SysReg_PMEVTYPER27_EL0 = 0xdf7b, // 11  011  1110  1111  011
        A64SysReg_PMEVTYPER28_EL0 = 0xdf7c, // 11  011  1110  1111  100
        A64SysReg_PMEVTYPER29_EL0 = 0xdf7d, // 11  011  1110  1111  101
        A64SysReg_PMEVTYPER30_EL0 = 0xdf7e, // 11  011  1110  1111  110

        // Trace registers
        A64SysReg_TRCPRGCTLR = 0x8808, // 10  001  0000  0001  000
        A64SysReg_TRCPROCSELR = 0x8810, // 10  001  0000  0010  000
        A64SysReg_TRCCONFIGR = 0x8820, // 10  001  0000  0100  000
        A64SysReg_TRCAUXCTLR = 0x8830, // 10  001  0000  0110  000
        A64SysReg_TRCEVENTCTL0R = 0x8840, // 10  001  0000  1000  000
        A64SysReg_TRCEVENTCTL1R = 0x8848, // 10  001  0000  1001  000
        A64SysReg_TRCSTALLCTLR = 0x8858, // 10  001  0000  1011  000
        A64SysReg_TRCTSCTLR = 0x8860, // 10  001  0000  1100  000
        A64SysReg_TRCSYNCPR = 0x8868, // 10  001  0000  1101  000
        A64SysReg_TRCCCCTLR = 0x8870, // 10  001  0000  1110  000
        A64SysReg_TRCBBCTLR = 0x8878, // 10  001  0000  1111  000
        A64SysReg_TRCTRACEIDR = 0x8801, // 10  001  0000  0000  001
        A64SysReg_TRCQCTLR = 0x8809, // 10  001  0000  0001  001
        A64SysReg_TRCVICTLR = 0x8802, // 10  001  0000  0000  010
        A64SysReg_TRCVIIECTLR = 0x880a, // 10  001  0000  0001  010
        A64SysReg_TRCVISSCTLR = 0x8812, // 10  001  0000  0010  010
        A64SysReg_TRCVIPCSSCTLR = 0x881a, // 10  001  0000  0011  010
        A64SysReg_TRCVDCTLR = 0x8842, // 10  001  0000  1000  010
        A64SysReg_TRCVDSACCTLR = 0x884a, // 10  001  0000  1001  010
        A64SysReg_TRCVDARCCTLR = 0x8852, // 10  001  0000  1010  010
        A64SysReg_TRCSEQEVR0 = 0x8804, // 10  001  0000  0000  100
        A64SysReg_TRCSEQEVR1 = 0x880c, // 10  001  0000  0001  100
        A64SysReg_TRCSEQEVR2 = 0x8814, // 10  001  0000  0010  100
        A64SysReg_TRCSEQRSTEVR = 0x8834, // 10  001  0000  0110  100
        A64SysReg_TRCSEQSTR = 0x883c, // 10  001  0000  0111  100
        A64SysReg_TRCEXTINSELR = 0x8844, // 10  001  0000  1000  100
        A64SysReg_TRCCNTRLDVR0 = 0x8805, // 10  001  0000  0000  101
        A64SysReg_TRCCNTRLDVR1 = 0x880d, // 10  001  0000  0001  101
        A64SysReg_TRCCNTRLDVR2 = 0x8815, // 10  001  0000  0010  101
        A64SysReg_TRCCNTRLDVR3 = 0x881d, // 10  001  0000  0011  101
        A64SysReg_TRCCNTCTLR0 = 0x8825, // 10  001  0000  0100  101
        A64SysReg_TRCCNTCTLR1 = 0x882d, // 10  001  0000  0101  101
        A64SysReg_TRCCNTCTLR2 = 0x8835, // 10  001  0000  0110  101
        A64SysReg_TRCCNTCTLR3 = 0x883d, // 10  001  0000  0111  101
        A64SysReg_TRCCNTVR0 = 0x8845, // 10  001  0000  1000  101
        A64SysReg_TRCCNTVR1 = 0x884d, // 10  001  0000  1001  101
        A64SysReg_TRCCNTVR2 = 0x8855, // 10  001  0000  1010  101
        A64SysReg_TRCCNTVR3 = 0x885d, // 10  001  0000  1011  101
        A64SysReg_TRCIMSPEC0 = 0x8807, // 10  001  0000  0000  111
        A64SysReg_TRCIMSPEC1 = 0x880f, // 10  001  0000  0001  111
        A64SysReg_TRCIMSPEC2 = 0x8817, // 10  001  0000  0010  111
        A64SysReg_TRCIMSPEC3 = 0x881f, // 10  001  0000  0011  111
        A64SysReg_TRCIMSPEC4 = 0x8827, // 10  001  0000  0100  111
        A64SysReg_TRCIMSPEC5 = 0x882f, // 10  001  0000  0101  111
        A64SysReg_TRCIMSPEC6 = 0x8837, // 10  001  0000  0110  111
        A64SysReg_TRCIMSPEC7 = 0x883f, // 10  001  0000  0111  111
        A64SysReg_TRCRSCTLR2 = 0x8890, // 10  001  0001  0010  000
        A64SysReg_TRCRSCTLR3 = 0x8898, // 10  001  0001  0011  000
        A64SysReg_TRCRSCTLR4 = 0x88a0, // 10  001  0001  0100  000
        A64SysReg_TRCRSCTLR5 = 0x88a8, // 10  001  0001  0101  000
        A64SysReg_TRCRSCTLR6 = 0x88b0, // 10  001  0001  0110  000
        A64SysReg_TRCRSCTLR7 = 0x88b8, // 10  001  0001  0111  000
        A64SysReg_TRCRSCTLR8 = 0x88c0, // 10  001  0001  1000  000
        A64SysReg_TRCRSCTLR9 = 0x88c8, // 10  001  0001  1001  000
        A64SysReg_TRCRSCTLR10 = 0x88d0, // 10  001  0001  1010  000
        A64SysReg_TRCRSCTLR11 = 0x88d8, // 10  001  0001  1011  000
        A64SysReg_TRCRSCTLR12 = 0x88e0, // 10  001  0001  1100  000
        A64SysReg_TRCRSCTLR13 = 0x88e8, // 10  001  0001  1101  000
        A64SysReg_TRCRSCTLR14 = 0x88f0, // 10  001  0001  1110  000
        A64SysReg_TRCRSCTLR15 = 0x88f8, // 10  001  0001  1111  000
        A64SysReg_TRCRSCTLR16 = 0x8881, // 10  001  0001  0000  001
        A64SysReg_TRCRSCTLR17 = 0x8889, // 10  001  0001  0001  001
        A64SysReg_TRCRSCTLR18 = 0x8891, // 10  001  0001  0010  001
        A64SysReg_TRCRSCTLR19 = 0x8899, // 10  001  0001  0011  001
        A64SysReg_TRCRSCTLR20 = 0x88a1, // 10  001  0001  0100  001
        A64SysReg_TRCRSCTLR21 = 0x88a9, // 10  001  0001  0101  001
        A64SysReg_TRCRSCTLR22 = 0x88b1, // 10  001  0001  0110  001
        A64SysReg_TRCRSCTLR23 = 0x88b9, // 10  001  0001  0111  001
        A64SysReg_TRCRSCTLR24 = 0x88c1, // 10  001  0001  1000  001
        A64SysReg_TRCRSCTLR25 = 0x88c9, // 10  001  0001  1001  001
        A64SysReg_TRCRSCTLR26 = 0x88d1, // 10  001  0001  1010  001
        A64SysReg_TRCRSCTLR27 = 0x88d9, // 10  001  0001  1011  001
        A64SysReg_TRCRSCTLR28 = 0x88e1, // 10  001  0001  1100  001
        A64SysReg_TRCRSCTLR29 = 0x88e9, // 10  001  0001  1101  001
        A64SysReg_TRCRSCTLR30 = 0x88f1, // 10  001  0001  1110  001
        A64SysReg_TRCRSCTLR31 = 0x88f9, // 10  001  0001  1111  001
        A64SysReg_TRCSSCCR0 = 0x8882, // 10  001  0001  0000  010
        A64SysReg_TRCSSCCR1 = 0x888a, // 10  001  0001  0001  010
        A64SysReg_TRCSSCCR2 = 0x8892, // 10  001  0001  0010  010
        A64SysReg_TRCSSCCR3 = 0x889a, // 10  001  0001  0011  010
        A64SysReg_TRCSSCCR4 = 0x88a2, // 10  001  0001  0100  010
        A64SysReg_TRCSSCCR5 = 0x88aa, // 10  001  0001  0101  010
        A64SysReg_TRCSSCCR6 = 0x88b2, // 10  001  0001  0110  010
        A64SysReg_TRCSSCCR7 = 0x88ba, // 10  001  0001  0111  010
        A64SysReg_TRCSSCSR0 = 0x88c2, // 10  001  0001  1000  010
        A64SysReg_TRCSSCSR1 = 0x88ca, // 10  001  0001  1001  010
        A64SysReg_TRCSSCSR2 = 0x88d2, // 10  001  0001  1010  010
        A64SysReg_TRCSSCSR3 = 0x88da, // 10  001  0001  1011  010
        A64SysReg_TRCSSCSR4 = 0x88e2, // 10  001  0001  1100  010
        A64SysReg_TRCSSCSR5 = 0x88ea, // 10  001  0001  1101  010
        A64SysReg_TRCSSCSR6 = 0x88f2, // 10  001  0001  1110  010
        A64SysReg_TRCSSCSR7 = 0x88fa, // 10  001  0001  1111  010
        A64SysReg_TRCSSPCICR0 = 0x8883, // 10  001  0001  0000  011
        A64SysReg_TRCSSPCICR1 = 0x888b, // 10  001  0001  0001  011
        A64SysReg_TRCSSPCICR2 = 0x8893, // 10  001  0001  0010  011
        A64SysReg_TRCSSPCICR3 = 0x889b, // 10  001  0001  0011  011
        A64SysReg_TRCSSPCICR4 = 0x88a3, // 10  001  0001  0100  011
        A64SysReg_TRCSSPCICR5 = 0x88ab, // 10  001  0001  0101  011
        A64SysReg_TRCSSPCICR6 = 0x88b3, // 10  001  0001  0110  011
        A64SysReg_TRCSSPCICR7 = 0x88bb, // 10  001  0001  0111  011
        A64SysReg_TRCPDCR = 0x88a4, // 10  001  0001  0100  100
        A64SysReg_TRCACVR0 = 0x8900, // 10  001  0010  0000  000
        A64SysReg_TRCACVR1 = 0x8910, // 10  001  0010  0010  000
        A64SysReg_TRCACVR2 = 0x8920, // 10  001  0010  0100  000
        A64SysReg_TRCACVR3 = 0x8930, // 10  001  0010  0110  000
        A64SysReg_TRCACVR4 = 0x8940, // 10  001  0010  1000  000
        A64SysReg_TRCACVR5 = 0x8950, // 10  001  0010  1010  000
        A64SysReg_TRCACVR6 = 0x8960, // 10  001  0010  1100  000
        A64SysReg_TRCACVR7 = 0x8970, // 10  001  0010  1110  000
        A64SysReg_TRCACVR8 = 0x8901, // 10  001  0010  0000  001
        A64SysReg_TRCACVR9 = 0x8911, // 10  001  0010  0010  001
        A64SysReg_TRCACVR10 = 0x8921, // 10  001  0010  0100  001
        A64SysReg_TRCACVR11 = 0x8931, // 10  001  0010  0110  001
        A64SysReg_TRCACVR12 = 0x8941, // 10  001  0010  1000  001
        A64SysReg_TRCACVR13 = 0x8951, // 10  001  0010  1010  001
        A64SysReg_TRCACVR14 = 0x8961, // 10  001  0010  1100  001
        A64SysReg_TRCACVR15 = 0x8971, // 10  001  0010  1110  001
        A64SysReg_TRCACATR0 = 0x8902, // 10  001  0010  0000  010
        A64SysReg_TRCACATR1 = 0x8912, // 10  001  0010  0010  010
        A64SysReg_TRCACATR2 = 0x8922, // 10  001  0010  0100  010
        A64SysReg_TRCACATR3 = 0x8932, // 10  001  0010  0110  010
        A64SysReg_TRCACATR4 = 0x8942, // 10  001  0010  1000  010
        A64SysReg_TRCACATR5 = 0x8952, // 10  001  0010  1010  010
        A64SysReg_TRCACATR6 = 0x8962, // 10  001  0010  1100  010
        A64SysReg_TRCACATR7 = 0x8972, // 10  001  0010  1110  010
        A64SysReg_TRCACATR8 = 0x8903, // 10  001  0010  0000  011
        A64SysReg_TRCACATR9 = 0x8913, // 10  001  0010  0010  011
        A64SysReg_TRCACATR10 = 0x8923, // 10  001  0010  0100  011
        A64SysReg_TRCACATR11 = 0x8933, // 10  001  0010  0110  011
        A64SysReg_TRCACATR12 = 0x8943, // 10  001  0010  1000  011
        A64SysReg_TRCACATR13 = 0x8953, // 10  001  0010  1010  011
        A64SysReg_TRCACATR14 = 0x8963, // 10  001  0010  1100  011
        A64SysReg_TRCACATR15 = 0x8973, // 10  001  0010  1110  011
        A64SysReg_TRCDVCVR0 = 0x8904, // 10  001  0010  0000  100
        A64SysReg_TRCDVCVR1 = 0x8924, // 10  001  0010  0100  100
        A64SysReg_TRCDVCVR2 = 0x8944, // 10  001  0010  1000  100
        A64SysReg_TRCDVCVR3 = 0x8964, // 10  001  0010  1100  100
        A64SysReg_TRCDVCVR4 = 0x8905, // 10  001  0010  0000  101
        A64SysReg_TRCDVCVR5 = 0x8925, // 10  001  0010  0100  101
        A64SysReg_TRCDVCVR6 = 0x8945, // 10  001  0010  1000  101
        A64SysReg_TRCDVCVR7 = 0x8965, // 10  001  0010  1100  101
        A64SysReg_TRCDVCMR0 = 0x8906, // 10  001  0010  0000  110
        A64SysReg_TRCDVCMR1 = 0x8926, // 10  001  0010  0100  110
        A64SysReg_TRCDVCMR2 = 0x8946, // 10  001  0010  1000  110
        A64SysReg_TRCDVCMR3 = 0x8966, // 10  001  0010  1100  110
        A64SysReg_TRCDVCMR4 = 0x8907, // 10  001  0010  0000  111
        A64SysReg_TRCDVCMR5 = 0x8927, // 10  001  0010  0100  111
        A64SysReg_TRCDVCMR6 = 0x8947, // 10  001  0010  1000  111
        A64SysReg_TRCDVCMR7 = 0x8967, // 10  001  0010  1100  111
        A64SysReg_TRCCIDCVR0 = 0x8980, // 10  001  0011  0000  000
        A64SysReg_TRCCIDCVR1 = 0x8990, // 10  001  0011  0010  000
        A64SysReg_TRCCIDCVR2 = 0x89a0, // 10  001  0011  0100  000
        A64SysReg_TRCCIDCVR3 = 0x89b0, // 10  001  0011  0110  000
        A64SysReg_TRCCIDCVR4 = 0x89c0, // 10  001  0011  1000  000
        A64SysReg_TRCCIDCVR5 = 0x89d0, // 10  001  0011  1010  000
        A64SysReg_TRCCIDCVR6 = 0x89e0, // 10  001  0011  1100  000
        A64SysReg_TRCCIDCVR7 = 0x89f0, // 10  001  0011  1110  000
        A64SysReg_TRCVMIDCVR0 = 0x8981, // 10  001  0011  0000  001
        A64SysReg_TRCVMIDCVR1 = 0x8991, // 10  001  0011  0010  001
        A64SysReg_TRCVMIDCVR2 = 0x89a1, // 10  001  0011  0100  001
        A64SysReg_TRCVMIDCVR3 = 0x89b1, // 10  001  0011  0110  001
        A64SysReg_TRCVMIDCVR4 = 0x89c1, // 10  001  0011  1000  001
        A64SysReg_TRCVMIDCVR5 = 0x89d1, // 10  001  0011  1010  001
        A64SysReg_TRCVMIDCVR6 = 0x89e1, // 10  001  0011  1100  001
        A64SysReg_TRCVMIDCVR7 = 0x89f1, // 10  001  0011  1110  001
        A64SysReg_TRCCIDCCTLR0 = 0x8982, // 10  001  0011  0000  010
        A64SysReg_TRCCIDCCTLR1 = 0x898a, // 10  001  0011  0001  010
        A64SysReg_TRCVMIDCCTLR0 = 0x8992, // 10  001  0011  0010  010
        A64SysReg_TRCVMIDCCTLR1 = 0x899a, // 10  001  0011  0011  010
        A64SysReg_TRCITCTRL = 0x8b84, // 10  001  0111  0000  100
        A64SysReg_TRCCLAIMSET = 0x8bc6, // 10  001  0111  1000  110
        A64SysReg_TRCCLAIMCLR = 0x8bce, // 10  001  0111  1001  110

        // GICv3 registers
        A64SysReg_ICC_BPR1_EL1 = 0xc663, // 11  000  1100  1100  011
        A64SysReg_ICC_BPR0_EL1 = 0xc643, // 11  000  1100  1000  011
        A64SysReg_ICC_PMR_EL1 = 0xc230, // 11  000  0100  0110  000
        A64SysReg_ICC_CTLR_EL1 = 0xc664, // 11  000  1100  1100  100
        A64SysReg_ICC_CTLR_EL3 = 0xf664, // 11  110  1100  1100  100
        A64SysReg_ICC_SRE_EL1 = 0xc665, // 11  000  1100  1100  101
        A64SysReg_ICC_SRE_EL2 = 0xe64d, // 11  100  1100  1001  101
        A64SysReg_ICC_SRE_EL3 = 0xf665, // 11  110  1100  1100  101
        A64SysReg_ICC_IGRPEN0_EL1 = 0xc666, // 11  000  1100  1100  110
        A64SysReg_ICC_IGRPEN1_EL1 = 0xc667, // 11  000  1100  1100  111
        A64SysReg_ICC_IGRPEN1_EL3 = 0xf667, // 11  110  1100  1100  111
        A64SysReg_ICC_SEIEN_EL1 = 0xc668, // 11  000  1100  1101  000
        A64SysReg_ICC_AP0R0_EL1 = 0xc644, // 11  000  1100  1000  100
        A64SysReg_ICC_AP0R1_EL1 = 0xc645, // 11  000  1100  1000  101
        A64SysReg_ICC_AP0R2_EL1 = 0xc646, // 11  000  1100  1000  110
        A64SysReg_ICC_AP0R3_EL1 = 0xc647, // 11  000  1100  1000  111
        A64SysReg_ICC_AP1R0_EL1 = 0xc648, // 11  000  1100  1001  000
        A64SysReg_ICC_AP1R1_EL1 = 0xc649, // 11  000  1100  1001  001
        A64SysReg_ICC_AP1R2_EL1 = 0xc64a, // 11  000  1100  1001  010
        A64SysReg_ICC_AP1R3_EL1 = 0xc64b, // 11  000  1100  1001  011
        A64SysReg_ICH_AP0R0_EL2 = 0xe640, // 11  100  1100  1000  000
        A64SysReg_ICH_AP0R1_EL2 = 0xe641, // 11  100  1100  1000  001
        A64SysReg_ICH_AP0R2_EL2 = 0xe642, // 11  100  1100  1000  010
        A64SysReg_ICH_AP0R3_EL2 = 0xe643, // 11  100  1100  1000  011
        A64SysReg_ICH_AP1R0_EL2 = 0xe648, // 11  100  1100  1001  000
        A64SysReg_ICH_AP1R1_EL2 = 0xe649, // 11  100  1100  1001  001
        A64SysReg_ICH_AP1R2_EL2 = 0xe64a, // 11  100  1100  1001  010
        A64SysReg_ICH_AP1R3_EL2 = 0xe64b, // 11  100  1100  1001  011
        A64SysReg_ICH_HCR_EL2 = 0xe658, // 11  100  1100  1011  000
        A64SysReg_ICH_MISR_EL2 = 0xe65a, // 11  100  1100  1011  010
        A64SysReg_ICH_VMCR_EL2 = 0xe65f, // 11  100  1100  1011  111
        A64SysReg_ICH_VSEIR_EL2 = 0xe64c, // 11  100  1100  1001  100
        A64SysReg_ICH_LR0_EL2 = 0xe660, // 11  100  1100  1100  000
        A64SysReg_ICH_LR1_EL2 = 0xe661, // 11  100  1100  1100  001
        A64SysReg_ICH_LR2_EL2 = 0xe662, // 11  100  1100  1100  010
        A64SysReg_ICH_LR3_EL2 = 0xe663, // 11  100  1100  1100  011
        A64SysReg_ICH_LR4_EL2 = 0xe664, // 11  100  1100  1100  100
        A64SysReg_ICH_LR5_EL2 = 0xe665, // 11  100  1100  1100  101
        A64SysReg_ICH_LR6_EL2 = 0xe666, // 11  100  1100  1100  110
        A64SysReg_ICH_LR7_EL2 = 0xe667, // 11  100  1100  1100  111
        A64SysReg_ICH_LR8_EL2 = 0xe668, // 11  100  1100  1101  000
        A64SysReg_ICH_LR9_EL2 = 0xe669, // 11  100  1100  1101  001
        A64SysReg_ICH_LR10_EL2 = 0xe66a, // 11  100  1100  1101  010
        A64SysReg_ICH_LR11_EL2 = 0xe66b, // 11  100  1100  1101  011
        A64SysReg_ICH_LR12_EL2 = 0xe66c, // 11  100  1100  1101  100
        A64SysReg_ICH_LR13_EL2 = 0xe66d, // 11  100  1100  1101  101
        A64SysReg_ICH_LR14_EL2 = 0xe66e, // 11  100  1100  1101  110
        A64SysReg_ICH_LR15_EL2 = 0xe66f, // 11  100  1100  1101  111

        // Statistical profiling registers
        A64SysReg_PMSIDR_EL1 = 0xc4cf, // 11  000  1001  1001  111
        A64SysReg_PMBIDR_EL1 = 0xc4d7, // 11  000  1001  1010  111
        A64SysReg_PMBLIMITR_EL1 = 0xc4d0, // 11  000  1001  1010  000
        A64SysReg_PMBPTR_EL1 = 0xc4d1, // 11  000  1001  1010  001
        A64SysReg_PMBSR_EL1 = 0xc4d3, // 11  000  1001  1010  011
        A64SysReg_PMSCR_EL1 = 0xc4c8, // 11  000  1001  1001  000
        A64SysReg_PMSCR_EL12 = 0xecc8, // 11  101  1001  1001  000
        A64SysReg_PMSCR_EL2 = 0xe4c8, // 11  100  1001  1001  000
        A64SysReg_PMSICR_EL1 = 0xc4ca, // 11  000  1001  1001  010
        A64SysReg_PMSIRR_EL1 = 0xc4cb, // 11  000  1001  1001  011
        A64SysReg_PMSFCR_EL1 = 0xc4cc, // 11  000  1001  1001  100
        A64SysReg_PMSEVFR_EL1 = 0xc4cd, // 11  000  1001  1001  101
        A64SysReg_PMSLATFR_EL1 = 0xc4ce,  // 11  000  1001  1001  110
        A64SysReg_MDCCSR_EL0 = 0x9808, // 10  011  0000  0001  000
        A64SysReg_DBGDTRRX_EL0 = 0x9828, // 10  011  0000  0101  000
        A64SysReg_MDRAR_EL1 = 0x8080, // 10  000  0001  0000  000
        A64SysReg_OSLSR_EL1 = 0x808c, // 10  000  0001  0001  100
        A64SysReg_DBGAUTHSTATUS_EL1 = 0x83f6, // 10  000  0111  1110  110
        A64SysReg_PMCEID0_EL0 = 0xdce6, // 11  011  1001  1100  110
        A64SysReg_PMCEID1_EL0 = 0xdce7, // 11  011  1001  1100  111
        A64SysReg_MIDR_EL1 = 0xc000, // 11  000  0000  0000  000
        A64SysReg_CCSIDR_EL1 = 0xc800, // 11  001  0000  0000  000
        A64SysReg_CLIDR_EL1 = 0xc801, // 11  001  0000  0000  001
        A64SysReg_CTR_EL0 = 0xd801, // 11  011  0000  0000  001
        A64SysReg_MPIDR_EL1 = 0xc005, // 11  000  0000  0000  101
        A64SysReg_REVIDR_EL1 = 0xc006, // 11  000  0000  0000  110
        A64SysReg_AIDR_EL1 = 0xc807, // 11  001  0000  0000  111
        A64SysReg_DCZID_EL0 = 0xd807, // 11  011  0000  0000  111
        A64SysReg_ID_PFR0_EL1 = 0xc008, // 11  000  0000  0001  000
        A64SysReg_ID_PFR1_EL1 = 0xc009, // 11  000  0000  0001  001
        A64SysReg_ID_DFR0_EL1 = 0xc00a, // 11  000  0000  0001  010
        A64SysReg_ID_AFR0_EL1 = 0xc00b, // 11  000  0000  0001  011
        A64SysReg_ID_MMFR0_EL1 = 0xc00c, // 11  000  0000  0001  100
        A64SysReg_ID_MMFR1_EL1 = 0xc00d, // 11  000  0000  0001  101
        A64SysReg_ID_MMFR2_EL1 = 0xc00e, // 11  000  0000  0001  110
        A64SysReg_ID_MMFR3_EL1 = 0xc00f, // 11  000  0000  0001  111
        A64SysReg_ID_MMFR4_EL1 = 0xc016, // 11  000  0000  0010  110
        A64SysReg_ID_ISAR0_EL1 = 0xc010, // 11  000  0000  0010  000
        A64SysReg_ID_ISAR1_EL1 = 0xc011, // 11  000  0000  0010  001
        A64SysReg_ID_ISAR2_EL1 = 0xc012, // 11  000  0000  0010  010
        A64SysReg_ID_ISAR3_EL1 = 0xc013, // 11  000  0000  0010  011
        A64SysReg_ID_ISAR4_EL1 = 0xc014, // 11  000  0000  0010  100
        A64SysReg_ID_ISAR5_EL1 = 0xc015, // 11  000  0000  0010  101
        A64SysReg_ID_A64PFR0_EL1 = 0xc020, // 11  000  0000  0100  000
        A64SysReg_ID_A64PFR1_EL1 = 0xc021, // 11  000  0000  0100  001
        A64SysReg_ID_A64DFR0_EL1 = 0xc028, // 11  000  0000  0101  000
        A64SysReg_ID_A64DFR1_EL1 = 0xc029, // 11  000  0000  0101  001
        A64SysReg_ID_A64AFR0_EL1 = 0xc02c, // 11  000  0000  0101  100
        A64SysReg_ID_A64AFR1_EL1 = 0xc02d, // 11  000  0000  0101  101
        A64SysReg_ID_A64ISAR0_EL1 = 0xc030, // 11  000  0000  0110  000
        A64SysReg_ID_A64ISAR1_EL1 = 0xc031, // 11  000  0000  0110  001
        A64SysReg_ID_A64MMFR0_EL1 = 0xc038, // 11  000  0000  0111  000
        A64SysReg_ID_A64MMFR1_EL1 = 0xc039, // 11  000  0000  0111  001
        A64SysReg_ID_A64MMFR2_EL1 = 0xC03A, // 11  000  0000  0111  010
        A64SysReg_LORC_EL1 = 0xc523, // 11  000  1010  0100  011
        A64SysReg_LOREA_EL1 = 0xc521, // 11  000  1010  0100  001
        A64SysReg_LORID_EL1 = 0xc527, // 11  000  1010  0100  111
        A64SysReg_LORN_EL1 = 0xc522, // 11  000  1010  0100  010
        A64SysReg_LORSA_EL1 = 0xc520, // 11  000  1010  0100  000
        A64SysReg_MVFR0_EL1 = 0xc018, // 11  000  0000  0011  000
        A64SysReg_MVFR1_EL1 = 0xc019, // 11  000  0000  0011  001
        A64SysReg_MVFR2_EL1 = 0xc01a, // 11  000  0000  0011  010
        A64SysReg_RVBAR_EL1 = 0xc601, // 11  000  1100  0000  001
        A64SysReg_RVBAR_EL2 = 0xe601, // 11  100  1100  0000  001
        A64SysReg_RVBAR_EL3 = 0xf601, // 11  110  1100  0000  001
        A64SysReg_ISR_EL1 = 0xc608, // 11  000  1100  0001  000
        A64SysReg_CNTPCT_EL0 = 0xdf01, // 11  011  1110  0000  001
        A64SysReg_CNTVCT_EL0 = 0xdf02,  // 11  011  1110  0000  010

                                        // Trace registers
        A64SysReg_TRCSTATR = 0x8818, // 10  001  0000  0011  000
        A64SysReg_TRCIDR8 = 0x8806, // 10  001  0000  0000  110
        A64SysReg_TRCIDR9 = 0x880e, // 10  001  0000  0001  110
        A64SysReg_TRCIDR10 = 0x8816, // 10  001  0000  0010  110
        A64SysReg_TRCIDR11 = 0x881e, // 10  001  0000  0011  110
        A64SysReg_TRCIDR12 = 0x8826, // 10  001  0000  0100  110
        A64SysReg_TRCIDR13 = 0x882e, // 10  001  0000  0101  110
        A64SysReg_TRCIDR0 = 0x8847, // 10  001  0000  1000  111
        A64SysReg_TRCIDR1 = 0x884f, // 10  001  0000  1001  111
        A64SysReg_TRCIDR2 = 0x8857, // 10  001  0000  1010  111
        A64SysReg_TRCIDR3 = 0x885f, // 10  001  0000  1011  111
        A64SysReg_TRCIDR4 = 0x8867, // 10  001  0000  1100  111
        A64SysReg_TRCIDR5 = 0x886f, // 10  001  0000  1101  111
        A64SysReg_TRCIDR6 = 0x8877, // 10  001  0000  1110  111
        A64SysReg_TRCIDR7 = 0x887f, // 10  001  0000  1111  111
        A64SysReg_TRCOSLSR = 0x888c, // 10  001  0001  0001  100
        A64SysReg_TRCPDSR = 0x88ac, // 10  001  0001  0101  100
        A64SysReg_TRCDEVAFF0 = 0x8bd6, // 10  001  0111  1010  110
        A64SysReg_TRCDEVAFF1 = 0x8bde, // 10  001  0111  1011  110
        A64SysReg_TRCLSR = 0x8bee, // 10  001  0111  1101  110
        A64SysReg_TRCAUTHSTATUS = 0x8bf6, // 10  001  0111  1110  110
        A64SysReg_TRCDEVARCH = 0x8bfe, // 10  001  0111  1111  110
        A64SysReg_TRCDEVID = 0x8b97, // 10  001  0111  0010  111
        A64SysReg_TRCDEVTYPE = 0x8b9f, // 10  001  0111  0011  111
        A64SysReg_TRCPIDR4 = 0x8ba7, // 10  001  0111  0100  111
        A64SysReg_TRCPIDR5 = 0x8baf, // 10  001  0111  0101  111
        A64SysReg_TRCPIDR6 = 0x8bb7, // 10  001  0111  0110  111
        A64SysReg_TRCPIDR7 = 0x8bbf, // 10  001  0111  0111  111
        A64SysReg_TRCPIDR0 = 0x8bc7, // 10  001  0111  1000  111
        A64SysReg_TRCPIDR1 = 0x8bcf, // 10  001  0111  1001  111
        A64SysReg_TRCPIDR2 = 0x8bd7, // 10  001  0111  1010  111
        A64SysReg_TRCPIDR3 = 0x8bdf, // 10  001  0111  1011  111
        A64SysReg_TRCCIDR0 = 0x8be7, // 10  001  0111  1100  111
        A64SysReg_TRCCIDR1 = 0x8bef, // 10  001  0111  1101  111
        A64SysReg_TRCCIDR2 = 0x8bf7, // 10  001  0111  1110  111
        A64SysReg_TRCCIDR3 = 0x8bff, // 10  001  0111  1111  111

        // GICv3 registers
        A64SysReg_ICC_IAR1_EL1 = 0xc660, // 11  000  1100  1100  000
        A64SysReg_ICC_IAR0_EL1 = 0xc640, // 11  000  1100  1000  000
        A64SysReg_ICC_HPPIR1_EL1 = 0xc662, // 11  000  1100  1100  010
        A64SysReg_ICC_HPPIR0_EL1 = 0xc642, // 11  000  1100  1000  010
        A64SysReg_ICC_RPR_EL1 = 0xc65b, // 11  000  1100  1011  011
        A64SysReg_ICH_VTR_EL2 = 0xe659, // 11  100  1100  1011  001
        A64SysReg_ICH_EISR_EL2 = 0xe65b, // 11  100  1100  1011  011
        A64SysReg_ICH_ELSR_EL2 = 0xe65d  // 11  100  1100  1011  101
    };

    typedef enum 
    {
        ARM64_SFT_INVALID = 0,
        ARM64_SFT_LSL = 1,
        ARM64_SFT_MSL = 2,
        ARM64_SFT_LSR = 3,
        ARM64_SFT_ASR = 4,
        ARM64_SFT_ROR = 5,
    } arm64_shifter;

    typedef enum
    {
        ARM64_EXT_INVALID = 0,
        ARM64_EXT_UXTB = 1,
        ARM64_EXT_UXTH = 2,
        ARM64_EXT_UXTW = 3,
        ARM64_EXT_UXTX = 4,
        ARM64_EXT_SXTB = 5,
        ARM64_EXT_SXTH = 6,
        ARM64_EXT_SXTW = 7,
        ARM64_EXT_SXTX = 8,
    } arm64_extender;

    typedef enum 
    {
        ARM64_VESS_INVALID = 0,
        ARM64_VESS_B,
        ARM64_VESS_H,
        ARM64_VESS_S,
        ARM64_VESS_D,
    } arm64_vess;
    
    typedef enum 
    {
        ARM64_VAS_INVALID = 0,
        ARM64_VAS_8B,
        ARM64_VAS_16B,
        ARM64_VAS_4H,
        ARM64_VAS_8H,
        ARM64_VAS_2S,
        ARM64_VAS_4S,
        ARM64_VAS_1D,
        ARM64_VAS_2D,
        ARM64_VAS_1Q,
    } arm64_vas;

    struct MCOperand
    {
        arm64_op_type type;
        arm64_reg Reg;
        int64_t ImmVal;
        double fp;
        A64SysRegValues SysReg;
        struct 
        {
            arm64_shifter type;	// shifter type of this operand
            unsigned int value;	// shifter value of this operand
        } shift;
        arm64_extender Extend;
        struct
        {
            arm64_reg base; // base register
            arm64_reg index; // index register
            int32_t disp; // displacement/offset value
        } mem;
        int32_t VectorIndex;
        arm64_vess Vess;
        arm64_vas Vas;
    };
private:
    class Arm64OpcodeDetail
    {
        friend Arm64OpcodeCache;

    public:
        Arm64OpcodeDetail(uint64_t pc, uint32_t insn);

        inline uint64_t PC(void) const { return m_pc; }
        inline const char * Name(void) const { return m_Name; }
        inline const char * Param(void) const { return m_Param; }
        inline instruct_t Opc(void) const { return m_Opc; }
        inline size_t Operands(void) const { return m_OperandCount; }
        inline const MCOperand & Operand(uint32_t index) const { return m_Operands[index]; }
        inline arm64_cc cc(void) const { return m_cc; }
        inline bool UpdateFlags(void) const { return m_UpdateFlags; }
        inline bool WriteBack(void) const { return m_WriteBack; }

    private:
        //> ARM64 registers
        typedef enum
        {
            CAPSTONE_ARM64_REG_INVALID = 0,

            CAPSTONE_ARM64_REG_X29,
            CAPSTONE_ARM64_REG_X30,
            CAPSTONE_ARM64_REG_NZCV,
            CAPSTONE_ARM64_REG_SP,
            CAPSTONE_ARM64_REG_WSP,
            CAPSTONE_ARM64_REG_WZR,
            CAPSTONE_ARM64_REG_XZR,
            CAPSTONE_ARM64_REG_B0,
            CAPSTONE_ARM64_REG_B1,
            CAPSTONE_ARM64_REG_B2,
            CAPSTONE_ARM64_REG_B3,
            CAPSTONE_ARM64_REG_B4,
            CAPSTONE_ARM64_REG_B5,
            CAPSTONE_ARM64_REG_B6,
            CAPSTONE_ARM64_REG_B7,
            CAPSTONE_ARM64_REG_B8,
            CAPSTONE_ARM64_REG_B9,
            CAPSTONE_ARM64_REG_B10,
            CAPSTONE_ARM64_REG_B11,
            CAPSTONE_ARM64_REG_B12,
            CAPSTONE_ARM64_REG_B13,
            CAPSTONE_ARM64_REG_B14,
            CAPSTONE_ARM64_REG_B15,
            CAPSTONE_ARM64_REG_B16,
            CAPSTONE_ARM64_REG_B17,
            CAPSTONE_ARM64_REG_B18,
            CAPSTONE_ARM64_REG_B19,
            CAPSTONE_ARM64_REG_B20,
            CAPSTONE_ARM64_REG_B21,
            CAPSTONE_ARM64_REG_B22,
            CAPSTONE_ARM64_REG_B23,
            CAPSTONE_ARM64_REG_B24,
            CAPSTONE_ARM64_REG_B25,
            CAPSTONE_ARM64_REG_B26,
            CAPSTONE_ARM64_REG_B27,
            CAPSTONE_ARM64_REG_B28,
            CAPSTONE_ARM64_REG_B29,
            CAPSTONE_ARM64_REG_B30,
            CAPSTONE_ARM64_REG_B31,
            CAPSTONE_ARM64_REG_D0,
            CAPSTONE_ARM64_REG_D1,
            CAPSTONE_ARM64_REG_D2,
            CAPSTONE_ARM64_REG_D3,
            CAPSTONE_ARM64_REG_D4,
            CAPSTONE_ARM64_REG_D5,
            CAPSTONE_ARM64_REG_D6,
            CAPSTONE_ARM64_REG_D7,
            CAPSTONE_ARM64_REG_D8,
            CAPSTONE_ARM64_REG_D9,
            CAPSTONE_ARM64_REG_D10,
            CAPSTONE_ARM64_REG_D11,
            CAPSTONE_ARM64_REG_D12,
            CAPSTONE_ARM64_REG_D13,
            CAPSTONE_ARM64_REG_D14,
            CAPSTONE_ARM64_REG_D15,
            CAPSTONE_ARM64_REG_D16,
            CAPSTONE_ARM64_REG_D17,
            CAPSTONE_ARM64_REG_D18,
            CAPSTONE_ARM64_REG_D19,
            CAPSTONE_ARM64_REG_D20,
            CAPSTONE_ARM64_REG_D21,
            CAPSTONE_ARM64_REG_D22,
            CAPSTONE_ARM64_REG_D23,
            CAPSTONE_ARM64_REG_D24,
            CAPSTONE_ARM64_REG_D25,
            CAPSTONE_ARM64_REG_D26,
            CAPSTONE_ARM64_REG_D27,
            CAPSTONE_ARM64_REG_D28,
            CAPSTONE_ARM64_REG_D29,
            CAPSTONE_ARM64_REG_D30,
            CAPSTONE_ARM64_REG_D31,
            CAPSTONE_ARM64_REG_H0,
            CAPSTONE_ARM64_REG_H1,
            CAPSTONE_ARM64_REG_H2,
            CAPSTONE_ARM64_REG_H3,
            CAPSTONE_ARM64_REG_H4,
            CAPSTONE_ARM64_REG_H5,
            CAPSTONE_ARM64_REG_H6,
            CAPSTONE_ARM64_REG_H7,
            CAPSTONE_ARM64_REG_H8,
            CAPSTONE_ARM64_REG_H9,
            CAPSTONE_ARM64_REG_H10,
            CAPSTONE_ARM64_REG_H11,
            CAPSTONE_ARM64_REG_H12,
            CAPSTONE_ARM64_REG_H13,
            CAPSTONE_ARM64_REG_H14,
            CAPSTONE_ARM64_REG_H15,
            CAPSTONE_ARM64_REG_H16,
            CAPSTONE_ARM64_REG_H17,
            CAPSTONE_ARM64_REG_H18,
            CAPSTONE_ARM64_REG_H19,
            CAPSTONE_ARM64_REG_H20,
            CAPSTONE_ARM64_REG_H21,
            CAPSTONE_ARM64_REG_H22,
            CAPSTONE_ARM64_REG_H23,
            CAPSTONE_ARM64_REG_H24,
            CAPSTONE_ARM64_REG_H25,
            CAPSTONE_ARM64_REG_H26,
            CAPSTONE_ARM64_REG_H27,
            CAPSTONE_ARM64_REG_H28,
            CAPSTONE_ARM64_REG_H29,
            CAPSTONE_ARM64_REG_H30,
            CAPSTONE_ARM64_REG_H31,
            CAPSTONE_ARM64_REG_Q0,
            CAPSTONE_ARM64_REG_Q1,
            CAPSTONE_ARM64_REG_Q2,
            CAPSTONE_ARM64_REG_Q3,
            CAPSTONE_ARM64_REG_Q4,
            CAPSTONE_ARM64_REG_Q5,
            CAPSTONE_ARM64_REG_Q6,
            CAPSTONE_ARM64_REG_Q7,
            CAPSTONE_ARM64_REG_Q8,
            CAPSTONE_ARM64_REG_Q9,
            CAPSTONE_ARM64_REG_Q10,
            CAPSTONE_ARM64_REG_Q11,
            CAPSTONE_ARM64_REG_Q12,
            CAPSTONE_ARM64_REG_Q13,
            CAPSTONE_ARM64_REG_Q14,
            CAPSTONE_ARM64_REG_Q15,
            CAPSTONE_ARM64_REG_Q16,
            CAPSTONE_ARM64_REG_Q17,
            CAPSTONE_ARM64_REG_Q18,
            CAPSTONE_ARM64_REG_Q19,
            CAPSTONE_ARM64_REG_Q20,
            CAPSTONE_ARM64_REG_Q21,
            CAPSTONE_ARM64_REG_Q22,
            CAPSTONE_ARM64_REG_Q23,
            CAPSTONE_ARM64_REG_Q24,
            CAPSTONE_ARM64_REG_Q25,
            CAPSTONE_ARM64_REG_Q26,
            CAPSTONE_ARM64_REG_Q27,
            CAPSTONE_ARM64_REG_Q28,
            CAPSTONE_ARM64_REG_Q29,
            CAPSTONE_ARM64_REG_Q30,
            CAPSTONE_ARM64_REG_Q31,
            CAPSTONE_ARM64_REG_S0,
            CAPSTONE_ARM64_REG_S1,
            CAPSTONE_ARM64_REG_S2,
            CAPSTONE_ARM64_REG_S3,
            CAPSTONE_ARM64_REG_S4,
            CAPSTONE_ARM64_REG_S5,
            CAPSTONE_ARM64_REG_S6,
            CAPSTONE_ARM64_REG_S7,
            CAPSTONE_ARM64_REG_S8,
            CAPSTONE_ARM64_REG_S9,
            CAPSTONE_ARM64_REG_S10,
            CAPSTONE_ARM64_REG_S11,
            CAPSTONE_ARM64_REG_S12,
            CAPSTONE_ARM64_REG_S13,
            CAPSTONE_ARM64_REG_S14,
            CAPSTONE_ARM64_REG_S15,
            CAPSTONE_ARM64_REG_S16,
            CAPSTONE_ARM64_REG_S17,
            CAPSTONE_ARM64_REG_S18,
            CAPSTONE_ARM64_REG_S19,
            CAPSTONE_ARM64_REG_S20,
            CAPSTONE_ARM64_REG_S21,
            CAPSTONE_ARM64_REG_S22,
            CAPSTONE_ARM64_REG_S23,
            CAPSTONE_ARM64_REG_S24,
            CAPSTONE_ARM64_REG_S25,
            CAPSTONE_ARM64_REG_S26,
            CAPSTONE_ARM64_REG_S27,
            CAPSTONE_ARM64_REG_S28,
            CAPSTONE_ARM64_REG_S29,
            CAPSTONE_ARM64_REG_S30,
            CAPSTONE_ARM64_REG_S31,
            CAPSTONE_ARM64_REG_W0,
            CAPSTONE_ARM64_REG_W1,
            CAPSTONE_ARM64_REG_W2,
            CAPSTONE_ARM64_REG_W3,
            CAPSTONE_ARM64_REG_W4,
            CAPSTONE_ARM64_REG_W5,
            CAPSTONE_ARM64_REG_W6,
            CAPSTONE_ARM64_REG_W7,
            CAPSTONE_ARM64_REG_W8,
            CAPSTONE_ARM64_REG_W9,
            CAPSTONE_ARM64_REG_W10,
            CAPSTONE_ARM64_REG_W11,
            CAPSTONE_ARM64_REG_W12,
            CAPSTONE_ARM64_REG_W13,
            CAPSTONE_ARM64_REG_W14,
            CAPSTONE_ARM64_REG_W15,
            CAPSTONE_ARM64_REG_W16,
            CAPSTONE_ARM64_REG_W17,
            CAPSTONE_ARM64_REG_W18,
            CAPSTONE_ARM64_REG_W19,
            CAPSTONE_ARM64_REG_W20,
            CAPSTONE_ARM64_REG_W21,
            CAPSTONE_ARM64_REG_W22,
            CAPSTONE_ARM64_REG_W23,
            CAPSTONE_ARM64_REG_W24,
            CAPSTONE_ARM64_REG_W25,
            CAPSTONE_ARM64_REG_W26,
            CAPSTONE_ARM64_REG_W27,
            CAPSTONE_ARM64_REG_W28,
            CAPSTONE_ARM64_REG_W29,
            CAPSTONE_ARM64_REG_W30,
            CAPSTONE_ARM64_REG_X0,
            CAPSTONE_ARM64_REG_X1,
            CAPSTONE_ARM64_REG_X2,
            CAPSTONE_ARM64_REG_X3,
            CAPSTONE_ARM64_REG_X4,
            CAPSTONE_ARM64_REG_X5,
            CAPSTONE_ARM64_REG_X6,
            CAPSTONE_ARM64_REG_X7,
            CAPSTONE_ARM64_REG_X8,
            CAPSTONE_ARM64_REG_X9,
            CAPSTONE_ARM64_REG_X10,
            CAPSTONE_ARM64_REG_X11,
            CAPSTONE_ARM64_REG_X12,
            CAPSTONE_ARM64_REG_X13,
            CAPSTONE_ARM64_REG_X14,
            CAPSTONE_ARM64_REG_X15,
            CAPSTONE_ARM64_REG_X16,
            CAPSTONE_ARM64_REG_X17,
            CAPSTONE_ARM64_REG_X18,
            CAPSTONE_ARM64_REG_X19,
            CAPSTONE_ARM64_REG_X20,
            CAPSTONE_ARM64_REG_X21,
            CAPSTONE_ARM64_REG_X22,
            CAPSTONE_ARM64_REG_X23,
            CAPSTONE_ARM64_REG_X24,
            CAPSTONE_ARM64_REG_X25,
            CAPSTONE_ARM64_REG_X26,
            CAPSTONE_ARM64_REG_X27,
            CAPSTONE_ARM64_REG_X28,

            CAPSTONE_ARM64_REG_V0,
            CAPSTONE_ARM64_REG_V1,
            CAPSTONE_ARM64_REG_V2,
            CAPSTONE_ARM64_REG_V3,
            CAPSTONE_ARM64_REG_V4,
            CAPSTONE_ARM64_REG_V5,
            CAPSTONE_ARM64_REG_V6,
            CAPSTONE_ARM64_REG_V7,
            CAPSTONE_ARM64_REG_V8,
            CAPSTONE_ARM64_REG_V9,
            CAPSTONE_ARM64_REG_V10,
            CAPSTONE_ARM64_REG_V11,
            CAPSTONE_ARM64_REG_V12,
            CAPSTONE_ARM64_REG_V13,
            CAPSTONE_ARM64_REG_V14,
            CAPSTONE_ARM64_REG_V15,
            CAPSTONE_ARM64_REG_V16,
            CAPSTONE_ARM64_REG_V17,
            CAPSTONE_ARM64_REG_V18,
            CAPSTONE_ARM64_REG_V19,
            CAPSTONE_ARM64_REG_V20,
            CAPSTONE_ARM64_REG_V21,
            CAPSTONE_ARM64_REG_V22,
            CAPSTONE_ARM64_REG_V23,
            CAPSTONE_ARM64_REG_V24,
            CAPSTONE_ARM64_REG_V25,
            CAPSTONE_ARM64_REG_V26,
            CAPSTONE_ARM64_REG_V27,
            CAPSTONE_ARM64_REG_V28,
            CAPSTONE_ARM64_REG_V29,
            CAPSTONE_ARM64_REG_V30,
            CAPSTONE_ARM64_REG_V31,

            CAPSTONE_ARM64_REG_ENDING,		// <-- mark the end of the list of registers

                                            //> alias registers

                                            CAPSTONE_ARM64_REG_IP1 = CAPSTONE_ARM64_REG_X16,
                                            CAPSTONE_ARM64_REG_IP0 = CAPSTONE_ARM64_REG_X17,
                                            CAPSTONE_ARM64_REG_FP = CAPSTONE_ARM64_REG_X29,
                                            CAPSTONE_ARM64_REG_LR = CAPSTONE_ARM64_REG_X30,
        } capstone_arm64_reg;

        arm64_reg TranslateArm64Reg(capstone_arm64_reg reg);

        uint64_t m_pc;
        char m_Name[40];
        char m_Param[200];
        bool m_WriteBack;
        instruct_t m_Opc;
        uint32_t m_OperandCount;
        MCOperand m_Operands[8];
        bool m_UpdateFlags;
        arm64_cc m_cc;
    };

public:
    Arm64Opcode(Arm64OpcodeCache &cache, uint64_t pc, uint32_t insn);

    inline uint64_t PC(void) const { return m_Details.PC(); }
    inline const char * Name ( void ) const { return m_Details.Name(); }
    inline const char * Param ( void ) const { return m_Details.Param(); }
    inline instruct_t Opc(void) const { return m_Details.Opc(); }
    inline size_t Operands(void) const { return m_Details.Operands(); }
    inline const MCOperand & Operand(uint32_t index) const { return m_Details.Operand(index); }
    inline arm64_cc cc(void) const { return m_Details.cc(); }
    inline bool UpdateFlags(void) const { return m_Details.UpdateFlags(); }
    inline bool WriteBack(void) const { return m_Details.WriteBack(); }

    bool IsJump(void) const;
    bool IsBranch(void) const;
    uint64_t BranchDest(void) const;

    static bool Is128bitReg(arm64_reg reg);
    static bool Is64bitReg(arm64_reg reg);
    static bool Is64bitFloatReg(arm64_reg reg);
    static bool Is32bitReg(arm64_reg reg);
    static bool Is32bitFloatReg(arm64_reg reg);
    static bool IsVectorReg(arm64_reg reg);
    static bool Is16bitSimdReg(arm64_reg reg);

private:
    const Arm64OpcodeDetail & m_Details;
};

class Arm64OpcodeCache
{
    struct OpcodeKey
    {
        uint64_t pc;
        uint32_t insn;

        bool operator==(const OpcodeKey &o) const
        {
            return pc == o.pc && insn == o.insn;
        }

        bool operator<(const OpcodeKey &o) const
        {
            return pc < o.pc || (pc == o.pc && insn < o.insn);
        }
    };

    typedef std::map<OpcodeKey, Arm64Opcode::Arm64OpcodeDetail> OPCODE_CACHE;

public:
    Arm64OpcodeCache();
    ~Arm64OpcodeCache();

    const Arm64Opcode::Arm64OpcodeDetail & GetOpcodeDetail(uint64_t pc, uint32_t insn);

private:
    Arm64OpcodeCache(const Arm64OpcodeCache&);
    Arm64OpcodeCache& operator=(const Arm64OpcodeCache&);

    CriticalSection m_CacheCS;
    OPCODE_CACHE m_OpcodeCache;
};
