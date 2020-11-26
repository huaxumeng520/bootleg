

#include <KIT/EngineModule.hpp>

#include "Dashboard/Module.hpp"

extern "C"
{
  DASHAPI kit::EngineModule *allocateModuleInstance(kit::Engine *engine)
  {
    return new bootleg::Module(engine); 
  }

  DASHAPI void freeModuleInstance(kit::EngineModule *module)
  {
    delete module;
  }
}