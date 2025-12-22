#pragma once
#include <concepts>

#include "imgui.h"

namespace Componenets {
inline void slider(char const* label, float& data, float min, float max)
{
    ImGui::SliderScalar(label, ImGuiDataType_Float,
                        &data, &min, &max,
                        "%.2f", ImGuiSliderFlags_ClampOnInput);
}

inline void text(char const* label)
{
    ImGui::Text("%s", label);
};

inline void button(char const* label, std::invocable auto const& callback)
{
    if (ImGui::SmallButton(label)) {
        callback();
    }
};

template <typename... Components>
inline void horizontal_layout(Components... components)
{
    bool first { true };
    (((first ? (void)(first = false) : ImGui::SameLine()),
      components()),
     ...);
}
}