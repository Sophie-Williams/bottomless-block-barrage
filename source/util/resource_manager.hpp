#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <memory>
#include <sf2d.h>

class ResourceManager
{
    public:
        static ResourceManager& instance()
        {
            static ResourceManager singleton;
            return singleton;
        }
        ~ResourceManager();
        sf2d_texture* load_image(const std::string& path);
        bool release_image(const std::string& path);
    private:
        ResourceManager() {};                                       // Private constructor
        ResourceManager(const ResourceManager&);                    // Prevent copy-construction
        ResourceManager& operator=(const ResourceManager&);         // Prevent assignment
        std::map<std::string, sf2d_texture*> images;
};

inline ResourceManager& ResourceManager()
{
    return ResourceManager::instance();
}

#endif

