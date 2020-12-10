
#include "Dashboard/Resolution.hpp"
namespace bootleg
{
  glm::uvec2 targetResolution{1920, 1080};
}

glm::vec2 bootleg::targetSpace(glm::vec2 const &p)
{
  float multiplier = float(targetResolution.y) / float(referenceResolution.y);
  return p * multiplier;
}

float bootleg::targetSpace(float p)
{
  float multiplier = float(targetResolution.y) / float(referenceResolution.y);
  return p * multiplier;
}