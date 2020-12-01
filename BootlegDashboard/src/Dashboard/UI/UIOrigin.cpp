
#include "Dashboard/UI/UIOrigin.hpp"


glm::vec2 bootleg::applyOrigin(UIOrigin origin, glm::vec2 const &position, glm::vec2 const &size)
{
  UIOrigin horizontal = UIOrigin(origin & O_Horizontal);
  UIOrigin vertical = UIOrigin(origin & O_Vertical);

  glm::vec2 output;

  switch (horizontal)
  {
  case O_Left:
    output.x = position.x;
    break;
  case O_HCenter:
    output.x = position.x - size.x / 2.0f;
    break;
  case O_Right:
    output.x = position.x - size.x;
    break;
  }

  switch (vertical)
  {
  case O_Top:
    output.y = position.y;
    break;
  case O_VCenter:
    output.y = position.y - size.y / 2.0f;
    break;
  case O_Bottom:
    output.y = position.y - size.y;
    break;
  }

  return output;
}
