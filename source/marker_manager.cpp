#include "marker_manager.hpp"
#include <vector>

const std::string marker_gfx = "romfs:/graphics/game/markers.png";

MarkerManager::MarkerManager(PanelSpeedSettings* s) : settings(s), markers(marker_gfx)
{
}

void MarkerManager::create(PanelSpeedSettings* s)
{
    settings = s;
    markers.create(marker_gfx);
}

void MarkerManager::add(int x, int y, Marker::Type type, int value)
{
    marker_list.emplace_back(x, y, type, value, settings->pending_match);
}

void MarkerManager::update()
{
    std::vector<std::list<Marker>::iterator> to_remove;
    for (std::list<Marker>::iterator marker = marker_list.begin(); marker != marker_list.end(); ++marker)
    {
        marker->lifetime -= 1;
        if (marker->lifetime <= 0)
            to_remove.push_back(marker);
    }

    for (const auto& marker : to_remove)
    {
        marker_list.erase(marker);
    }
}

void MarkerManager::draw(int offx, int offy)
{
    for (const auto& marker : marker_list)
    {
        int draws = 1;
        int sx = 0, sy = 0;
        int s2x, s2y = 0;
        int s3x, s3y = 0;

        if (marker.type == Marker::COMBO)
        {
            if (marker.value < 10)
                sx = (marker.value - 4) * 16;
            else if (marker.value < 100)
            {
                draws = 2;
                sx = marker.value / 10 * 8;
                sy = 16;
                s2x = marker.value % 10 * 8;
                s2y = 32;
            }
            else
            {
                sx = 80;
                sy = 16;
            }
        }
        else
        {
            draws = 2;
            switch (marker.type)
            {
                case Marker::CHAIN:
                    sy = s2y = 48;
                    s3y = 64;
                    break;
                case Marker::CLINK:
                    sy = s2y = 80;
                    s3y = 96;
                    break;
                default:
                    break;
            }
            if (marker.value < 10)
            {
                s2x = 16 + 8 * marker.value;
            }
            else if (marker.value < 100)
            {
                draws = 3;
                sx += 16;
                s2x = (marker.value / 10 - 1) * 5;
                s2y = s3y;
                s3x = marker.value % 10 * 5 + 45;
            }
            else
            {
                draws = 1;
            }
        }

        float percent = marker.lifetime * 1.0f / settings->pending_match;
        int offset = (1 - percent) * 16;
        int y = marker.y - offset + offy;
        switch (draws)
        {
            case 1:
                markers.draw(offx + marker.x, y, sx, sy, 16, 16, percent);
                break;
            case 2:
                markers.draw(offx + marker.x, y, sx, sy, 8, 16, percent);
                markers.draw(offx + marker.x + 8, y, s2x, s2y, 8, 16, percent);
                break;
            case 3:
                markers.draw(offx + marker.x, y, sx, sy, 6, 16, percent);
                markers.draw(offx + marker.x + 6, y, s2x, s2y, 5, 16, percent);
                markers.draw(offx + marker.x + 11, y, s3x, s3y, 5, 16, percent);
                break;
        }
    }
}
