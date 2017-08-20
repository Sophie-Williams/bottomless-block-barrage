#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include <memory>
#include <string>


#include <util/background.hpp>
#include <util/batch.hpp>
#include <util/command_window.hpp>
#include <util/text.hpp>
#include <util/texture.hpp>

#include "scene.hpp"
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
        int rows = 11;
        int columns = 6;
        PanelTable::Type type = PanelTable::Type::ENDLESS;
        Difficulty difficulty = EASY;
        int level = 1;
        std::string panel_gfx = "";
    };

    GameScene(const GameConfig& c) : config(c), batch(200), level(c.level) {}
    virtual ~GameScene() {}

    void initialize();
    void update();
    std::string music() {return scene_music;}
protected:
    void draw_top();
    void draw_bottom();

    virtual void init_recorder();
    virtual void init_panel_table();
    virtual void init_sprites();
    virtual void init_menu();

    virtual bool is_gameover() const;

    virtual void update_windows() {}
    virtual void update_on_gameover() {}
    virtual void update_gameover();

    virtual void update_input();
    void update_read_input();
    virtual void update_move();
    virtual void update_on_move(int x, int y);
    virtual void update_selector();
    virtual void update_on_swap();
    virtual void update_quick_rise();
    virtual void update_on_quick_rise();
    virtual void update_exit();

    virtual void update_match();
    virtual void update_on_timeout();

    void update_create_markers();
    void update_score();
    void update_level();
    virtual void update_on_level() {}
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

    // Batch
    Batch batch;

    // Images
    Texture panels;
    Texture selector;
    Texture border;
    Texture debug;

    // Core game stuff
    std::unique_ptr<PanelTable> table;
    MatchInfo current_match;
    int selector_x = 2;
    int selector_y = 6;
    int score = 0;
    int level = 0;
    int next = 0;
    std::string scene_music;

    // Animation
    AnimationParams frames;
    MarkerManager markers;
    int danger_panel;

    // Debugging
    MovesRecorder recorder;

    // Nice things
    Background background_top;
    Background background_bottom;

    // Input
    u32 held;
    u32 trigger;

    int frame = 0;
};

#endif
