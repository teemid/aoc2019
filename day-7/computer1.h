#ifndef INTCODE_COMPUTER_H
#define INTCODE_COMPUTER_H

#include <cstdint>
#include <vector>

#include "instruction.h"

class Computer
{
public:
    enum OpCode
    {
        Add = 1,
        Multiply = 2,
        Input = 3,
        Output = 4,
        JumpIfTrue = 5,
        JumpIfFalse = 6,
        LessThan = 7,
        Equals = 8,

        Halt = 99,
    };

    Computer(std::vector<int32_t> instructions, const std::vector<int32_t> & input)
        : m_memory(instructions)
        , m_input(input)
    {}

    ~Computer() = default;

    Instruction decode_instruction(int32_t code)
    {
        Instruction instruction;
        instruction.opcode = static_cast<OpCode>(code % 100);
        instruction.param1Mode = static_cast<ParameterMode>((code / 100) % 10);
        instruction.param2Mode = static_cast<ParameterMode>((code / 1000) % 10);
        instruction.param3Mode = static_cast<ParameterMode>((code / 10000) & 10);

        return instruction;
    }

    int32_t evaluate_parameter(int32_t value, ParameterMode mode)
    {
        return (mode == ParameterMode::PositionMode) ? m_memory[value] : value;
    }

    int take_input()
    {
        // int value = 0;
        // std::cout << "Enter input:";
        // std::cin >> value;

        // return value;
        auto value = m_input.at(0);
        m_input.erase(m_input.begin());

        return value;
    }

    void output(int value)
    {
        m_output.push_back(value);
    }

    std::vector<int32_t> & get_output()
    {
        return m_output;
    }

    int32_t process(int start = 0)
    {
        for (int i = start; ; )
        {
            auto inst = decode_instruction(m_memory[i]);
            switch (inst.opcode)
            {
                case Add:
                {
                    auto op1 = evaluate_parameter(m_memory[i + 1], inst.param1Mode);
                    auto op2 = evaluate_parameter(m_memory[i + 2], inst.param2Mode);
                    auto result = m_memory[i + 3];

                    m_memory[result] = op1 + op2;

                    i += 4;
                } break;
                case Multiply:
                {
                    auto op1 = evaluate_parameter(m_memory[i + 1], inst.param1Mode);
                    auto op2 = evaluate_parameter(m_memory[i + 2], inst.param2Mode);
                    auto result = m_memory[i + 3];

                    m_memory[result] = op1 * op2;

                    i += 4;
                } break;
                case Input:
                {
                    // Opcode 3 takes a single integer as input and saves it to the position
                    // given by its only parameter. For example, the instruction 3,50 would
                    // take an input value and store it at address 50.
                    auto adr = m_memory[i + 1];

                    m_memory[adr] = take_input();

                    i += 2;
                } break;
                case Output:
                {
                    // Opcode 4 outputs the value of its only parameter. For example,
                    // the instruction 4,50 would output the value at address 50.
                    auto val = evaluate_parameter(m_memory[i + 1], inst.param1Mode);

                    output(val);

                    i += 2;
                } break;
                case JumpIfTrue:
                {
                    auto op1 = evaluate_parameter(m_memory[i + 1], inst.param1Mode);
                    auto op2 = evaluate_parameter(m_memory[i + 2], inst.param2Mode);

                    if (op1 != 0)
                    {
                        i = op2;
                    }
                    else
                    {
                        i += 3;
                    }
                } break;
                case JumpIfFalse:
                {
                    auto op1 = evaluate_parameter(m_memory[i + 1], inst.param1Mode);
                    auto op2 = evaluate_parameter(m_memory[i + 2], inst.param2Mode);

                    if (op1 == 0)
                    {
                        i = op2;
                    }
                    else
                    {
                        i += 3;
                    }
                } break;
                case LessThan:
                {
                    auto op1 = evaluate_parameter(m_memory[i + 1], inst.param1Mode);
                    auto op2 = evaluate_parameter(m_memory[i + 2], inst.param2Mode);
                    auto result = m_memory[i + 3];

                    m_memory[result] = (op1 < op2) ? 1 : 0;

                    i += 4;
                } break;
                case Equals:
                {
                    auto op1 = evaluate_parameter(m_memory[i + 1], inst.param1Mode);
                    auto op2 = evaluate_parameter(m_memory[i + 2], inst.param2Mode);
                    auto result = m_memory[i + 3];

                    m_memory[result] = (op1 == op2) ? 1 : 0;

                    i += 4;
                } break;
                case Halt:
                {
                    return m_memory[0];
                } break;
                default:
                {
                    return 0;
                } break;
            }
        }
    }

private:
    std::vector<int32_t> m_memory;
    std::vector<int32_t> m_input;
    std::vector<int32_t> m_output;
};

#endif
