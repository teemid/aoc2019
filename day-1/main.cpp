#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

int calculateFuelRequirement(int mass)
{
    return (std::floor(mass / 3) - 2);
}

// NOTE (Emil): Correct answer: 3399394.
int part1(const std::string & filename)
{
    std::ifstream fs;
    fs.open(filename, std::ifstream::in);

    int fuelRequirement = 0;
    for (std::string s; std::getline(fs, s); )
    {
        int moduleWeight = std::stoi(s);

        fuelRequirement += calculateFuelRequirement(moduleWeight);
    }

    return fuelRequirement;
}

// NOTE (Emil): Correct answer: 5096223.
int part2(const std::string & filename)
{
    std::ifstream fs;
    fs.open(filename, std::ifstream::in);

    int fuelRequirement = 0;
    for (std::string s; std::getline(fs, s); )
    {
        int moduleWeight = std::stoi(s);

        int moduleFuel = calculateFuelRequirement(moduleWeight);

        for (int extraFuel = calculateFuelRequirement(moduleFuel); extraFuel > 0; extraFuel = calculateFuelRequirement(extraFuel))
        {
            if (extraFuel > 0)
            {
                moduleFuel += extraFuel;
                continue;
            }
        }

        fuelRequirement += moduleFuel;
    }

    return fuelRequirement;
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "Expects an input file" << std::endl;

        return 1;
    }

    auto filename = argv[1];

    int fuelRequirement = part2(filename);

    std::cout << fuelRequirement << std::endl;

    return 0;
}
