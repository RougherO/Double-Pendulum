#include "DPendulum.hpp"
#include <cmath>
#include <numbers>
#include <ranges>
#include "imgui.h"
#include <cstdlib>

using std::sin, std::cos;
constexpr float pi = std::numbers::pi_v<float>;

constexpr inline static auto rad_to_deg(float angle) noexcept -> float
{
    return angle * 180.F / pi;
}

DPendulum::DPendulum() noexcept
{
    m_bob1.setOrigin(m_bob1.getRadius(), m_bob1.getRadius());   // Setting origin of circle shape(bob) to the center of the circle
    m_bob2.setOrigin(m_bob2.getRadius(), m_bob2.getRadius());   // Initially the origin is at top left of the circle's enclosing rectangle
    m_reset_trail();
}

void DPendulum::position(sf::Vector2f const& coords) noexcept
{
    position_rod1(coords);
    m_rod1.setRotation(rad_to_deg(-m_curr_angle1));   // negative angle because SFML considers clockwise as positive and anticlockwise as negative

    position_rod2(m_bob1.getPosition());
    m_rod2.setRotation(rad_to_deg(-m_curr_angle2));

    std::ranges::for_each_n(m_trail.begin(), m_trail_length, [this](sf::Vertex& points) {
        points.position = m_bob2.getPosition();
    });
}

void DPendulum::position_rod1(sf::Vector2f const& coords) noexcept
{
    m_x_coord = coords.x;
    m_y_coord = coords.y;

    m_rod1.setPosition(m_x_coord, m_y_coord);
    position_bob1(coords);
    position_rod2(m_bob1.getPosition());
}

void DPendulum::position_bob1(sf::Vector2f const& rod_coords) noexcept
{
    auto [x_coord, y_coord] = rod_coords;

    m_bob1.setPosition(
        x_coord + m_length1 * sin(m_curr_angle1),
        y_coord + m_length1 * cos(m_curr_angle1));
}

void DPendulum::position_rod2(sf::Vector2f const& coords) noexcept
{
    auto [x_coord, y_coord] = coords;

    m_rod2.setPosition(x_coord, y_coord);
    position_bob2(coords);
}

void DPendulum::position_bob2(sf::Vector2f const& rod_coords) noexcept
{
    auto [x_coord, y_coord] = rod_coords;

    m_bob2.setPosition(
        x_coord + m_length2 * sin(m_curr_angle2),
        y_coord + m_length2 * cos(m_curr_angle2));
}

void DPendulum::set_mass1(float mass1) noexcept
{
    m_mass1      = mass1;
    float radius = m_mass1 / Defaults::MASS_TO_RADIUS_RATIO;
    m_bob1.setRadius(radius);
    m_bob1.setOrigin(radius, radius);
}

void DPendulum::set_mass2(float mass2) noexcept
{
    m_mass2      = mass2;
    float radius = m_mass2 / Defaults::MASS_TO_RADIUS_RATIO;
    m_bob2.setRadius(radius);
    m_bob2.setOrigin(radius, radius);
}

void DPendulum::set_length1(float length) noexcept
{
    m_length1 = length;
    m_rod1.setSize({ 1, length });

    position_bob1(m_rod1.getPosition());
    position_rod2(m_bob1.getPosition());
}

void DPendulum::set_length2(float length) noexcept
{
    m_length2 = length;
    m_rod2.setSize({ 1, length });

    position_bob2(m_rod2.getPosition());
}
void DPendulum::set_angle1(float angle) noexcept
{
    m_curr_angle1 = angle;
    m_rod1.setRotation(rad_to_deg(-m_curr_angle1));

    position_bob1(m_rod1.getPosition());
    position_rod2(m_bob1.getPosition());
}

void DPendulum::set_angle2(float angle) noexcept
{
    m_curr_angle2 = angle;
    m_rod2.setRotation(rad_to_deg(-m_curr_angle2));

    position_bob2(m_rod2.getPosition());
}

void DPendulum::set_gravity(float gravity) noexcept
{
    m_gravity = gravity;
}

