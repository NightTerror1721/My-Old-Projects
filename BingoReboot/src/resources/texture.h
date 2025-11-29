#pragma once

#include "resource.h"

class Texture : public TypedResource<ResourceType::Texture>
{
private:
    ALLEGRO_BITMAP* _bitmap = nullptr;

public:
    Texture() = default;
    virtual ~Texture() override = default;

    Texture(const Texture&) = delete;
    Texture(Texture&&) noexcept = delete;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) noexcept = delete;

public:
    inline usize width() const noexcept { return _bitmap ? static_cast<size_t>(al_get_bitmap_width(_bitmap)) : 0; }
    inline usize height() const noexcept { return _bitmap ? static_cast<size_t>(al_get_bitmap_height(_bitmap)) : 0; }

protected:
    virtual void onLoad(const std::filesystem::path& path) override
    {
        _bitmap = al_load_bitmap(path.string().c_str());
        if (!_bitmap)
        {
            _bitmap = nullptr;
            throw std::runtime_error("Failed to load texture from path: " + path.string());
        }
    }

    virtual void onUnload() override
    {
        if (_bitmap)
        {
            al_destroy_bitmap(_bitmap);
            _bitmap = nullptr;
        }
    }
};