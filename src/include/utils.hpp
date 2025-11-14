#include <type_traits>

template <typename Arg, typename... Pack>
concept Contains = (std::is_same_v<Arg, Pack> || ...);