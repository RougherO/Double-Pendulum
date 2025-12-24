#pragma once
#include <concepts>
#include <numbers>

#include "components.hpp"

namespace UI {
namespace HUD {
    namespace Constants {
        inline constexpr float min_gravity = 0.1F;
        inline constexpr float max_gravity = 20.F;

        inline constexpr float min_damp = 0.0F;
        inline constexpr float max_damp = 1.0F;

        inline constexpr float min_mass = 1.F;
        inline constexpr float max_mass = 200.F;

        inline constexpr float min_length = 1.F;
        inline constexpr float max_length = 200.F;

        inline constexpr float min_angle = 0;
        inline constexpr float max_angle = 2 * std::numbers::pi_v<float>;

        inline constexpr float max_hud_width = 200.F;
    }

    void mass_slider(char const* label, float& data)
    {
        Components::slider(label, data, Constants::min_mass, Constants::max_mass);
    }

    void length_slider(char const* label, float& data)
    {
        Components::slider(label, data, Constants::min_length, Constants::max_length);
    }

    void angle_slider(char const* label, float& data)
    {
        Components::slider(label, data, Constants::min_angle, Constants::max_angle);
    }

    void view(char const* name, std::invocable auto const& window_setter, std::invocable auto const& layout_setter)
    {
        window_setter();

        ImGui::Begin(name, nullptr,
                     ImGuiWindowFlags_NoMove
                         | ImGuiWindowFlags_NoResize
                         | ImGuiWindowFlags_HorizontalScrollbar);

        layout_setter();

        ImGui::End();
    }

}
}