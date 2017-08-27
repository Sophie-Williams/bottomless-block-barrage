#ifndef PUZZLE_PANEL_SOURCE_HPP
#define PUZZLE_PANEL_SOURCE_HPP

#include "basic_puzzle.hpp"
#include "panel.hpp"

class PuzzlePanelSource : public PanelSource
{
public:
    PuzzlePanelSource(const BasicPuzzle& puzzle) : PanelSource(12, 6), table(72)
    {
        for (unsigned int i = 0; i < table.size(); i++)
            table[i] = (Panel::Type) puzzle.panels[i];
    }
    ~PuzzlePanelSource() override {}
    std::vector<Panel::Type> board() override {return table;}
    Panel::Type panel() override {return Panel::Type::EMPTY;}
private:
    std::vector<Panel::Type> table;
};

#endif
