#pragma once

#include "resource.h"
#include "texture.h"


class ResourceManager
{
private:
    std::unordered_map<ResourcePath, Resource::IdType> _resources;
    std::unordered_map<Resource::IdType, Resource::Ptr> _resourcesById;

public:
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) noexcept = delete;

    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) noexcept = delete;

private:
    ResourceManager() = default;

public:
    static ResourceManager& instance()
    {
        static ResourceManager instance;
        return instance;
    }

    bool unload(Resource::IdType id)
    {
        auto it = _resourcesById.find(id);
        if (it == _resourcesById.end())
            return false;

        Resource::Ptr resource = it->second;
        const ResourcePath& path = resource->_path;
        resource->unload();
        _resources.erase(path);
        _resourcesById.erase(it);
        return true;
    }
    bool unload(const Resource::Ptr& resource)
    {
        if (!resource)
            return false;
        return unload(resource->id());
    }

private:
    template <IsResource R> requires std::default_initializable<R>
    std::shared_ptr<R> loadResource(const ResourcePath& path, ResourceType type)
    {
        static_assert(std::derived_from<R, Resource>, "R must be derived from Resource");

        ResourcePath resolvedPath = resolvePath(path, type);
        auto it = _resources.find(resolvedPath);
        if (it != _resources.end())
        {
            Resource::IdType existingId = it->second;
            if (!_resourcesById.contains(existingId))
                throw std::runtime_error("Inconsistent resource manager state");

            auto resource = _resourcesById[existingId];
            if (resource->type() != type)
                throw std::runtime_error("Resource type mismatch");

            return std::static_pointer_cast<R>(resource);
        }

        auto resource = std::make_shared<R>();
        try
        {
            resource->load(resolvedPath);
            _resources.emplace(resolvedPath, resource->id());
            _resourcesById.emplace(resource->id(), resource);
            return resource;
        }
        catch (...)
        {
            logger::error("Failed to load resource from path: {}", path.str());
            return nullptr;
        }
    }

    static ResourcePath resolvePath(const ResourcePath& path, ResourceType type)
    {
        ResourcePath basePath;
        switch (type)
        {
        case ResourceType::Texture:
            basePath = paths::textures;
            break;
        case ResourceType::Sound:
            basePath = paths::sounds;
            break;
        case ResourceType::Font:
            basePath = paths::fonts;
            break;
        default:
            throw std::runtime_error("Unknown ResourceType");
        }
        return basePath / path;
    }
};
