
#pragma once

#include <KIT/Assets/Texture.hpp>
#include <KIT/Engine.hpp>

namespace bootleg
{
  class UIBox;
  class UIMask;
  class UIBanner;

  enum BannerRowMode
  {
    BR_Inactive = 0,
    BR_Selected = 1
  };

  class UIBannerRow
  {
  public:
    UIBannerRow(kit::Engine *engine, kit::TexturePtr texture, UIMask *mask);
    virtual ~UIBannerRow();

    void update(double seconds);
    void render(glm::vec2 position);

    void mode(BannerRowMode newMode);

    BannerRowMode mode() const
    {
      return m_mode;
    }

  protected:
    glm::vec2 modeTextSize(BannerRowMode mode);
    float modeTextAlpha(BannerRowMode mode);

    kit::Engine *m_engine = nullptr;

    UIMask *m_mask = nullptr;

    BannerRowMode m_mode = BR_Inactive;
    BannerRowMode m_oldMode = BR_Inactive;
    float m_modeDelta = 1.0f;

    glm::vec2 m_size;
    glm::vec4 m_uvRect;
    float m_alpha = 0.0f;
    float m_borderSize = 0.0f;
    glm::vec2 m_infoSize;
  };

} // namespace bootleg