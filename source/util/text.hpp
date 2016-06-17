#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "widget.hpp"
#include "font.hpp"

class Text : public Widget
{
public:
    Text() {}
    Text(const std::string& str, int x, int y, Font* font = NULL);
    virtual ~Text() {}
    virtual void create(const std::string& str, int x, int y, Font* font = NULL);
    virtual void update() {};
    virtual void draw();
    void set_value(const std::string& str) {text = str;}
    const std::string& get_value() const {return text;}
private:
    std::string text;
};

#endif

