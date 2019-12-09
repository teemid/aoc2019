#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <stack>

struct Node
{
    std::string name;
    Node * parent;
    bool visited;
    std::vector<std::unique_ptr<Node>> children;

    Node(const std::string & name_)
        : name(name_)
        , parent(nullptr)
        , visited(false)
    { }
    ~Node() { }

    Node(const Node & rhs) = default;
    Node(Node && rhs) = default;

    Node & operator=(const Node & rhs) = default;
    Node & operator=(Node && rhs) = default;
};

struct OrbitPair
{
    std::string first;
    std::string second;
};

OrbitPair make_orbit_pair(const std::string & str)
{
    auto pos = str.find_first_of(')');

    OrbitPair p =
    {
        str.substr(0, pos),
        str.substr(pos + 1, str.size())
    };

    return p;
}

std::string to_string(const OrbitPair & p)
{
    return std::string("OrbitPair{" + p.first + ", " + p.second + "}");
}

// void sort_orbits(std::vector<OrbitPair> & orbits)
// {
//     std::string min = "COM";
//     for (auto i = 0; i < orbits.size(); ++i)
//     {
//         for (auto j = i; orbits.size(); ++j)
//         {
//             auto & orbit = orbits[j];
//             if (orbit.first == min)
//             {
//                 std::swap(orbits[i], orbits[j]);
//             }
//         }

//         min =
//     }
// }

std::unique_ptr<Node> make_orbit_tree(std::vector<OrbitPair> & orbits)
{
    auto root = std::make_unique<Node>("COM");

    std::stack<Node *> s;

    auto current = root.get();
    while (orbits.size() > 0)
    {
        auto it = std::find_if(orbits.begin(), orbits.end(), [current](OrbitPair p)
        {
            return p.first == current->name;
        });

        if (it == orbits.end())
        {
            // NOTE (Emil): No more children for the current node.
            current = s.top();
            s.pop();

            continue;
        }

        auto node = std::make_unique<Node>(it->second);
        node->parent = current;
        current->children.push_back(std::move(node));

        s.push(current->children.back().get());

        orbits.erase(it);
    }

    return root;
}

Node * find_node(Node * root, const std::string & name)
{
    std::stack<Node *> s;

    s.push(root);
    while (s.size() > 0)
    {
        auto curr = s.top();
        s.pop();

        if (curr->name == name)
        {
            return curr;
        }

        for (auto it = curr->children.begin(); it != curr->children.end(); it++)
        {
            auto child = it->get();
            s.push(child);
        }
    }

    return nullptr;
}

int count_orbits(Node * node)
{
    int count = 0;
    while (node->parent)
    {
        node = node->parent;
        count++;
    }

    return count;
}

int total_count(Node * root)
{
    int count = 0;

    std::stack<Node *> s;

    s.push(root);

    while (s.size() > 0)
    {
        auto curr = s.top();
        s.pop();

        count += count_orbits(curr);

        for (auto it = curr->children.begin(); it != curr->children.end(); it++)
        {
            s.push(it->get());
        }
    }

    return count;
}

int find_orbital_transfers(Node * root)
{
    auto you = find_node(root, "YOU");
    auto san = find_node(root, "SAN");

    std::vector<Node *> path1;
    std::vector<Node *> path2;

    auto curr = you->parent;
    while (curr->parent)
    {
        path1.push_back(curr);
        curr = curr->parent;
    }

    curr = san->parent;
    while (curr->parent)
    {
        path2.push_back(curr);
        curr = curr->parent;
    }

    auto last1 = path1.end() - 1;
    auto last2 = path2.end() - 1;

    int count = 0;
    while ((*last1)->name == (*last2)->name)
    {
        count++;

        last1--;
        last2--;
    }

    auto count1 = count_orbits(you->parent);
    auto count2 = count_orbits(san->parent);

    return (count1 + count2 - (count * 2));
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << "Expects an input file" << std::endl;

        return 1;
    }

    auto filename = argv[1];

    std::ifstream fs;
    fs.open(filename, std::ifstream::in);

    std::vector<OrbitPair> orbits;
    std::string s;
    for (std::string s; std::getline(fs, s); )
    {
        auto p = make_orbit_pair(s);
        orbits.push_back(p);
    }

    auto root = make_orbit_tree(orbits);
    std::cout << total_count(root.get()) << std::endl;

    auto transfers = find_orbital_transfers(root.get());
    std::cout << transfers << std::endl;

    return 0;
}
