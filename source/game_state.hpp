#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <3ds.h>
#include <memory>
#include "panel_table.hpp"
#include "util/texture.hpp"

struct AnimationParams
{
    AnimationParams() : panel(0), selector(0), panel_counter(0), selector_counter(0) {}
    void Reset() {panel = selector = panel_counter = selector_counter = 0;}
    void Update(const PanelTable::State& state, bool is_danger);
    unsigned int panel;
    unsigned int selector;
    unsigned int panel_counter;
    unsigned int selector_counter;
};

class GameState
{
public:
    struct Options
    {
        int rows;
        int columns;
        int colors;
    };
    GameState(const Options& opts) : options(opts), score(0) {Init();}
    void Init() {Init(options);}
    void Init(const Options& opts);
    void Update();
    void Render();
private:
    void UpdateScore(int combo_num, int chain_num);
    Options options;
    int score;
    int level;
    std::unique_ptr<Texture> panels;
    std::unique_ptr<Texture> selector;
    std::unique_ptr<Texture> border;
    std::unique_ptr<Texture> debug;
    std::unique_ptr<PanelTable> panel_table;
    int selector_x, selector_y;
    u64 last_frame;
    u64 last_rise;
    AnimationParams frames;
};

#endif
