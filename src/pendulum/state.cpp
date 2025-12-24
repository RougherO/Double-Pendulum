#include "pendulum/state.hpp"

namespace Pendulum {
void init_state(PendulumState& state)
{
    using namespace Constants;

    state.length1     = default_length1;
    state.length2     = default_length2;
    state.mass1       = default_mass1;
    state.mass2       = default_mass2;
    state.curr_angle1 = default_angle1;
    state.curr_angle2 = default_angle2;
}
}