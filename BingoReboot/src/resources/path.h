#pragma once

#include "core/common.h"
#include <filesystem>

class ResourcePath
{
private:
    static inline constexpr char Separator = '/';

private:
    std::string _path;

public:
    ResourcePath() = default;
    ~ResourcePath() = default;

    ResourcePath(const ResourcePath&) = default;
    ResourcePath(ResourcePath&&) noexcept = default;

    ResourcePath& operator=(const ResourcePath&) = default;
    ResourcePath& operator=(ResourcePath&&) noexcept = default;

    bool operator==(const ResourcePath& other) const = default;
    auto operator<=>(const ResourcePath& other) const = default;

public:
    inline ResourcePath(std::string_view path) : _path(std::move(normalize(path))) {}
    
    inline ResourcePath& operator=(std::string_view path)
    {
        _path = normalize(path);
        return *this;
    }

    inline const std::string& str() const noexcept { return _path; }

    inline std::filesystem::path fsPath() const { return rootFsPath() / _path; }

    inline ResourcePath concat(const ResourcePath& other) const
    {
        if (_path.empty())
            return other;
        if (other._path.empty())
            return *this;
        return ResourcePath{ _path + Separator + other._path };
    }
    inline ResourcePath concat(std::string_view other) const
    {
        if (_path.empty())
            return other;
        if (other.empty())
            return *this;
        return ResourcePath{ _path + Separator + std::string{other} };
    }

public:
    inline ResourcePath operator/(const ResourcePath& other) const { return concat(other); }
    inline ResourcePath operator/(std::string_view other) const { return concat(other); }

private:
    static std::string normalize(std::string_view path)
    {
        std::string normalized{ path };
        std::ranges::replace(normalized, '\\', Separator);
        return normalized;
    }

    static std::filesystem::path rootFsPath()
    {
        static bool initialized = false;
        static std::filesystem::path rootPath;
        if (!initialized)
        {
            rootPath = std::filesystem::current_path() / "data";
            initialized = true;
        }
        return rootPath;
    }
};

namespace paths
{
    inline const ResourcePath root = ResourcePath{};
    inline const ResourcePath textures = ResourcePath{ "textures" };
    inline const ResourcePath sounds = ResourcePath{ "sounds" };
    inline const ResourcePath fonts = ResourcePath{ "fonts" };
}

template <>
struct std::hash<ResourcePath>
{
    std::size_t operator()(const ResourcePath& path) const noexcept
    {
        return std::hash<std::string>{}(path.str());
    }
};
