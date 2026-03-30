#include <fstream>
#include <iostream>
#include <string>

constexpr int DIAL_SIZE = 100;
constexpr int INITIAL_DIAL = 50;

int main()
{
    std::ifstream input_file("input");
    if (!input_file)
    {
        std::cerr << "Failed to open input\n";
        return 1;
    }

    int dial = INITIAL_DIAL;
    int count = 0;

    int line_num = 1;
    std::string line;
    while (std::getline(input_file, line))
    {
        if (line.size() < 2 || (line[0] != 'R' && line[0] != 'L'))
        {
            std::cerr << "Invalid input at line " << line_num << ": " << line << '\n';
            return 1;
        }

        char dir = line[0];
        int dist = std::stoi(line.substr(1));

        if (dir == 'R')
        {
            // Right rotation stays in the non-negative range
            dial = (dial + dist) % DIAL_SIZE;
        }
        else 
        {
            // Left rotation requires careful handling of negative modulo 
            dial = ((dial - dist) % DIAL_SIZE + DIAL_SIZE) % DIAL_SIZE;
        }

        if (dial == 0)
        {
            ++count;
        }

        ++line_num;
    }

    std::cout << count << '\n';

    return 0;
}