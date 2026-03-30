#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

uint64_t buildNumberFromChar(char tens, char units)
{
    return static_cast<uint64_t>(tens - '0') * 10 + (units - '0');
}

int main()
{
    std::ifstream input_file("input");
    if (!input_file)
    {
        std::cerr << "Failed to open input\n";
        return 1;
    }

    uint64_t result = 0;
    std::string line;

    while (std::getline(input_file, line))
    {
        if (line.size() < 2)
        {
            std::cerr << "Invalid input line: expected at least 2 characters\n";
            return 1;
        }

        char max_right = line.back();
        uint64_t best = 0;

        // We scan from right to left. For each digit as the tens digit, 
        // we pair it with the largest digit seen so far on its right.
        for (auto it = line.rbegin() + 1; it != line.rend(); ++it)
        {
            best = std::max(best, buildNumberFromChar(*it, max_right));
            if (*it > max_right)
            {
                max_right = *it;
            }
        }

        result += best;
    }

    std::cout << result << '\n';
    return 0;
}