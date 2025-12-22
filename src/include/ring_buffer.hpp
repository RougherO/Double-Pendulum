#pragma once
#include <array>
#include <numeric>
#include <type_traits>

#include "types.hpp"

namespace DataStructures {
template <typename T, Core::usize N>
    requires std::is_arithmetic_v<T>
class RingBuffer {
public:
    using buffer_type = std::array<T, N>;

    void push_back(T elem)
    {
        m_buffer[m_curr_idx] = std::move(elem);
        m_curr_idx           = (m_curr_idx + 1) % N;
    }

    auto reduce(std::invocable<T, T> auto&& f)
    {
        return std::reduce(m_buffer.begin(), m_buffer.end(), T {}, std::forward_like<decltype(f)>(f));
    }

private:
    buffer_type m_buffer;
    Core::usize m_curr_idx {};
};
}
