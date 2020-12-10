
#pragma once

#include <KIT/Game/GameMode.hpp>
#include <KIT/Game/PlayerState.hpp>
#include <KIT/Rendering/TextRenderer.hpp>

#include <KIT/Assets/Texture.hpp>

#include <Dashboard/Export.hpp>

#include <array>

namespace bootleg
{
  class UIBanner;

  class Repository;

  class DASHAPI DashboardMode : public kit::GameMode 
  {
    WIR_CLASS_DECLARATION()
  public:
    DashboardMode(wir::DynamicArguments const &args);
    DashboardMode(kit::GameManager *manager);
    virtual ~DashboardMode();

    virtual void onModeActivated() override;
    virtual void onModeDeactivated() override;

    virtual void update(double seconds) override;

  protected:

    void handleNavigateHorizontal(float delta);
    void handleNavigateVertical(float delta);

    void handleNavigateDown();
    void handleNavigateLeft();
    void handleNavigateRight();
    void handleNavigateUp();

    void handleNextView();
    void handlePreviousView();
    void handleSelect();
    void handleBack();


    Repository *m_repository = nullptr;
    kit::PlayerState *m_playerState = nullptr;


    //glm::vec4 m_backgroundDarken = wir::srgbEncode({.086f, .082f, .078f, 0.38f});

    //kit::TexturePtr m_backgroundNormal;
    kit::TexturePtr m_backgroundBlurred;
    /*float m_backgroundAlpha = 1.0f;
    float m_backgroundAlphaTarget = 0.0f;
    float m_backgroundAlphaSpeed = 0.65f; 
    */
    void updateBackground(double seconds);


    //UIMask *m_bannerMask = nullptr;
    std::array<UIBanner *, 8> m_dummyBanners;
    /*
    kit::Text *m_clock = nullptr;

    kit::Text *m_groupMostPlayed = nullptr;
    kit::Text *m_groupNintendoSwitch = nullptr;

    kit::Text *m_menuGames = nullptr;
    kit::Text *m_menuStore = nullptr;
    kit::Text *m_menuDownloads = nullptr;
    kit::Text *m_menuSettings = nullptr;*/
  };
}

