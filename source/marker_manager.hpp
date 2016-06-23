#ifndef MARKER_MANAGER_HPP
#define MARKER_MANAGER_HPP

#include "panel.hpp"

#include <list>
#include <util/texture.hpp>

struct Marker
{
    enum Type
    {
        INVALID = 0,
        COMBO = 1,
        CHAIN = 2,
        CLINK = 3,
    };
    Marker(int px, int py, Type t, int val, int time) : x(px), y(py), type(t), value(val), lifetime(time) {}
    int x;
    int y;
    Type type;
    int value;
    int lifetime;
};

class MarkerManager
{
public:
    MarkerManager() {}
    MarkerManager(PanelSpeedSettings* s);
    void create(PanelSpeedSettings* s);
    void add(int x, int y, Marker::Type type, int value);
    void update();
    void draw(int offx, int offy);
private:
    // Not owned by this class.
    PanelSpeedSettings* settings;
    std::list<Marker> marker_list;
    Texture markers;
};


#endif
