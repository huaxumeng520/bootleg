
#pragma once

#include <KIT/Engine.hpp>
#include <KIT/Assets/Texture.hpp>

namespace bootleg 
{
  class UIBox;
  class UIMask;

  enum BannerMode 
  {
    BM_InactiveRow = 0,
    BM_ActiveRow = 1,
    BM_Selected = 2
  };

  class UIBanner 
  {
  public:
    UIBanner(kit::Engine *engine, kit::TexturePtr texture, UIMask *mask);
    virtual ~UIBanner();

    void update(double seconds);
    void render(glm::vec2 position);

    void mode(BannerMode newMode, bool instant = false);

    BannerMode mode() const
    {
      return m_mode;
    }

  protected:

    glm::vec2 modeSize(BannerMode mode);
    glm::vec4 modeUvRect(BannerMode mode);
    float modeAlpha(BannerMode mode);
    float modeBorderSize(BannerMode mode);
    glm::vec2 modeInfoSize(BannerMode mode);

    kit::Engine *m_engine = nullptr;

    UIMask *m_mask = nullptr;

    BannerMode m_mode = BM_InactiveRow;
    BannerMode m_oldMode = BM_InactiveRow;
    float m_modeDelta = 1.0f;

    kit::TexturePtr m_bannerTexture;
    UIBox *m_mainBox = nullptr;

    glm::vec2 m_size;
    glm::vec4 m_uvRect;
    float m_alpha = 0.0f;
    float m_borderSize = 0.0f;
    glm::vec2 m_infoSize;

  };

}