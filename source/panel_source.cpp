#include "panel_source.hpp"
#include <cstdlib>

inline int randomInt(int max)
{
    return rand() / (RAND_MAX / max + 1);
}

inline int randomInt(int start, int end)
{
    return randomInt(end - start + 1) + start;
}

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
/// TODO make this better.
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

RandomPanelSource::RandomPanelSource(int rows, int columns, int _colors) : PanelSource(rows, columns), colors(_colors)
{

}

std::vector<int> RandomPanelSource::board_layout()
{
    return generate_values(columns, columns * 5, rows - 5);
}

Panel::Type RandomPanelSource::panel()
{
    return (Panel::Type) randomInt(1, colors);
}
