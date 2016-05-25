#ifndef ENDLESS_SCENE_HPP
#define ENDLESS_SCENE_HPP

#include "scene.hpp"
#include <util/texture.hpp>
#include "panel_table.hpp"
#include "animation_params.hpp"

class EndlessScene : public Scene
{
public:
    enum Difficulty
    {
        EASY = 0,
        NORMAL = 1,
        HARD = 2,
    };
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
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();

private:
    Config config;
    std::unique_ptr<Texture> panels;
    std::unique_ptr<Texture> selector;
    std::unique_ptr<Texture> border;
    std::unique_ptr<PanelTable> panel_table;
    AnimationParams frames;

    int selector_x = 2;
    int selector_y = 6;

    int score = 0;
    int level = 0;

    int frame = 0;
    u64 last_frame = 0;
    u64 last_rise = 0;
};


#endif
