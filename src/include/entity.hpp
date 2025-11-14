#include <vector>

#include "sprites.hpp"

template <typename... Entities>
class EntityManager {
public:
    static_assert(((!std::is_copy_constructible_v<Entities> && !std::is_copy_assignable_v<Entities>) && ...), "Expected all entity types to be non copyable");

    EntityManager(EntityManager const&)                    = delete;
    auto operator=(EntityManager const&) -> EntityManager& = delete;

    EntityManager(EntityManager&&) noexcept                    = default;
    auto operator=(EntityManager&&) noexcept -> EntityManager& = default;

    static auto get_instance() -> EntityManager&
    {
        static EntityManager instance;
        return instance;
    }

    template <typename EntityType, typename... Args>
    auto create_entity(Args&&... args) -> EntityType&
        requires Contains<EntityType, Entities...>
    {
        auto& entity_list = std::get<std::vector<EntityType>>(m_entities_list);
        entity_list.emplace_back(std::forward<Args>(args)...);
        return entity_list.back();
    }

private:
    EntityManager() = default;

    std::tuple<std::vector<Entities>...> m_entities_list;
};