auto DPendulum::m_accel_func(float a_vel1, float a_vel2) const noexcept -> std::pair<float, float>
{
    // Updating Angular acceleration of each pendulum per frame on this massive formula
    float num1_1 = -m_gravity * (2 * m_mass1 + m_mass2) * sin(m_curr_angle1);
    float num2_1 = m_mass2 * m_gravity * sin(m_curr_angle1 - 2 * m_curr_angle2);
    float num3_1 = 2 * sin(m_curr_angle1 - m_curr_angle2) * m_mass2 * (a_vel2 * a_vel2 * m_length2 + a_vel1 * a_vel1 * m_length1 * cos(m_curr_angle1 - m_curr_angle2));
    float den    = (2 * m_mass1 + m_mass2 - m_mass2 * cos(2 * (m_curr_angle1 - m_curr_angle2)));

    float num1_2 = 2 * sin(m_curr_angle1 - m_curr_angle2);
    float num2_2 = a_vel1 * a_vel1 * m_length1 * (m_mass1 + m_mass2);
    float num3_2 = m_gravity * (m_mass1 + m_mass2) * cos(m_curr_angle1);
    float num4_2 = a_vel2 * a_vel2 * m_length2 * m_mass2 * cos(m_curr_angle1 - m_curr_angle2);

    return { (num1_1 - num2_1 - num3_1) / (m_length1 * den) - m_damp_coeff * m_a_vel1, (num1_2 * (num2_2 + num3_2 + num4_2)) / (m_length2 * den) - m_damp_coeff * m_a_vel2 };
}

void DPendulum::m_verlet_solve(float delta) noexcept
{
    // Modified Velocity Verlet Solver when acceleration is dependent on velocity
    // along with position.

    // Source: https://gamedev.stackexchange.com/a/41917
    m_curr_angle1 += m_a_vel1 * delta + 0.5F * m_a_acc1 * delta * delta;
    m_curr_angle2 += m_a_vel2 * delta + 0.5F * m_a_acc2 * delta * delta;

    m_a_vel1 += m_a_acc1 * delta;
    m_a_vel2 += m_a_acc2 * delta;

    auto [new_acc1, new_acc2] = m_accel_func(m_a_vel1, m_a_vel2);

    m_a_vel1 = m_a_vel1 + 0.5F * (new_acc1 - m_a_acc1) * delta;
    m_a_vel2 = m_a_vel2 + 0.5F * (new_acc2 - m_a_acc2) * delta;

    m_a_acc1 = new_acc1;
    m_a_acc2 = new_acc2;
}

void DPendulum::m_update_positions() noexcept
{
    // After new angles are calculated, we update the angular
    // rotation of both pendulums
    m_rod1.setRotation(-rad_to_deg(m_curr_angle1));
    position_bob1(m_rod1.getPosition());

    m_rod2.setRotation(-rad_to_deg(m_curr_angle2));
    position_rod2(m_bob1.getPosition());
}

void DPendulum::m_reset_state() noexcept
{
    m_a_acc1 = m_a_acc2 = m_a_vel1 = m_a_vel2 = 0;
}

void DPendulum::m_reset_trail() noexcept
{
    std::ranges::for_each_n(m_trail.begin(), m_trail_length, [this, i = 0U](sf::Vertex& point) mutable {
        point.position = m_bob2.getPosition();
        point.color.a  = static_cast<uint8_t>(255 * (m_trail_length - i) / m_trail_length);   // Setting a fading alpha for all points in the trail
        i++;
    });
}

void DPendulum::m_update_trail() noexcept
{
    static uint8_t update_count { 1 };

    for (auto* ptr = m_trail.begin() + m_trail_length - 1; ptr != m_trail.begin(); --ptr) {
        ptr[0].position = ptr[-1].position;
        uint8_t alpha   = ptr[0].color.a;

        // Linearly interpolating between the last color and the next color
        ptr[0].color.r = std::lerp(ptr[0].color.r, ptr[-1].color.r, 0.5f);
        ptr[0].color.g = std::lerp(ptr[0].color.g, ptr[-1].color.g, 0.5f);
        ptr[0].color.b = std::lerp(ptr[0].color.b, ptr[-1].color.b, 0.5f);
        ptr[0].color.a = alpha;
    }

    m_trail[0].position = m_bob2.getPosition();
    update_count++;

    if (m_trail_multi_color) {
        if (update_count == Defaults::MAX_TRAIL_COLOR_LENGTH) {
            // Each color will have a length of at least MAX_TRAIL_COLOR_LENGTH in the trail
            // after which we add a new random color
            m_trail[0].color.r = std::rand() % 255;
            m_trail[0].color.g = std::rand() % 255;
            m_trail[0].color.b = std::rand() % 255;
            update_count       = 1;
        }
    } else {
        // If multi color option is disabled then apply the color picker's color
        m_trail[0].color.r = m_trail_color_picker[0] * 255;
        m_trail[0].color.g = m_trail_color_picker[1] * 255;
        m_trail[0].color.b = m_trail_color_picker[2] * 255;
    }
}

