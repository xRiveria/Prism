#pragma once

#ifdef PRISM_PLATFORM_WINDOWS
	#ifdef PRISM_BUILD_DLL
		#define PRISM_API __declspec(dllexport)
	#else
		#define PRISM_API __declspec(dllimport)
	#endif

#else
	#error Prism Engine only supports Windows!
#endif

#define BIT(x) (1 << x)