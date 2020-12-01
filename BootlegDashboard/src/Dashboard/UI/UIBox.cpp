
#include "Dashboard/UI/UIBox.hpp"
#include "Dashboard/UI/UIMask.hpp"

#include <KIT/Managers/RenderManager.hpp>

bootleg::UIBox::UIBox(kit::Engine *engine, glm::vec2 position, glm::vec2 size, kit::TexturePtr texture, UIMask *mask, glm::vec4 color)
    : m_engine(engine), m_position(position), m_size(size), m_texture(texture), m_mask(mask), m_color(color)
{
}

bootleg::UIBox::UIBox(kit::Engine *engine, glm::vec2 position, glm::vec2 size, glm::vec4 color, UIMask *mask)
    : m_engine(engine), m_position(position), m_size(size), m_texture(nullptr), m_mask(mask), m_color(color)
{
}

bootleg::UIBox::~UIBox()
{
}

void bootleg::UIBox::render()
{
  auto r = m_engine->renderManager();
  auto pos = applyOrigin(m_origin, m_position, m_size);

  if (m_mask)
  {
    auto maskPos = applyOrigin(m_mask->origin(), m_mask->position(), m_mask->size());
    r->spriteMasked(pos, m_size, m_texture, maskPos, m_mask->size(), m_mask->texture(), m_color, m_uvRect);
  }
  else 
  {
    r->sprite(pos, m_size, m_texture, m_color, m_uvRect);
  }
  
}
