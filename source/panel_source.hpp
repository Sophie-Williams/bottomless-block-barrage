#ifndef PANEL_SOURCE_HPP
#define PANEL_SOURCE_HPP

#include "panel.hpp"
#include <vector>

class PanelSource
{
public:
    PanelSource(int rows, int columns);
    virtual ~PanelSource() {}
    /// Generate a board.  Default implementation calls board_layout and panel.
    virtual std::vector<Panel::Type> board();
    /// Build the initial board layout
    /// Returns a vector with columns values specifying the number of panels for that column
    virtual std::vector<int> board_layout() {return std::vector<int>();};
    /// Get a panel, used for generating the initial board
    virtual Panel::Type panel() = 0;
    /// Get a line of panels, used for generating next set of panels
    /// Default implementation calls panel "columns" times.
    virtual std::vector<Panel::Type> line();

protected:
    int rows;
    int columns;
};

class RandomPanelSource : public PanelSource
{
public:
    RandomPanelSource(int rows, int columns, int colors);
    ~RandomPanelSource() override {}
    std::vector<int> board_layout() override;
    Panel::Type panel() override;
private:
    int colors;
};

#endif
