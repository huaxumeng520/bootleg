
#include "Dashboard/DashboardMode.hpp"

#include <KIT/Engine.hpp>

#include <KIT/Assets/Texture.hpp>

#include <KIT/Managers/AssetManager.hpp>
#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Managers/VRManager.hpp>
#include <KIT/Managers/RenderManager.hpp>
#include <KIT/Renderer/Renderer.hpp>

#include <KIT/Game/Object.hpp>
#include <KIT/Game/World.hpp>

#include <KIT/Game/PlayerState.hpp>


#include <KIT/Rendering/SpriteRenderer.hpp>

#include <WIR/String.hpp>

bootleg::DashboardMode::DashboardMode(wir::DynamicArguments const &args)
    : kit::GameMode(args)
{
}

bootleg::DashboardMode::DashboardMode(kit::GameManager *manager)
    : kit::GameMode(manager)
{
}

bootleg::DashboardMode::~DashboardMode()
{
}

void bootleg::DashboardMode::onModeActivated()
{
  m_playerState = engine()->gameManager()->playerState(0);
   
  // Bind inputs to events
  m_playerState->bindAxis("NavigateHorizontal", "gamepad_analog_x", wir::AT_Normal);
  m_playerState->bindAxis("NavigateVertical", "gamepad_analog_y", wir::AT_Normal);
  m_playerState->bindButton("NavigateHorizontal", "gamepad_dpad_right", wir::BT_AsAxisPositive);
  m_playerState->bindButton("NavigateVertical", "gamepad_dpad_down", wir::BT_AsAxisPositive);
  m_playerState->bindButton("NavigateHorizontal", "gamepad_dpad_left", wir::BT_AsAxisNegative);
  m_playerState->bindButton("NavigateVertical", "gamepad_dpad_up", wir::BT_AsAxisNegative);
  m_playerState->bindButton("NextView", "gamepad_shoulder_right", wir::BT_Down);
  m_playerState->bindButton("PreviousView", "gamepad_shoulder_left", wir::BT_Down);
  m_playerState->bindButton("Select", "gamepad_a", wir::BT_AsAxisPositive);
  m_playerState->bindButton("Back", "gamepad_b", wir::BT_AsAxisNegative);
  
  // Bind events to functions
  m_playerState->getAxisEvent("NavigateHorizontal") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateHorizontal);
  m_playerState->getAxisEvent("NavigateVertical") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNavigateVertical);
  m_playerState->getButtonEvent("NextView") += wir::MemberFunction(this, &bootleg::DashboardMode::handleNextView);
  m_playerState->getButtonEvent("PreviousView") += wir::MemberFunction(this, &bootleg::DashboardMode::handlePreviousView);
  m_playerState->getButtonEvent("Select") += wir::MemberFunction(this, &bootleg::DashboardMode::handleSelect);
  m_playerState->getButtonEvent("Back") += wir::MemberFunction(this, &bootleg::DashboardMode::handleBack);



}

void bootleg::DashboardMode::onModeDeactivated()
{
  
}

void bootleg::DashboardMode::onWorldLoading()
{
}

void bootleg::DashboardMode::onWorldStart()
{
 
}

void bootleg::DashboardMode::onWorldTick(double seconds)
{
  

}

void bootleg::DashboardMode::onWorldDestroyed()
{
}

void bootleg::DashboardMode::handleNavigateHorizontal(float delta)
{
}

void bootleg::DashboardMode::handleNavigateVertical(float delta)
{
}

void bootleg::DashboardMode::handleNextView()
{
}

void bootleg::DashboardMode::handlePreviousView()
{
}

void bootleg::DashboardMode::handleSelect()
{
}

void bootleg::DashboardMode::handleBack()
{
}
