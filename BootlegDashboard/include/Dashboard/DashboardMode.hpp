
#pragma once

#include <KIT/Game/GameMode.hpp>
#include <KIT/Game/PlayerState.hpp>
#include <KIT/Rendering/TextRenderer.hpp>

#include <Dashboard/Export.hpp>

namespace bootleg
{
  class DASHAPI DashboardMode : public kit::GameMode 
  {
    WIR_CLASS_DECLARATION()
  public:
    DashboardMode(wir::DynamicArguments const &args);
    DashboardMode(kit::GameManager *manager);
    virtual ~DashboardMode();

    virtual void onModeActivated() override;
    virtual void onModeDeactivated() override;

    virtual void onWorldLoading() override;
    virtual void onWorldStart() override;
    virtual void onWorldTick(double seconds) override;
    virtual void onWorldDestroyed() override;

  protected:

    void handleNavigateHorizontal(float delta);
    void handleNavigateVertical(float delta);
    void handleNextView();
    void handlePreviousView();
    void handleSelect();
    void handleBack();

    kit::PlayerState *m_playerState = nullptr;
  };
}
