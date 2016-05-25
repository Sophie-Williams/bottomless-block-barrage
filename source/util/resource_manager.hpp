#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <memory>
#include "texture.hpp"

class ResourceManager
{
    public:
        static ResourceManager& instance()
        {
            static ResourceManager singleton;
            return singleton;
        }
        ~ResourceManager() {}
        Texture* load_image(const void* data, int width, int height);
        bool release_image(const void* data);
        //Texture* load_image(const std::string& filename, bool& error);
        //bool release_image(const std::string& filename);
    private:
        ResourceManager() {};                                       // Private constructor
        ResourceManager(const ResourceManager&);                    // Prevent copy-construction
        ResourceManager& operator=(const ResourceManager&);         // Prevent assignment
        std::map<const void*, std::unique_ptr<Texture>> images;
};

inline ResourceManager& ResourceManager()
{
    return ResourceManager::instance();
}

#endif

