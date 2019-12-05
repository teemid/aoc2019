#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

void read_input(const char * filename, std::vector<int32_t> * values)
{
    std::ifstream fs;
    fs.open(filename, std::ifstream::in);

    std::string s;
    for (std::string s; std::getline(fs, s, ','); )
    {
        auto i = std::stoul(s);

        values->push_back(i);
    }
}

void part1(std::vector<int32_t> * values)
{
    (*values)[1] = 12;
    (*values)[2] = 2;
}

int32_t process(std::vector<int32_t> values, int start = 0)
{
    for (int i = start; ; i += 4)
    {
        auto op = values[i];
        switch (op)
        {
            case 1:
            {
                auto op1 = values[i + 1];
                auto op2 = values[i + 2];
                auto result = values[i + 3];

                values[result] = values[op1] + values[op2];
            } break;
            case 2:
            {
                auto op1 = values[i + 1];
                auto op2 = values[i + 2];
                auto result = values[i + 3];

                values[result] = values[op1] * values[op2];
            } break;
            case 99:
            {
                return values[0];
            } break;
            default:
            {
                return 0;
            } break;
        }
    }
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "Expects input file argument" << std::endl;

        return 1;
    }

    auto filename = argv[1];

    std::vector<int32_t> values;
    read_input(filename, &values);

    for (auto noun = 0; noun < 99; ++noun)
    {
        for (auto verb = 0; verb < 99; ++verb)
        {
            auto copy = values;
            copy[1] = noun;
            copy[2] = verb;

            auto result = process(copy);

            if (result == 19690720)
            {
                std::cout << "noun " << noun << std::endl;
                std::cout << "verb " << verb << std::endl;
                std::cout << (100 * noun) + verb << std::endl;
            }
        }
    }

    return 0;
}
