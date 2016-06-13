#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "widget.hpp"

class Text : public Widget
{
public:
    Text() {}
    Text(int x, int y, const std::string& str);
    virtual void create(int x, int y, const std::string& str);
    virtual void update() {};
    virtual void draw();
    void set_value(const std::string& str) {text = str;}
    const std::string& get_value() const {return text;}
private:
    std::string text;
};

#endif

