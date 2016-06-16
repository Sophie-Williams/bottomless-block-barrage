#ifndef ENDLESS_SCENE_HPP
#define ENDLESS_SCENE_HPP

#include "scene.hpp"
#include <windows/info_window.hpp>
#include <windows/ccc_window.hpp>
#include <util/command_window.hpp>
#include <util/text.hpp>
#include <util/texture.hpp>
#include "panel_table.hpp"
#include "animation_params.hpp"
#include "moves_recorder.hpp"

class EndlessScene : public Scene
{
public:
    struct Config
    {
        Difficulty difficulty;
        int level;
    };
    EndlessScene(const Config& config);
    ~EndlessScene() {}
    void initialize();
    void update();

protected:
    void draw_top();
    void draw_bottom();

    void draw_selector();
    void draw_board();
    void draw_panels();

    void update_game();
    void update_gameover();
private:
    Config config;

    // game
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
    // game over
    Text game_over;
    Text try_again;
    CommandWindow save_replay_command;
    CommandWindow try_again_command;

    int selector_x = 2;
    int selector_y = 6;

    int score = 0;
    int level = 0;
    // To get to next speed level
    int experience = 0;

    int frame = 0;
    u64 last_frame = 0;
};

#endif
