#include "background.hpp"
#include "scenes/scene.hpp" // for screen sizes

Background::Background(const std::string& path, int sx, int sy, unsigned int style) :
    Widget(0, 0, 0, 0, style), batch(50), texture(path), speed_x(sx), speed_y(sy)
{
    width = texture.width();
    height = texture.height();
}

void Background::create(const std::string& path, int sx, int sy, unsigned int style)
{
    Widget::create(0, 0, 0, 0, style);
    texture.create(path);
    width = texture.width();
    height = texture.height();
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
    batch.start();
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
        batch.draw(texture);
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
                batch.draw(texture, j * iw, i * ih);
            }
        }
    }
    else if (mode == (Background::Autoscroll | Background::Once))
    {
        batch.draw(texture, x, y);
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
                batch.draw(texture, x + j * iw, y + i * ih);
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
    batch.end();
}
