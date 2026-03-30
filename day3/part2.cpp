#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

constexpr std::size_t N_DIGITS = 12;

int main()
{
    std::ifstream input_file("input");
    if (!input_file)
    {
        std::cerr << "Failed to open input\n";
        return 1;
    }

    std::uint64_t result = 0;

    std::string line;
    while (std::getline(input_file, line))
    {
        if (line.size() < N_DIGITS)
        {
            std::cerr << "Invalid input line: expected at least " << N_DIGITS << " digits\n";
            return 1;
        }

        std::string best_joltage = line.substr(line.size() - N_DIGITS);

        // We scan the prefix from right to left and try to insert each digit into the
        // current 12-digit subsequence. A digit is moved left as long as it does not
        // make the subsequence worse lexicographically.
        for (auto digit = line.rbegin() + N_DIGITS; digit != line.rend(); ++digit)
        {
            char candidate = *digit;
            for (char& chosen_digit : best_joltage)
            {
                if (candidate < chosen_digit)
                {
                    break;
                }

                std::swap(candidate, chosen_digit);
            }
        }

        result += std::stoull(best_joltage);
    }

    std::cout << result << '\n';

    return 0;
}
