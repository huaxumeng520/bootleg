#pragma once

#include <KIT/EngineModule.hpp>

#include <Dashboard/Export.hpp>

namespace kit
{
  class Engine;
  class Configuration;
}

namespace bootleg
{
  class DashboardMode;

  class DASHAPI Module : public kit::EngineModule
  {
  public:
    Module(kit::Engine *engine);
    virtual ~Module();

    virtual void initialize(kit::Configuration *configuration) override;
    virtual void shutdown() override;

    virtual void update(double seconds) override;

    virtual std::string const & name()override;

  protected:

    bootleg::DashboardMode *m_dashboardMode = nullptr;
  };



}