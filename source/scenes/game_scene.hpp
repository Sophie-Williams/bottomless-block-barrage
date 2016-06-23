#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scene.hpp"
#include <util/command_window.hpp>
#include <util/text.hpp>
#include <util/texture.hpp>
#include <string>
#include "panel_table.hpp"
#include "animation_params.hpp"
#include "moves_recorder.hpp"
#include "marker_manager.hpp"

enum Difficulty
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
};

class GameScene : public Scene
{
public:
    struct Config
    {
        // Either replay_filename is set or difficulty/level
        Difficulty difficulty;
        int level;
        int panel_gfx;

        std::string replay_filename;
    };

    GameScene(const Config& c) : config(c), level(c.level) {}
    virtual ~GameScene() {}

    void initialize();
    void update();

protected:
    void draw_top();
    void draw_bottom();

    virtual void init_recorder();
    virtual void init_panel_table();
    virtual void init_sprites();
    virtual void init_menu();

    virtual bool is_gameover() const;
    virtual void update_input();
    virtual void update_windows() {}
    virtual void update_gameover();

    virtual void update_match();
    virtual void update_end_match() {}
    virtual void update_on_timeout();

    void update_score();
    void update_level();
    virtual void update_on_matched() {}

    virtual void draw_game_top() {}
    virtual void draw_game_bottom();
    virtual void draw_gameover_top();
    virtual void draw_gameover_bottom();

    void draw_selector();
    void draw_board();
    void draw_panels();

    // Configuration
    Config config;

    // Images
    Texture panels;
    Texture selector;
    Texture border;
    Texture debug;

    // Core game stuff
    PanelTable panel_table;
    MatchInfo last_match;
    MatchInfo current_match;
    int selector_x = 2;
    int selector_y = 6;
    int score = 0;
    int level = 0;
    int experience = 0;
    int current_timeout = 0;

    // Animation
    AnimationParams frames;
    MarkerManager markers;

    // Debugging
    MovesRecorder recorder;

    // Game over stuff
    Text game_over;
    Text try_again;
    CommandWindow save_replay_command;
    CommandWindow try_again_command;

    // Housekeeping
    // Frame id
    int frame = 0;
    // Frame in milliseconds
    u64 last_frame = 0;

};


#endif

