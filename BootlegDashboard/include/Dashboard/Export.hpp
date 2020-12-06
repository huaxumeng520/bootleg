
#pragma once

#include <WIR/Platform.hpp>

#if defined(WIR_Windows)
#if defined(DASH_BUILD)
#define DASHAPI __declspec(dllexport)
#else 
#define DASHAPI __declspec(dllimport)
#endif 
#else 
#define DASHAPI
#endif
