#include <iostream>
#include <string>
#include <vector>

// 1. Six digit number
bool is_six_digits(const std::string & password)
{
    if (password.size() != 6)
    {
        return false;
    }

    for (auto i = 0; i < password.size(); ++i)
    {
        auto c = password.at(i);
        if ('0' > c && c > '9')
        {
            return false;
        }
    }

    return true;
}

// 2. Value is within range (108457-562041)
bool is_within_range(const std::string & password)
{
    int pw = std::stoi(password);
    if ((pw < 108457) || (562041 < pw))
    {
        return false;
    }

    return true;
}

// 3. Two adjacent digits are the same (like 22 in 122345)
bool has_two_equal_adjacent_digits(const std::string & password)
{
    int same = 0;
    for (auto i = 0; i < password.size() - 1; ++i)
    {
        auto c = password.at(i);
        auto p = password.at(i + 1);

        if (c == p)
        {
            return true;
        }
    }

    return false;
}

// 3b. Minimum two adjacent of the same digit,
// but the double is not part of a larger group
bool has_two_adjacent_digits_of_no_larger_group(const std::string & password)
{
    std::vector<int> streaks;

    auto c = password.at(0);
    int streak = 1;
    for (auto i = 1; i < password.size(); ++i)
    {
        auto p = password.at(i);

        if (c == p)
        {
            streak += 1;
        }
        else
        {
            streaks.push_back(streak);
            streak = 1;
        }

        c = p;
        if (i == password.size() - 1)
        {
            streaks.push_back(streak);
        }
    }

    for (auto streak : streaks)
    {
        if (streak == 2)
        {
            return true;
        }
    }

    return false;
}


// 4. Going from left to right, the digits never decrease; they only ever
//    increase or stay the same (like 111123 or 135679)
bool is_not_decreasing(const std::string & password)
{
    char min = '0';
    for (auto i = 0; i < password.size(); ++i)
    {
        auto c = password.at(i);
        if (c < min)
        {
            return false;
        }

        min = c;
    }

    return true;
}

// Input: 108457-562041
bool validate_password(const std::string & password)
{
    return is_six_digits(password)
        && is_within_range(password)
        && has_two_adjacent_digits_of_no_larger_group(password)
        && is_not_decreasing(password);
}

int main(int argc, char * argv[])
{
    std::cout << "Total password count: " << (562041 - 108457) << std::endl;

    // OK
    validate_password("111111");
    // NOT OK decreasing
    validate_password("223450");
    // NOT OK no double
    validate_password("123789");

    // OK
    validate_password("112233");
    // NOT OK larger group than 2
    validate_password("123444");
    // OK meets the criteria (even though 1 is repeated more
    // than twice, it still contains a double 22)
    validate_password("111122");

    std::vector<std::string> passwords;
    for (auto i = 108457; i <= 562041; ++i)
    {
        passwords.push_back(std::to_string(i));
    }

    int count = 0;
    for (auto password : passwords)
    {
        if (validate_password(password))
        {
            // std::cout << password << std::endl;
            count += 1;
        }
    }

    std::cout << "password count: " << count << std::endl;

    return 0;
}
