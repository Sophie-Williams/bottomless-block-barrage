#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>

struct Input
{
    Input() : input(0) {}
    Input(uint16_t _input) : input(_input) {}

    bool button_a() const {return input & 0x80;}
    bool button_b() const {return input & 0x8000;}
    bool button_x() const {return input & 0x40;}
    bool button_y() const {return input & 0x4000;}
    bool button_l() const {return input & 0x10;}
    bool button_r() const {return input & 0x20;}

    bool button_start() const {return input & 0x1000;}
    bool button_select() const {return input & 0x2000;}

    bool button_left() const {return input & 0x200;}
    bool button_right() const {return input & 0x100;}
    bool button_up() const {return input & 0x800;}
    bool button_down() const {return input & 0x400;}
    bool on(unsigned int key) const {return (input & key) == key;}
    bool only(unsigned int key) const {return input == key;}
    bool any(unsigned int key) const {return (input & key) != 0;}
    bool off(unsigned int key) const {return !on(key);}
    uint16_t value() const {return input;}

    uint16_t input;

    static constexpr unsigned int BUTTON_L = 0x10;
    static constexpr unsigned int BUTTON_R = 0x20;
    static constexpr unsigned int BUTTON_X = 0x40;
    static constexpr unsigned int BUTTON_A = 0x80;
    static constexpr unsigned int BUTTON_RIGHT = 0x100;
    static constexpr unsigned int BUTTON_LEFT = 0x200;
    static constexpr unsigned int BUTTON_DOWN = 0x400;
    static constexpr unsigned int BUTTON_UP = 0x800;
    static constexpr unsigned int BUTTON_START = 0x1000;
    static constexpr unsigned int BUTTON_SELECT = 0x2000;
    static constexpr unsigned int BUTTON_Y = 0x4000;
    static constexpr unsigned int BUTTON_B = 0x8000;
    static constexpr unsigned int BUTTON_DIRECTIONAL = 0x0F00;
};

class InputManager
{
public:
    InputManager() {}
    void update(const Input& input);

    bool triggered(unsigned int key);
    bool triggered_only(unsigned int key);
    bool held(unsigned int key);
    bool held_only(unsigned int key);
    bool released(unsigned int key);
    bool released_only(unsigned int key);

    bool on(unsigned int key) {return current.on(key);}
    bool off(unsigned int key) {return current.off(key);}
    bool repeat(unsigned int key, unsigned int repeat_frames);
    bool repeat_quick(unsigned int key, unsigned int repeat_frames, unsigned int quick_delay, unsigned int quick_repeat_frames);

private:
    Input current, last;
    std::map<unsigned int, unsigned int> repeat_map;
};

#endif
