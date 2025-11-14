#pragma once
#include <vector>

#include "utils.hpp"

#include "SFML/Graphics/Drawable.hpp"

template <typename EntityType>
class Sprite : public sf::Drawable {
public:
    Sprite(EntityType const& entity)
        : m_entity { &entity }
    {
    }

    Sprite(Sprite const&)                    = delete;
    auto operator=(Sprite const&) -> Sprite& = delete;

    Sprite(Sprite&&) noexcept                    = default;
    auto operator=(Sprite&&) noexcept -> Sprite& = default;

protected:
    EntityType const* m_entity;
};

template <typename... Sprites>
class SpriteManager {
public:
    SpriteManager(SpriteManager const&)                    = delete;
    auto operator=(SpriteManager const&) -> SpriteManager& = delete;

    SpriteManager(SpriteManager&&) noexcept                    = default;
    auto operator=(SpriteManager&&) noexcept -> SpriteManager& = default;

    static auto get_instance() -> SpriteManager&
    {
        static SpriteManager instance;
        return instance;
    }

    template <typename SpriteType, typename EntityType>
    auto create_sprite(EntityType const& entity) -> SpriteType&
        requires Contains<SpriteType, Sprites...>
              && (std::is_base_of_v<Sprite<EntityType>, SpriteType>)
    {
        auto& sprite_list = std::get<std::vector<SpriteType>>(m_sprites_list);
        sprite_list.emplace_back(entity);
        return sprite_list.back();
    }

private:
    SpriteManager() = default;

    std::tuple<std::vector<Sprites>...> m_sprites_list;
};