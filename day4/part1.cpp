#include <fstream>
#include <iostream>
#include <string>
#include <vector>


bool isInside(int row, int col, int n_rows, int n_cols)
{
    return row >= 0 && col >= 0 && row < n_rows && col < n_cols;
}

int countAdjacentRolls(const std::vector<std::string>& grid, int row, int col)
{
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    int adjacent_rolls = 0;
    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0)
            {
                continue;
            }

            const int neighbor_row = row + dr;
            const int neighbor_col = col + dc;

            if (isInside(neighbor_row, neighbor_col, rows, cols) &&
                grid[neighbor_row][neighbor_col] == '@')
            {
                ++adjacent_rolls;
            }
        }
    }

    return adjacent_rolls;
}

int main()
{
    std::ifstream input_file("input");
    if (!input_file)
    {
        std::cerr << "Failed to open input\n";
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(input_file, line))
    {
        grid.push_back(line);
    }

    if (grid.empty())
    {
        std::cerr << "Empty input\n";
        return 1;
    }

    const int n_rows = static_cast<int>(grid.size());
    const int n_cols = static_cast<int>(grid[0].size());

    int result = 0;

    for (int row = 0; row < n_rows; ++row)
    {
        for (int col = 0; col < n_cols; ++col)
        {
            if (grid[row][col] != '@')
            {
                continue;
            }

            // A roll is accessible if fewer than four of its 8 neighbors are rolls.
            if (countAdjacentRolls(grid, row, col) < 4)
            {
                ++result;
            }
        }
    }

    std::cout << result << '\n';

    return 0;
}
