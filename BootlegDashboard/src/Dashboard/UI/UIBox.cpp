
#include "Dashboard/UI/UIBox.hpp"

#include <KIT/Managers/RenderManager.hpp>
#include <KIT/Configuration.hpp>


bootleg::UIBox::UIBox(kit::Engine *engine, glm::vec2 position, glm::vec2 size, kit::TexturePtr texture, bool alpha, glm::vec4 color)
    : m_engine(engine), m_position(position), m_size(size), m_texture(texture), m_color(color), m_alpha(alpha)
{
  
}

bootleg::UIBox::UIBox(kit::Engine *engine, glm::vec2 position, glm::vec2 size, glm::vec4 color)
    : m_engine(engine), m_position(position), m_size(size), m_texture(nullptr), m_color(color)
{
}

bootleg::UIBox::~UIBox()
{
}

void bootleg::UIBox::render()
{
  static bool useAlpha = m_engine->configuration()->get("DashboardAlphaBlend", true);

  auto r = m_engine->renderManager();
  auto pos = applyOrigin(m_origin, m_position, m_size);

  auto c = m_color;
  if (!useAlpha && c.a != 0.0f)
    c.a = 1.0f;


  r->sprite(pos, m_size, m_texture, m_alpha, c, m_uvRect);
  
}
