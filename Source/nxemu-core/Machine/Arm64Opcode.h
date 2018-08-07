#pragma once

class Arm64Opcode
{
public:
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

        //Alias
        ARM64_REG_LR = ARM64_REG_X30,
    } arm64_reg;
};