void DPendulum::m_render_trail_imgui()
{
    ImGui::Checkbox("Trail", &m_trail_enable);
    ImGui::BeginDisabled(!m_trail_enable);
    if (ImGui::SliderScalar("Trail Length", ImGuiDataType_U16,
                            &m_trail_length,
                            &Defaults::MIN_TRAIL, &Defaults::MAX_TRAIL, "%hu", ImGuiSliderFlags_AlwaysClamp)) {   // If trail length changes we reset the entire trail
        m_reset_trail();
    }
    ImGui::ColorEdit3("Trail Colour", m_trail_color_picker.data());
    ImGui::Checkbox("Multi Coloured", &m_trail_multi_color);
    ImGui::EndDisabled();
}

void DPendulum::m_render_mass1_imgui()
{
    if (ImGui::SliderFloat("Mass*##1", &m_mass1, Defaults::MIN_MASS, Defaults::MAX_MASS, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_mass1(m_mass1);
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::m_render_mass2_imgui()
{
    if (ImGui::SliderFloat("Mass*##2", &m_mass2, Defaults::MIN_MASS, Defaults::MAX_MASS, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_mass2(m_mass2);
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::m_render_length1_imgui()
{
    // ##1 is used for ImGui's internal ID system to recognise this and the length2 SLiderFloat as differnt so they need different labels
    // ##N will be omitted when rendered as Text on screen
    if (ImGui::SliderFloat("Length*##1", &m_length1, Defaults::MIN_LENGTH, Defaults::MAX_LENGTH, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_length1(m_length1);
        position_rod2(m_bob1.getPosition());
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::m_render_length2_imgui()
{
    if (ImGui::SliderFloat("Length*##2", &m_length2, Defaults::MIN_LENGTH, Defaults::MAX_LENGTH, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_length2(m_length2);
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::m_render_angle1_imgui()
{
    if (ImGui::SliderAngle("Angle*##1", &m_initial_angle1, rad_to_deg(Defaults::MIN_ANGLE), rad_to_deg(Defaults::MAX_ANGLE), "%.0f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_angle1(m_initial_angle1);
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::m_render_angle2_imgui()
{
    if (ImGui::SliderAngle("Angle*##2", &m_initial_angle2, rad_to_deg(Defaults::MIN_ANGLE), rad_to_deg(Defaults::MAX_ANGLE), "%.0f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_angle2(m_initial_angle2);
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::m_render_gravity_imgui()
{
    if (ImGui::SliderFloat("Gravity*", &m_gravity, Defaults::MIN_GRAVITY, Defaults::MAX_GRAVITY, "%.2f", ImGuiSliderFlags_AlwaysClamp)) {
        m_reset_state();
        set_gravity(m_gravity);
    }
    m_pause_req |= ImGui::IsItemActive();
}

void DPendulum::update(float delta) noexcept
{
    if (!m_pause && !m_pause_req) {
        m_verlet_solve(delta);
        m_update_positions();
        m_update_trail();
    }
    m_pause_req = false;   // Refreshing pause_req to take pause requests in the next frame
}

void DPendulum::render_imgui()
{
    m_render_gravity_imgui();
    m_render_trail_imgui();

    ImGui::Text("Rod1");
    m_render_mass1_imgui();
    m_render_length1_imgui();
    m_render_angle1_imgui();

    ImGui::Text("Rod2");
    m_render_mass2_imgui();
    m_render_length2_imgui();
    m_render_angle2_imgui();
}

void DPendulum::pause(bool value) noexcept
{
    m_pause = value;
}

auto DPendulum::pause() const noexcept -> bool
{
    return m_pause;
}