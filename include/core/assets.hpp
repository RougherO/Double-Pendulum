#pragma once
#include <filesystem>

namespace Assets {
inline auto const fonts = std::filesystem::path { ASSET_FOLDER } / "fonts";
}