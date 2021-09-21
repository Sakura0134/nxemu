#pragma once
#include "ShaderCharacteristics.h"
#include "ShaderInstruction.h"
#include <stdint.h>
#include <memory>

std::unique_ptr<ShaderCharacteristics> ShaderScanFlow(const ShaderProgramCode & ProgramCode, uint32_t StartAddress);