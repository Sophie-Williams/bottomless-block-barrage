#include "batch.hpp"

Batch::Batch(int size)
{
    batch = sf2d_create_batch(size);
}

Batch::~Batch()
{
    sf2d_free_batch(batch);
}

void Batch::start()
{
    sf2d_start_batch(batch);
}

void Batch::draw(Texture& tex, int x, int y)
{
    sf2d_batch_texture(tex.get_texture(), x, y);
}

void Batch::draw(Texture& tex, int x, int y, int sx, int sy, int sw, int sh)
{
    sf2d_batch_texture_part(tex.get_texture(), x, y, sx, sy, sw, sh);
}

void Batch::end()
{
    sf2d_end_batch();
}
