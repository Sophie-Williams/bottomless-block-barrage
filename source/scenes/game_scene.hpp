#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scene.hpp"
#include <util/background.hpp>
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

enum TimingMode
{
    TIME = 0,
    LINES = 1,
    MOVES = 2,
};

class GameScene : public Scene
{
public:
    struct GameConfig
    {
        // Score Mode Only
        TimingMode time_mode = TIME;
        int value = 120;

        // Puzzle Mode Only
        std::string puzzle_filename;
        std::string level_name;
        // For getting next puzzle.
        std::string set_name;
        int stage_id;
        int level_id;

        // Replay Mode
        std::string replay_filename;

        // All modes
        Difficulty difficulty = EASY;
        int level = 1;
        std::string panel_gfx = "";
    };

    GameScene(const GameConfig& c) : config(c), level(c.level) {}
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
    virtual void update_on_gameover() {}
    virtual void update_gameover();

    virtual void update_match();
    virtual void update_end_match() {}
    virtual void update_on_timeout();

    void update_create_markers();
    void update_score();
    void update_level();
    virtual void update_on_matched() {}

    virtual void draw_game_top() {}
    virtual void draw_game_bottom();
    virtual void draw_gameover_top() {}
    virtual void draw_gameover_bottom();

    void draw_selector();
    void draw_board();
    void draw_panels();

    // Configuration
    GameConfig config;

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

    // Nice things
    Background background_top;
    Background background_bottom;

    // Housekeeping
    // Frame id
    int frame = 0;
    // Frame in milliseconds
    u64 last_frame = 0;

};

#endif
