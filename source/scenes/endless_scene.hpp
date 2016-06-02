#ifndef ENDLESS_SCENE_HPP
#define ENDLESS_SCENE_HPP

#include "scene.hpp"
#include <windows/info_window.hpp>
#include <windows/ccc_window.hpp>
#include <util/texture.hpp>
#include "panel_table.hpp"
#include "animation_params.hpp"

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
    void draw_top_left();
    void draw_top_right();
    void draw_bottom();

private:
    Config config;
    std::unique_ptr<InfoWindow> info;
    std::unique_ptr<CCCWindow> ccc_stats;
    std::unique_ptr<Texture> panels;
    std::unique_ptr<Texture> selector;
    std::unique_ptr<Texture> border;
    std::unique_ptr<Texture> debug;
    std::unique_ptr<PanelTable> panel_table;
    AnimationParams frames;

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
