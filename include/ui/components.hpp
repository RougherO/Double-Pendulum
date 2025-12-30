#pragma once
#include <concepts>
#include <utility>

#include "core/types.hpp"
#include "imgui.h"

namespace UI {
namespace Components {
    inline void slider(char const* label, float& data, float min, float max)
    {
        ImGui::SliderScalar(label, ImGuiDataType_Float,
                            &data, &min, &max,
                            "%.2f", ImGuiSliderFlags_ClampOnInput);
    }

    inline void slider(char const* label, Core::u64& data, Core::u64 min, Core::u64 max)
    {
        ImGui::SliderScalar(label, ImGuiDataType_U64,
                            &data, &min, &max,
                            "%lu", ImGuiSliderFlags_ClampOnInput);
    }

    template <typename... Args>
    inline void text(char const* fmt, Args&&... args)
    {
        ImGui::Text(fmt, std::forward<Args>(args)...);
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
}