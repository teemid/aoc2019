#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

#include "computer1.h"
#include "computer2.h"

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

std::string to_string(const std::vector<int> & a)
{
    return "["
        + std::to_string(a[0]) + ","
        + std::to_string(a[1]) + ","
        + std::to_string(a[2]) + ","
        + std::to_string(a[3]) + ","
        + std::to_string(a[4]) + "]";
}

class HeapsAlgorithm
{
public:
    HeapsAlgorithm(int n_, std::vector<int> a_) : n(n_), a(a_), i(0), complete(false)
    {
        c.reserve(n);

        for (int i = 0; i < n; ++i)
        {
            c[i] = 0;
        }
    }
    ~HeapsAlgorithm() = default;

    bool finished()
    {
        return complete;
    }

    bool next(std::vector<int> * output)
    {
        static bool first = true;

        if (first)
        {
            first = false;
            *output = a;

            return true;
        }

        while (i < n)
        {
            if (c[i] < i)
            {
                if (i % 2 == 0)
                {
                    std::swap(a[0], a[i]);
                }
                else
                {
                    std::swap(a[c[i]], a[i]);
                }

                *output = a;

                c[i] += 1;
                i = 0;

                return true;
            }
            else
            {
                c[i] = 0;
                i += 1;
            }
        }

        complete = true;

        return false;
    }

private:
    int n;
    int i;
    bool complete;
    std::vector<int> a;
    std::vector<int> c;
};

void part1(const std::vector<int> & program)
{
    std::vector<int32_t> phases = {0, 1, 2, 3, 4};

    auto permutator = HeapsAlgorithm(5, phases);

    std::vector<int> order;
    int max = std::numeric_limits<int>::min();
    while (permutator.next(&order))
    {
        int32_t phase = 0;
        int32_t input = 0;
        for (int i = 0; i < 5; ++i)
        {
            phase = order[i];

            Computer amp(program, {phase, input});
            amp.process();

            input = amp.get_output()[0];
        }

        if (input > max)
        {
            max = input;
        }
    }

    std::cout << max << std::endl;
}

void reset_amplifiers(std::vector<Computer2> & amplifiers, const std::vector<int> & program)
{
    amplifiers[0] = Computer2(program);
    amplifiers[1] = Computer2(program);
    amplifiers[2] = Computer2(program);
    amplifiers[3] = Computer2(program);
    amplifiers[4] = Computer2(program);
}

void part2(const std::vector<int> & program)
{
    std::vector<int32_t> phases = {5, 6, 7, 8, 9};

    auto permutator = HeapsAlgorithm(5, phases);
    auto amplifiers = std::vector<Computer2>
    {
        Computer2(program),
        Computer2(program),
        Computer2(program),
        Computer2(program),
        Computer2(program),
    };

    auto isHalted = [&amplifiers]()
    {
        return amplifiers[0].halted()
            && amplifiers[1].halted()
            && amplifiers[2].halted()
            && amplifiers[3].halted()
            && amplifiers[4].halted();
    };

    std::vector<int> order;
    std::vector<int> maxOrder;
    int max = std::numeric_limits<int>::min();
    while (permutator.next(&order))
    {
        reset_amplifiers(amplifiers, program);

        int32_t phase = 0;
        int32_t input = 0;
        for (int i = 0; ; ++i)
        {
            phase = order[i % 5];
            Computer2 & amp = amplifiers[i % 5];
            amp.setInput(amp.started() ? std::vector<int>{input} : std::vector<int>{phase, input});
            amp.process();

            auto out = amp.get_output();
            if (amp.halted() && out.size() == 0)
            {
                if (out.size() > 0)
                {
                    input = out[0];
                }

                if (input > max)
                {
                    max = input;
                    maxOrder = order;
                }

                break;
            }

            input = out[0];
        }
    }

    std::cout << to_string(maxOrder) << " " << max << std::endl;
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "Expects input file argument" << std::endl;

        return 1;
    }

    auto filename = argv[1];

    std::vector<int32_t> program;
    read_input(filename, &program);

    part2(program);

    return 0;
}
