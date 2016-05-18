#ifndef SLIDER_HPP
#define SLIDER_HPP

class Slider
{
public:
    Slider(int mi, int max, int val, int x, int y, int w, int h);
    void update();
    void draw();
    void set_active(bool a) {active = a;}
    bool is_active() const  {return active;}
    void set_hidden(bool h) {hidden = h;}
    bool is_hidden() const {return hidden;}
    int get_value() const {return value;}
    void set_value(int v) {value = v;}
    void center(int sw, int sh);
    void set_width(int w) {width = w;}
    void set_height(int h) {height = h;}
private:
    int min;
    int max;
    int value;
    int x;
    int y;
    int width;
    int height;
    bool hidden;
    bool active;
};

#endif
