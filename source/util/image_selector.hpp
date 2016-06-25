#ifndef IMAGE_SELECTOR_HPP
#define IMAGE_SELECTOR_HPP

#include "color_modulation.hpp"
#include "widget.hpp"
#include "texture.hpp"
#include "hid_helper.hpp"

class ImageSelector : public Widget
{
public:
    struct ImageSelectorPayload
    {
        ImageSelectorPayload(const void* src_buffer, int pitch, int height, const void* data) : texture(src_buffer, pitch, height), payload(data) {}
        Texture texture;
        const void* payload;
    };
    ImageSelector() {}
    ImageSelector(int x, int y, int width, int height);
    void create(int x, int y, int width, int height);
    void update();
    void draw();
    void add(const std::string& group, const void* src_buffer, int pitch = -1, const void* client_data = nullptr);
    int selection() const {return index;}
    int sub_selection() const {return sub_index;}
    const void* client_data() const {return images.at(keys[index])[sub_index].payload;}
private:
    std::vector<std::string> keys;
    std::map<std::string, std::vector<ImageSelectorPayload>> images;
    int index;
    int sub_index;
    ColorModulation color;
    KeyRepeatStore repeat;
};


#endif
