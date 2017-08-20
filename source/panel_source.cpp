#include "panel_source.hpp"
#include <cstdlib>
#include <algorithm>

inline int randomInt(int max)
{
    return rand() / (RAND_MAX / max + 1);
}

inline int randomInt(int start, int end)
{
    return randomInt(end - start + 1) + start;
}

#define BOARD_CONFIGURATION_SIZE 46
// Array of combinations of 6 values summing to 30.
int board_configurations[BOARD_CONFIGURATION_SIZE][6] = {
    {0, 2, 7, 7, 7, 7},
    {0, 3, 6, 7, 7, 7},
    {0, 4, 5, 7, 7, 7},
    {0, 4, 6, 6, 7, 7},
    {0, 5, 5, 6, 7, 7},
    {0, 5, 6, 6, 6, 7},
    {0, 6, 6, 6, 6, 6},
    {1, 1, 7, 7, 7, 7},
    {1, 2, 6, 7, 7, 7},
    {1, 3, 5, 7, 7, 7},
    {1, 3, 6, 6, 7, 7},
    {1, 4, 4, 7, 7, 7},
    {1, 4, 5, 6, 7, 7},
    {1, 4, 6, 6, 6, 7},
    {1, 5, 5, 5, 7, 7},
    {1, 5, 5, 6, 6, 7},
    {1, 5, 6, 6, 6, 6},
    {2, 2, 5, 7, 7, 7},
    {2, 2, 6, 6, 7, 7},
    {2, 3, 4, 7, 7, 7},
    {2, 3, 5, 6, 7, 7},
    {2, 3, 6, 6, 6, 7},
    {2, 4, 4, 6, 7, 7},
    {2, 4, 5, 5, 7, 7},
    {2, 4, 5, 6, 6, 7},
    {2, 4, 6, 6, 6, 6},
    {2, 5, 5, 5, 6, 7},
    {2, 5, 5, 6, 6, 6},
    {3, 3, 3, 7, 7, 7},
    {3, 3, 4, 6, 7, 7},
    {3, 3, 5, 5, 7, 7},
    {3, 3, 5, 6, 6, 7},
    {3, 3, 6, 6, 6, 6},
    {3, 4, 4, 5, 7, 7},
    {3, 4, 4, 6, 6, 7},
    {3, 4, 5, 5, 6, 7},
    {3, 4, 5, 6, 6, 6},
    {3, 5, 5, 5, 5, 7},
    {3, 5, 5, 5, 6, 6},
    {4, 4, 4, 4, 7, 7},
    {4, 4, 4, 5, 6, 7},
    {4, 4, 4, 6, 6, 6},
    {4, 4, 5, 5, 5, 7},
    {4, 4, 5, 5, 6, 6},
    {4, 5, 5, 5, 5, 6},
    {5, 5, 5, 5, 5, 5},
};

PanelSource::PanelSource(int _rows, int _columns) : rows(_rows), columns(_columns)
{

}

std::vector<Panel::Type> PanelSource::board()
{
    std::vector<Panel::Type> board(rows * columns, Panel::Type::EMPTY);
    std::vector<int> values = board_layout();

    // Generate initial board.
    for (int j = 0; j < columns; j++)
    {
        int value = values[j];
        for (int i = 0; i < value; i++)
            board[(rows - 1 - i) * columns + j] = panel();
    }

    return board;
}

std::vector<Panel::Type> PanelSource::line()
{
    std::vector<Panel::Type> next(columns, Panel::Type::EMPTY);
    for (int i = 0; i < columns; i++)
        next[i] = panel();
    return next;
}


/// Generates n values which sum to total each value being < max
/// TODO Remove this function.
std::vector<int> generate_values(int n, int total, int max)
{
    std::vector<int> values(n, 0);
    while(1)
    {
		int sum = 0;
        for (int i = 0; i < n - 1; i++)
        {
            int value = randomInt(max);
            values[i] = value;
            sum += value;
        }
		int left = total - sum;
        if (left >= 0 && left <= max)
        {
            values[n - 1] = left;
            break;
        }
    }
    return values;
}

std::vector<int> generate_board_setup()
{
    int configuration = randomInt(BOARD_CONFIGURATION_SIZE);
    std::vector<int> ret(board_configurations[configuration], board_configurations[configuration] + 6);
    std::random_shuffle(ret.begin(), ret.end());
    return ret;

}

RandomPanelSource::RandomPanelSource(int rows, int columns, int _colors) : PanelSource(rows, columns), colors(_colors)
{

}

std::vector<int> RandomPanelSource::board_layout()
{
    return generate_board_setup();
}

Panel::Type RandomPanelSource::panel()
{
    return (Panel::Type) randomInt(1, colors);
}
