
#pragma once

#if defined(DASH_BUILD)
#define DASHAPI __declspec(dllexport)
#else 
#define DASHAPI __declspec(dllimport)
#endif 