#include "resource_manager.hpp"
#include <sfil.h>

ResourceManager::~ResourceManager()
{
    for (const auto& file_image : images)
        sf2d_free_texture(file_image.second);
    images.clear();
}

sf2d_texture* ResourceManager::load_image(const std::string& path)
{
	std::map<std::string, sf2d_texture*>::iterator i = images.find(path);

	if (i != images.end())
		return i->second;

    sf2d_texture* image = sfil_load_PNG_file(path.c_str(), SF2D_PLACE_RAM);
	images[path] = image;

    return image;
}

bool ResourceManager::release_image(const std::string& path)
{
    std::map<std::string, sf2d_texture*>::iterator i = images.find(path);

    if (i != images.end())
        return false;

    sf2d_free_texture(i->second);
    images.erase(i);

    return true;
}
