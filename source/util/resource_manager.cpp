#include "resource_manager.hpp"

Texture* ResourceManager::load_image(const void* data, int width, int height)
{
	std::map<const void*, std::unique_ptr<Texture>>::iterator i;

	i = images.find(data);
	if(i != images.end())
		return i->second.get();

    Texture* image = new Texture(data, width, height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	images[data].reset(image);

    return image;
}

bool ResourceManager::release_image(const void* data)
{
    std::map<const void*, std::unique_ptr<Texture>>::iterator i;

    i = images.find(data);
    if (i != images.end())
        return false;

    images.erase(i);
    return true;
}
