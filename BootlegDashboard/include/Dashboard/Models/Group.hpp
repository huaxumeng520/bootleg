
#pragma once

#include <KIT/Assets/Texture.hpp>
#include <WIR/Math.hpp>

namespace bootleg
{

  struct Group
  {
    uint64_t id{};
    std::string title;
    std::vector<uint64_t> games;
  };

} // namespace bootleg