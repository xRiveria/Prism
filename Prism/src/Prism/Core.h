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

#ifdef PRISM_ENABLE_ASSERTS //Zero is intepreted as false, while anything non-zero is true. 
	#define PRISM_ASSERT(x, ...) {!(x)) { PRISM_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugBreak(); }}
	#define PRISM_ENGINE_ASSERT(x, ...) {!(x)) { PRISM_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugBreak(); }}
#else
	#define PRISM_ASSERT(x, ...)
	#define PRISM_ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)