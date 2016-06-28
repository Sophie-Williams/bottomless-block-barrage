#ifndef PUZZLE_SCENE
#define PUZZLE_SCENE

#include "game_scene.hpp"
#include <string>

class PuzzleScene : public GameScene
{
public:
    struct PuzzleConfig
    {
        const std::string& filename;
    };
    PuzzleScene(const PuzzleConfig& config);
    virtual void init_panel_table();
    virtual void update_input();
    virtual void update_windows();
    bool is_gameover() const;
private:
    PuzzleConfig puzzle_config;
};


#endif

