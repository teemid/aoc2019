#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using Layer = std::string;

struct Image
{
    int width;
    int height;
    std::vector<Layer> layers;
};

Image read_input(const char * filename, int width, int height)
{
    Image image;
    image.width = width;
    image.height = height;

    std::ifstream fs;
    fs.open(filename, std::ifstream::in);

    std::string s;
    std::getline(fs, s);

    int size = width * height;
    for (int i = 0; i < s.size(); i += size)
    {
        image.layers.push_back(s.substr(i, size));
    }

    return image;
}

void part1(const Image & image)
{
    int min = std::numeric_limits<int>::max();
    auto selected = image.layers.end();
    for (auto layer = image.layers.begin(); layer != image.layers.end(); layer++)
    {
        auto count = std::count(layer->begin(), layer->end(), '0');

        if (count < min)
        {
            min = count;
            selected = layer;
        }
    }

    auto ones = std::count(selected->begin(), selected->end(), '1');
    auto twos = std::count(selected->begin(), selected->end(), '2');

    std::cout << ones * twos << std::endl;
}

void part2(const Image & image)
{
    std::vector<std::string> output(image.height);

    for (int y = 0; y < image.height; ++y)
    {
        output[y] = std::string(image.width, ' ');
    }

    for (auto it = image.layers.end() - 1; it >= image.layers.begin(); --it)
    {
        auto & layer = *it;
        for (int y = 0; y < image.height; ++y)
        {
            auto & scanline = output[y];
            auto line = layer.substr(y * image.width, image.width);
            for (int x = 0; x < image.width; ++x)
            {
                auto pixel = line[x];
                switch (pixel)
                {
                    case '0': { scanline[x] = ' '; } break;
                    case '1': { scanline[x] = 'X'; } break;
                    case '2': {                    } break;
                }
            }
        }
    }

    std::string img;
    for (auto line : output)
    {
        img += line + "\n";
    }

    std::cout << img;
}

int main(int argc, char * argv[])
{
    if (argc < 4)
    {
        std::cout << "Expects input file argument" << std::endl;

        return 1;
    }

    auto width = std::stoi(argv[1]);
    auto height = std::stoi(argv[2]);
    auto filename = argv[3];

    auto image = read_input(filename, width, height);
    part1(image);
    part2(image);

    return 0;
}
