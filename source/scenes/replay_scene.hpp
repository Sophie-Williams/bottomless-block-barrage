#ifndef REPLAY_SCENE_HPP
#define REPLAY_SCENE_HPP

#include "scene.hpp"
#include <string>
#include <windows/info_window.hpp>
#include <windows/ccc_window.hpp>
#include <util/texture.hpp>
#include "panel_table.hpp"
#include "animation_params.hpp"
#include "moves_recorder.hpp"

class ReplayScene : public Scene
{
public:
    struct Config
    {
        std::string replay_filename;
        Difficulty difficulty;
        int level;
    };
    ReplayScene(const Config& config);
    ~ReplayScene() {}
    void initialize();
    void update();

protected:
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();

    void draw_selector();
    void draw_board();
    void draw_panels();

    void update_game();
    void update_gameover();
private:
    Config config;
    InfoWindow info;
    CCCWindow ccc_stats;
    Texture panels;
    Texture selector;
    Texture border;
    Texture debug;
    PanelTable panel_table;
    AnimationParams frames;
    MatchInfo last_match;
    MovesRecorder recorder;

    int selector_x = 2;
    int selector_y = 6;

    int score = 0;
    int level = 0;
    // To get to next speed level
    int experience = 0;

    int frame = 0;
    u64 last_frame = 0;
    u64 last_rise = 0;
};

#endif
