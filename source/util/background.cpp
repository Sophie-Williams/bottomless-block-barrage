#include "background.hpp"
#include "scenes/scene.hpp" // FOR Screen Sizes

Background::Background(const void* src_buffer, int sw, int sh, int sx, int sy, unsigned int style) :
    Widget(0, 0, sw, sh, style), texture(src_buffer, sw, sh), speed_x(sx), speed_y(sy)
{
}

void Background::create(const void* src_buffer, int sw, int sh, int sx, int sy, unsigned int style)
{
    Widget::create(0, 0, sw, sh, style);
    texture.create(src_buffer, sw, sh);
    speed_x = sx;
    speed_y = sy;
}

void Background::update()
{
    if (style & Background::Autoscroll)
    {
        int sx, sy;
        x += speed_x;
        y += speed_y;
        if (style & Background::Repeating)
        {
            sx = texture.width();
            sy = texture.height();
        }
        else
        {
            if (style & Background::TopScreen)
            {
                sx = TOP_SCREEN_WIDTH;
                sy = TOP_SCREEN_HEIGHT;
            }
            else
            {
                sx = BOTTOM_SCREEN_WIDTH;
                sy = BOTTOM_SCREEN_HEIGHT;
            }
        }

        x = x % sx;
        y = y % sy;

        if (x < 0)
            x += sx;
        if (y < 0)
            y += sy;
    }
}

void Background::draw()
{
    int iw, ih, vw, vh;
    iw = texture.width();
    ih = texture.height();
    unsigned int mode = style & 7;

    if (style & Background::TopScreen)
    {
        vw = TOP_SCREEN_WIDTH;
        vh = TOP_SCREEN_HEIGHT;
    }
    else
    {
        vw = BOTTOM_SCREEN_WIDTH;
        vh = BOTTOM_SCREEN_HEIGHT;
    }

    if (mode == (Background::Stationary | Background::Once))
    {
        texture.draw(0, 0);
    }
    else if (mode == (Background::Stationary | Background::Repeating))
    {
        int sxf, syf;
        sxf = vw / iw;
        syf = vh / ih;
        for (int i = 0; i <= syf; i++)
        {
            for (int j = 0; j <= sxf; j++)
            {
                texture.draw(j * iw, i * ih);
            }
        }
    }
    else if (mode == (Background::Autoscroll | Background::Once))
    {
        texture.draw(x, y);
    }
    else if (mode == (Background::Autoscroll | Background::Repeating))
    {
        int sxf, syf;
        sxf = vw / iw;
        syf = vh / ih;
        for (int i = -1; i <= syf; i++)
        {
            for (int j = -1; j <= sxf; j++)
            {
                texture.draw(x + j * iw, y + i * ih);
            }
        }
    }
    /*else if (style == (Background::Camera | Background::Once))
    {
        gtx->DrawBitmap(image, vx * speedx, vy * speedy, iw, ih);
    }
    else if (style == (Background::Camera | Background::Repeating))
    {
        int sxi, syi, sxf, syf;
        sxi = int(vx * x / iw) - 1;
        syi = int(vy * y / ih) - 1;
        sxf = int((vx * x + vw) / iw);
        syf = int((vy * y + vh) / ih);
        for (int i = syi; i <= syf; i++)
        {
            for (int j = sxi; j <= sxf; j++)
            {
                gtx->DrawBitmap(image, j * iw - vx * (speedx - 1), i * ih - vy * (speedy - 1), iw, ih);
            }
        }
    }*/
}
