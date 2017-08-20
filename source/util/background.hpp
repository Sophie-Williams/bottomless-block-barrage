#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "batch.hpp"
#include "texture.hpp"
#include "widget.hpp"
#include <string>

/**
  * This class represents data for a background
  * Backgrounds have names their associated images
  * specified as a filename, speed, and a mode.<br>
  *
  * Background modes control how the background moves
  * and how it is drawn.<br>
  * A Background can move: stationary (not at all),
  * camera (moves with the camera), or automatically.<br>
  * A Background can be drawn once, or it can be
  * drawn repeatedly to fit the canvas.
  */
class Background : public Widget
{
public:
    /** Controls how the background moves */
    enum ScrollMode
    {
        /** Stationary background does not move at all */
        Stationary = 0,
        /** Scrolls with the camera */
        Camera = 1,
        /** Automatically scrolling background */
        Autoscroll = 2,
    };
    /** Controls how the background is drawn */
    enum DrawMode
    {
        /** Drawn once */
        Once = 0,
        /** Drawn repeatedly to fill the view */
        Repeating = 4,
        /** Drawn on Bottom Screen */
        BottomScreen = 0,
        /** Drawn on Top Screen */
        TopScreen = 8,
    };

    Background() : batch(50) {}
    /** Creates a new Background.
      * @param src_buffer pixel data.
      * @param sw Width of image.
      * @param sh Height of image.
      * @param sx Horizontal speed.
      * @param sy Vertical speed.
      * @param style Background style.
      */
    Background(const std::string& path, int sx, int sy, unsigned int style);
    void create(const std::string& path, int sx, int sy, unsigned int style);
    void update();
    void draw();

private:
    Batch batch;
    Texture texture;
    int speed_x, speed_y;
};


#endif

