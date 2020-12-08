

#include "Dashboard/UI/UIBanner.hpp"

#include "Dashboard/UI/UIBox.hpp"
#include "Dashboard/Resolution.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/RenderManager.hpp>

bootleg::UIBanner::UIBanner(kit::Engine *engine, kit::TexturePtr texture, UIMask *mask)
    : m_engine(engine), m_bannerTexture(texture), m_mask(mask)
{
  m_mainBox = new UIBox(m_engine, {}, {}, m_bannerTexture, m_mask);
  m_mainBox->origin(O_Center);
}

bootleg::UIBanner::~UIBanner()
{
  delete m_mainBox;
}

void bootleg::UIBanner::update(double seconds)
{
  if (m_modeDelta < 1.0f)
  {
    m_modeDelta = glm::mix(m_modeDelta, 1.0f, seconds * 16.0f);

    if (m_modeDelta > 1.0f)
      m_modeDelta = 1.0f;

    if (m_modeDelta >= 1.0f)
    {
      m_oldMode = m_mode;
    }
  }
  
  if (m_oldMode != m_mode)
  {
    auto oldSize = modeSize(m_oldMode);
    auto oldAlpha = modeAlpha(m_oldMode);
    auto oldUvRect = modeUvRect(m_oldMode);
    auto oldBorderSize = modeBorderSize(m_oldMode);
    auto oldInfoSize = modeInfoSize(m_oldMode);

    auto newSize = modeSize(m_mode);
    auto newAlpha = modeAlpha(m_mode);
    auto newUvRect = modeUvRect(m_mode);
    auto newBorderSize = modeBorderSize(m_mode);
    auto newInfoSize = modeInfoSize(m_mode);

    m_size = glm::mix(oldSize, newSize, m_modeDelta);
    m_alpha = glm::mix(oldAlpha, newAlpha, m_modeDelta);
    m_uvRect = glm::mix(oldUvRect, newUvRect, m_modeDelta);
    m_borderSize = glm::mix(oldBorderSize, newBorderSize, m_modeDelta);
    m_infoSize = glm::mix(oldInfoSize, newInfoSize, m_modeDelta);
  }
  else 
  {
    m_size = modeSize(m_mode);
    m_alpha = modeAlpha(m_mode);
    m_uvRect = modeUvRect(m_mode);
    m_borderSize = modeBorderSize(m_mode);
    m_infoSize = modeInfoSize(m_mode);
  }
  
}

void bootleg::UIBanner::render(glm::vec2 position)
{
  static const glm::vec4 borderColor = wir::srgbEncode({0.0f, 70.f / 255.f, 180.f / 255.f, 1.0f});
  static const glm::vec4 infoColor = {0.0f, 0.0f, 0.0f, 0.95f};

  auto topLeft = applyOrigin(O_Center, position, m_size);
  auto r = m_engine->renderManager();

  m_mainBox->position(position);
  m_mainBox->size(m_size);
  m_mainBox->color({1.0f, 1.0f, 1.0f, m_alpha});
  m_mainBox->uvRect(m_uvRect);
  m_mainBox->render();

  if (m_borderSize > 0.0f)
  {
    r->sprite(topLeft, {m_size.x, m_borderSize}, borderColor);
    r->sprite(topLeft + glm::vec2{0.0f, m_size.y - m_borderSize}, {m_size.x, m_borderSize}, borderColor);
    r->sprite(topLeft + glm::vec2{0.0f, m_borderSize}, {m_borderSize, m_size.y - m_borderSize * 2.0f}, borderColor);
    r->sprite(topLeft + glm::vec2{m_size.x - m_borderSize, m_borderSize}, {m_borderSize, m_size.y - m_borderSize * 2.0f}, borderColor);
  }

}

void bootleg::UIBanner::mode(BannerMode newMode, bool instant)
{
  if (newMode == m_mode)
  {
    return;
  }

  m_oldMode = m_mode;
  m_mode = newMode;
  m_modeDelta = instant ? 1.0f : 0.0f;
  m_mainBox->mask(newMode == BM_Selected ? nullptr : m_mask);
}

glm::vec2 bootleg::UIBanner::modeSize(BannerMode mode)
{
  /** 
   * Aspect ratio is:  1.6666667:1, or 1:0.6
   * Select ratio is: 2.0474:1.0, or 1.0:0.488426
   * Select to UV is 0.81404333
   */
  switch (mode)
  {
  case BM_ActiveRow:
    return {320.f, 192.f};
  case BM_InactiveRow:
    return {320.f, 192.f};
  case BM_Selected:
    return {416.f, 250.f};
  default:
    return {};
  }
}

glm::vec4 bootleg::UIBanner::modeUvRect(BannerMode mode)
{
  switch (mode)
  {
  case BM_ActiveRow:
    return {0.0f, 0.0f, 1.0f, 1.0f};
  case BM_InactiveRow:
    return {0.0f, 0.0f, 1.0f, 1.0f};
  case BM_Selected:
    return {0.0f, 0.0f, 1.0f, 1.0f};
  default:
    return {};
  }
}

float bootleg::UIBanner::modeAlpha(BannerMode mode)
{
  switch (mode)
  {
  case BM_ActiveRow:
    return 0.90f;
  case BM_InactiveRow:
    return 0.65f;
  case BM_Selected:
    return 1.0f;
  default:
    return 0.0f;
  }
}

float bootleg::UIBanner::modeBorderSize(BannerMode mode)
{
  switch (mode)
  {
  case BM_Selected:
    return 4.0f;
  default:
    return 0.0f;
  }
}

glm::vec2 bootleg::UIBanner::modeInfoSize(BannerMode mode)
{
  switch (mode)
  {
  case BM_Selected:
    return {0.0f, 80.0f};
  default:
    return {0.0f, 0.0f};
  }
}
