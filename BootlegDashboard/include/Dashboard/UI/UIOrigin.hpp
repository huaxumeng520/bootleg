
#pragma once


#include <WIR/Math.hpp>

#include <cstdint>

namespace bootleg
{
  enum UIOrigin : uint8_t
  {
    O_Vertical = 0b0000'0011,
    O_Top = 0b0000'0000,
    O_VCenter = 0b0000'0001,
    O_Bottom = 0b0000'0010,

    O_Horizontal = 0b0000'1100,
    O_Left = 0b0000'0000,
    O_HCenter = 0b0000'0100,
    O_Right = 0b0000'1000,

    O_TopLeft = O_Top | O_Left,
    O_TopRight = O_Top | O_Right,
    O_BottomLeft = O_Bottom | O_Left,
    O_BottomRight = O_Bottom | O_Right,

    O_Center = O_VCenter | O_HCenter
  };

  glm::vec2 applyOrigin(UIOrigin origin, glm::vec2 const &position, glm::vec2 const &size);
}
