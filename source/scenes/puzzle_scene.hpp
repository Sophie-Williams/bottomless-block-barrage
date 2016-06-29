#ifndef PUZZLE_SCENE
#define PUZZLE_SCENE

#include "game_scene.hpp"
#include <string>
#include <util/text.hpp>

class PuzzleScene : public GameScene
{
public:
    struct PuzzleConfig
    {
        std::string filename;
    };
    PuzzleScene(const PuzzleConfig& config);
    virtual void init_panel_table();
    virtual void update_input();
    virtual void update_windows();
    bool is_gameover() const;
    virtual void draw_top();
private:
    PuzzleConfig puzzle_config;
    Text debug;
};


#endif

