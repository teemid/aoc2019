#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <strstream>
#include <cmath>
#include <algorithm>

struct Vector
{
    int32_t x;
    int32_t y;
};

struct Point
{
    int32_t x;
    int32_t y;
};

struct Line
{
    Point a;
    Point b;
};

Vector make_vector(const Point & a, const Point & b)
{
    Vector v;
    v.x = b.x - a.x;
    v.y = b.y - a.y;

    return v;
}

Vector make_vector(const Line & l)
{
    return make_vector(l.a, l.b);
}

int32_t dot_product(const Vector & v1, const Vector & v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}

void read_input(const char * filename, std::vector<std::string> * v)
{
    std::ifstream fs;
    fs.open(filename, std::ifstream::in);

    for (std::string s; std::getline(fs, s); )
    {
        v->push_back(s);
    }
}

std::vector<Line> make_line_segments(std::string & s)
{
    std::strstream ss;
    ss << s;

    std::vector<Line> lines;
    Point last{ 0, 0 };
    for (std::string s; std::getline(ss, s, ','); )
    {
        auto c = s.at(0);

        auto n = s.substr(1, s.size());
        int i = std::stoi(n);

        Point current = last;

        switch (c)
        {
            case 'R': { current.x += i; } break;
            case 'L': { current.x -= i; } break;
            case 'U': { current.y += i; } break;
            case 'D': { current.y -= i; } break;
        }

        lines.push_back(Line{last, current});

        last = current;
    }

    return lines;
}

int32_t manhattan_distance(const Point & a, const Point & b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool intersects(const Line & l1, const Line & l2)
{
    Vector v1 = make_vector(l1);
    Vector v2 = make_vector(l2);

    // NOTE (Emil): Lines are parallel
    if (dot_product(v1, v2) != 0)
    {
        return false;
    }

    // NOTE (Emil): Line is horizontal
    if (l1.a.x == l1.b.x)
    {
        auto minY = std::min(l1.a.y, l1.b.y);
        auto maxY = std::max(l1.a.y, l1.b.y);

        auto minX = std::min(l2.a.x, l2.b.x);
        auto maxX = std::max(l2.a.x, l2.b.x);

        return (minY < l2.a.y) && (l2.a.y < maxY) && (minX < l1.a.x) && (l1.a.x < maxX);
    }
    // NOTE (Emil): Line is vertical
    else if (l1.a.y == l1.b.y)
    {
        auto minX = std::min(l1.a.x, l1.b.x);
        auto maxX = std::max(l1.a.x, l1.b.x);

        auto minY = std::min(l2.a.y, l2.b.y);
        auto maxY = std::max(l2.a.y, l2.b.y);

        return (minX < l2.a.x) && (l2.a.x < maxX) && (minY < l1.a.y) && (l1.a.y < maxY);
    }

    return false;
}

Point find_intersection(const Line & l1, const Line & l2)
{
    Point p;

    if (l1.a.x == l1.b.x)
    {
        p.x = l1.a.x;
        p.y = l2.a.y;
    }
    else
    {
        p.x = l2.a.x;
        p.y = l1.a.y;
    }

    return p;
}

std::string to_string(const Point & p)
{
    std::string s("Point{");
    s.append(std::to_string(p.x));
    s.append(", ");
    s.append(std::to_string(p.y));
    s.append("}");

    return s;
}

std::string to_string(const Line & l)
{
    std::string s("Line{");
    s.append(to_string(l.a));
    s.append(", ");
    s.append(to_string(l.b));
    s.append("}");

    return s;
}

std::vector<Point> find_intersections(const std::vector<Line> & set1, const std::vector<Line> & set2)
{
    std::vector<Point> intersections;

    for (auto & l1 : set1)
    {
        for (auto & l2 : set2)
        {
            if (intersects(l1, l2))
            {
                auto p = find_intersection(l1, l2);

                intersections.push_back(p);
            }
        }
    }

    return intersections;
}

struct IntersectData
{
    Point p;
    int32_t steps;
};

std::vector<IntersectData> find_intersections2(const std::vector<Line> & set1, const std::vector<Line> & set2)
{
    std::vector<IntersectData> data;

    int32_t steps1 = 0;
    int32_t steps2 = 0;
    for (auto i = 0; i < set1.size(); ++i)
    {
        auto & l1 = set1[i];

        steps2 = 0;
        for (auto j = 0; j < set2.size(); ++j)
        {
            auto & l2 = set2[j];

            if (intersects(l1, l2))
            {
                auto p = find_intersection(l1, l2);

                auto e1 = manhattan_distance(l1.a, p);
                auto e2 = manhattan_distance(l2.a, p);

                data.push_back(IntersectData{ p, steps1 + steps2 + e1 + e2 });
            }

            steps2 += manhattan_distance(l2.a, l2.b);
        }

        steps1 += manhattan_distance(l1.a, l1.b);
    }

    return data;
}

void print_smallest_distance(const std::vector<Point> & intersections)
{
    std::vector<int32_t> distances;

    for (auto intersection : intersections)
    {
        distances.push_back(manhattan_distance(Point{}, intersection));
    }

    auto result = std::min_element(distances.begin(), distances.end());

    if (result == distances.end())
    {
        std::cout << "No intersections";
    }

    std::cout << *result << std::endl;
}

bool is_on_line(const Point & p, const Line & l)
{
    auto minX = std::min(l.a.x, l.b.x);
    auto maxX = std::max(l.a.x, l.b.x);

    auto minY = std::min(l.a.y, l.b.y);
    auto maxY = std::max(l.a.y, l.b.y);

    return (minX < p.x) && (p.x << maxX) && (minY < p.y) && (p.y << maxY);
}

void print_shortest_path_intersection(const std::vector<IntersectData> & data)
{
    int32_t min = std::numeric_limits<int32_t>::max();

    for (auto datum : data)
    {
        if (datum.steps < min)
        {
            min = datum.steps;
        }
    }

    std::cout << min << std::endl;
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "Expects input file argument" << std::endl;

        return 1;
    }

    auto filename = argv[1];

    std::vector<std::string> values;
    read_input(filename, &values);

    std::vector<std::vector<Line>> sets;
    for (auto str : values)
    {
        auto lines = make_line_segments(str);
        sets.push_back(lines);
    }

    // auto intersections = find_intersections(sets[0], sets[1]);
    // print_smallest_distance(intersections);
    auto intersections = find_intersections2(sets[0], sets[1]);
    print_shortest_path_intersection(intersections);

    return 0;
}
