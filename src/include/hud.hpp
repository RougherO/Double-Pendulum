#pragma once
#include <concepts>

#include "components.hpp"

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

struct MassSlider : Componenets::Slider {
    MassSlider(char const* label, float& data)
        : Componenets::Slider(label, data, Constants::min_mass, Constants::max_mass)
    {
    }
};

struct LengthSlider : Componenets::Slider {
    LengthSlider(char const* label, float& data)
        : Componenets::Slider(label, data, Constants::min_length, Constants::max_length)
    {
    }
};

struct EnvParamSlider : Componenets::Slider {
    EnvParamSlider(char const* label, float& data, float min, float max)
        : Componenets::Slider(label, data, min, max)
    {
    }
};

struct AngleSlider : Componenets::Slider {
    AngleSlider(char const* label, float& data)
        : Componenets::Slider(label, data, Constants::min_angle, Constants::max_angle)
    {
    }
};

using Componenets::Text, Componenets::Button;

template <std::invocable... Components>
void view(char const* name, std::invocable auto const& window_setter, Components... components)
{
    window_setter();

    ImGui::Begin(name, nullptr,
                 ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_NoResize
                     | ImGuiWindowFlags_HorizontalScrollbar);
    (components(), ...);
    ImGui::End();
}

}