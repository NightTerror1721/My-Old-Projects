#pragma once

#include "core/unique_id.h"
#include "core/logger.h"
#include "path.h"
#include <memory>
#include <algorithm>
#include <unordered_map>

enum class ResourceType : u8
{
    Texture,
    Sound,
    Font,
};

template <ResourceType T>
class TypedResource;

class ResourceManager;

class Resource
{
public:
    using IdType = UniqueId<u64>;
    using Ptr = std::shared_ptr<Resource>;
    using WeakPtr = std::weak_ptr<Resource>;
    
    template <ResourceType T>
    friend class TypedResource;

    friend class ResourceManager;

private:
    static inline IdType::Generator IdGenerator{};

private:
    bool _loaded = false;
    ResourcePath _path;

protected:
    const IdType _id = IdGenerator();
    const ResourceType _type;

private:
    Resource(ResourceType type) : _type(type) {}

public:
    virtual ~Resource() { unload(); }

    Resource(const Resource&) = delete;
    Resource(Resource&&) noexcept = delete;

    Resource& operator=(const Resource&) = delete;
    Resource& operator=(Resource&&) noexcept = delete;

public:
    constexpr IdType id() const noexcept { return _id; }
    constexpr ResourceType type() const noexcept { return _type; }

private:
    void load(const ResourcePath& path)
    {
        if (_loaded)
            return;

        onLoad(path.fsPath());
        _path = path;
        _loaded = true;
    }

    void unload()
    {
        if (!_loaded)
            return;

        onUnload();
        _path = ResourcePath{};
        _loaded = false;
    }

protected:
    virtual void onLoad(const std::filesystem::path& path) = 0;
    virtual void onUnload() = 0;
};

template <ResourceType T>
class TypedResource : public Resource
{
public:
    using Ptr = std::shared_ptr<TypedResource<T>>;
    using WeakPtr = std::weak_ptr<TypedResource<T>>;

protected:
    TypedResource() : Resource(T) {}

public:
    virtual ~TypedResource() override = default;

    TypedResource(const TypedResource&) = delete;
    TypedResource(TypedResource&&) noexcept = delete;

    TypedResource& operator=(const TypedResource&) = delete;
    TypedResource& operator=(TypedResource&&) noexcept = delete;
};

template <typename T>
concept IsResource = DerivedFrom<T, Resource> && !SameAs<T, Resource> && !std::is_abstract_v<T>;
