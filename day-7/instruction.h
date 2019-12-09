#ifndef INTCODE_INSTRUCTION_H
#define INTCODE_INSTRUCTION_H

#include <cstdint>

enum class ParameterMode : uint8_t
{
    PositionMode,
    ImmediateMode,
};

struct Instruction
{
    uint8_t opcode;
    ParameterMode param1Mode;
    ParameterMode param2Mode;
    ParameterMode param3Mode;
};

#endif
