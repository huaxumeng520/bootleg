
#include "Dashboard/Module.hpp"
#include "Dashboard/DashboardMode.hpp"

#include <KIT/Engine.hpp>
#include <KIT/Managers/GameManager.hpp>
#include <KIT/Managers/InputManager.hpp>
#include <KIT/Configuration.hpp>

#include <WIR/Error.hpp>

bootleg::Module::Module(kit::Engine *engine)
  : kit::EngineModule(engine)
{
    
}

bootleg::Module::~Module()
{

}

void bootleg::Module::initialize(kit::Configuration *configuration)
{

  auto game = gameManager();

  // Todo assign input devices

  // Always start in dashboard mode
  m_dashboardMode = new bootleg::DashboardMode(game);
  game->mode(m_dashboardMode);
}

void bootleg::Module::shutdown()
{
  engine()->gameManager()->mode(nullptr);

  delete m_dashboardMode;
}

void bootleg::Module::update(double seconds)
{

}

std::string const &bootleg::Module::name()
{
#if defined (DASH_DEBUG)
  static const std::string moduleName = "Bootleg Dashboard (DEBUG)";
#else 
  static const std::string moduleName = "Bootleg Dashboard";
#endif
  return moduleName;
}

