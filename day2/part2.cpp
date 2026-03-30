#include <array>
#include <string>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>

// Powers of 10 used to shift a d-digit pattern when building repeated IDs.
constexpr std::array<uint64_t, 20> POW10{
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

void accumulateWrongId(const std::string& start,
                       const std::string& end,
                       uint64_t& accumulator)
{
    const uint64_t L = std::stoull(start);
    const uint64_t R = std::stoull(end);

    const uint64_t len_start = start.size();
    const uint64_t len_end = end.size();

    // The same invalid ID can be generated from different repeated blocks:
    // for instance 1111 = 1 repeated 4 times = 11 repeated 2 times.
    // We use the set to check if the id was not already inserted.
    std::unordered_set<uint64_t> ids;

    // Generate invalid IDs by length. For a total length len, any repeated
    // pattern must have a block length d that divides len.
    for (uint64_t len = len_start; len <= len_end; ++len)
    {
        for (uint64_t d = 1; d <= len / 2; ++d)
        {
            if (len % d != 0)
            {
                continue;
            }

            // Repeating a d-digit block num exactly len / d times gives:
            // wrong_id = block * multiplier.
            // As an example, with len = 6 and d = 2, we have 121212 = 12 * 10101
            // The multiplier is 10^(d*(k-1)) + ... + 10^d + 1, where k = len / d.
            uint64_t multiplier = 0;
            for (uint64_t i = 0; i < len / d; ++i)
            {
                multiplier = multiplier * POW10[d] + 1;
            }

            // We restrict the candidate block range so that num * multiplier stays
            // inside [L, R]. This avoids scanning every possible d-digit block.
            uint64_t block_low = (L + multiplier - 1) / multiplier;
            uint64_t block_high = R / multiplier;

            // We keep only blocks with exactly d digits.
            block_low = std::max(block_low, POW10[d - 1]);
            block_high = std::min(block_high, POW10[d] - 1);

            if (block_low > block_high)
            {
                continue;
            }

            for (uint64_t block = block_low; block <= block_high; ++block)
            {
                const uint64_t wrong_id = block * multiplier;

                // Only accumulate if the ID was not seen before
                if (!ids.count(wrong_id))
                {
                    ids.insert(wrong_id);
                    accumulator += wrong_id;
                }
            }
        }
    }
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

    std::string token;
    while (std::getline(input_file, token, ','))
    {
        const size_t dash = token.find('-');
        if (dash == std::string::npos)
        {
            continue;
        }

        const std::string lo = token.substr(0, dash);
        const std::string hi = token.substr(dash + 1);
        accumulateWrongId(lo, hi, result);
    }

    std::cout << result << "\n";

    return 0;
}