#ifndef PUZZLE_SCENE
#define PUZZLE_SCENE

#include "game_scene.hpp"
#include <string>
#include <deque>
#include <windows/time_window.hpp>
#include <windows/puzzle_status_window.hpp>
#include <util/background.hpp>

class PuzzleSnapshot
{
public:
    PuzzleSnapshot(const PanelTable& table);
    void restore(PanelTable& table) const;
private:
    int moves;
    std::vector<Panel::Type> panels;
};

class PuzzleScene : public GameScene
{
public:
    PuzzleScene(const GameConfig& config);
    ~PuzzleScene() {}

protected:
    void init_panel_table() override;
    void init_menu() override;

    void update_input() override;
    void update_windows() override;
    void update_gameover() override;
    void update_on_gameover() override;

    bool is_gameover() const override;

    void draw_bottom() override;
    void draw_gameover_top() override;
private:
    std::deque<PuzzleSnapshot> snapshots;
    TimeWindow time_window;
    PuzzleStatusWindow status_window;
    Text result_text;
};

#endif
