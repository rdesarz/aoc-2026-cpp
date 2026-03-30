#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

bool isInside(int row, int col, int rows, int cols)
{
    return row >= 0 && col >= 0 && row < rows && col < cols;
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

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    std::vector<std::vector<int>> degree(rows, std::vector<int>(cols, 0));
    std::vector<std::vector<bool>> queued(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> removable_rolls;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (grid[row][col] != '@')
            {
                continue;
            }

            degree[row][col] = countAdjacentRolls(grid, row, col);

            if (degree[row][col] < 4)
            {
                removable_rolls.push({row, col});
                queued[row][col] = true;
            }
        }
    }

    int result = 0;

    // A brute force approach would be to revisit all the grid until we cannot remove
    // a new roll. By maintaining a queue of rolls to remove we can simply visit the rolls that
    // should be removed which is more efficient. Every time a roll is deleted, we update the
    // local neighborhood and insert new potentially removable rolls in the queue. 
    while (!removable_rolls.empty())
    {
        const auto [row, col] = removable_rolls.front();
        removable_rolls.pop();

        if (grid[row][col] != '@')
        {
            continue;
        }

        grid[row][col] = '.';
        ++result;

        for (int dr = -1; dr <= 1; ++dr)
        {
            for (int dc = -1; dc <= 1; ++dc)
            {
                if (dr == 0 && dc == 0)
                {
                    continue;
                }

                const int neigh_row = row + dr;
                const int neigh_col = col + dc;

                if (!isInside(neigh_row, neigh_col, rows, cols) ||
                    grid[neigh_row][neigh_col] != '@')
                {
                    continue;
                }

                --degree[neigh_row][neigh_col];
                if (degree[neigh_row][neigh_col] < 4 &&
                    !queued[neigh_row][neigh_col])
                {
                    removable_rolls.push({neigh_row, neigh_col});
                    queued[neigh_row][neigh_col] = true;
                }
            }
        }
    }

    std::cout << result << '\n';

    return 0;
}
