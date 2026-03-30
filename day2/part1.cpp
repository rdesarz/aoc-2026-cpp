#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

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

void accumulateInvalidId(const std::string& start,
                         const std::string& end,
                         uint64_t& accumulator)
{
    const uint64_t L = std::stoull(start);
    const uint64_t R = std::stoull(end);

    const uint64_t half_len_min = start.size() / 2;
    const uint64_t half_len_max = end.size() / 2;

    // An invalid ID of length 2 * k is a k-digit number x repeated once:
    // invalid_id = x * (10^k + 1). Here, k is half_len. We iterate over all
    // values of half_len that are part of the interval [half_len_min, half_len_max]
    for (uint64_t half_len = half_len_min; half_len <= half_len_max; ++half_len)
    {
        // The smallest (2*half_len)-digit number is 10^(2*half_len-1),
        // the largest is 10^(2*half_len) - 1. We clamp to [L, R].
        const uint64_t id_min = std::max(L, POW10[2 * half_len - 1]);
        const uint64_t id_max = std::min(R, POW10[2 * half_len] - 1);

        if (id_min > id_max)
        {
            continue;
        }

        const uint64_t multiplier = POW10[half_len] + 1;

        // We need L <= x * multiplier <= R, so x is in [x_low, x_high].
        // We perform integer ceiling: x_low = ceil(id_min / multiplier).
        const uint64_t x_low = (id_min + multiplier - 1) / multiplier;
        const uint64_t x_high = id_max / multiplier;

        if (x_low > x_high)
        {
            continue;
        }

        // The arithmetic sum of x from x_low to x_high can be computed by:
        // sum(x) = x_high*(x_high+1)/2 - (x_low-1)*x_low/2
        // Each invalid ID is x * multiplier, so we multiply the sum.
        const uint64_t sum = (x_high * (x_high + 1) - (x_low - 1) * x_low) / 2;
        accumulator += sum * multiplier;
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
        accumulateInvalidId(lo, hi, result);
    }

    std::cout << result << "\n";

    return 0;
}