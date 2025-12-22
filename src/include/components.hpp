#pragma once
#include <concepts>
#include <functional>

#include "imgui.h"

namespace Componenets {
struct Slider {
    Slider(char const* label, float& data, float min, float max)
        : m_label { label }
        , m_data { data }
        , m_min { min }
        , m_max { max }
    {
    }

    void operator()()
    {
        ImGui::SliderScalar(m_label, ImGuiDataType_Float,
                            &m_data, &m_min, &m_max,
                            "%.2f", ImGuiSliderFlags_ClampOnInput);
    }

private:
    char const* m_label {};
    float& m_data;
    float m_min {};
    float m_max {};
};

struct Text {
    Text(char const* label)
        : m_label { label }
    {
    }

    void operator()()
    {
        ImGui::Text("%s", m_label);
    }

private:
    char const* m_label {};
};

struct Button {
    Button(char const* label, std::invocable auto const& callback)
        : m_label { label }
        , m_callback { callback }
    {
    }

    void operator()()
    {
        if (ImGui::SmallButton(m_label)) {
            m_callback();
        }
    }

private:
    char const* m_label {};
    std::function<void()> m_callback {};
};
}