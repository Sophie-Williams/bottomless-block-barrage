#ifndef BATCH_HPP
#define BATCH_HPP

#include <sf2d.h>
#include "texture.hpp"

class Batch
{
public:
    Batch(int size);
    ~Batch();
    void start();
    void draw(Texture& tex, int x = 0, int y = 0);
    void draw(Texture& tex, int x, int y, int sx, int sy, int sw, int sh);
    void end();
private:
    sf2d_batch* batch;
};


#endif
