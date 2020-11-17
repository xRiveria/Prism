#pragma once
#include <memory>

#ifdef PRISM_PLATFORM_WINDOWS
#if PRISM_DYNAMIC_LINK
	#ifdef PRISM_BUILD_DLL
		#define PRISM_API __declspec(dllexport)
	#else
		#define PRISM_API __declspec(dllimport)
	#endif
#else
	#define PRISM_API
#endif
#else
	#error Prism Engine only supports Windows!
#endif

#ifdef PRISM_DEBUG
	#define PRISM_ENABLE_ASSERTS
#endif;

//To Do: Make this macro be able to take in no arguments except condition.
#ifdef PRISM_ENABLE_ASSERTS //Zero is intepreted as false, while anything non-zero is true. 
#define PRISM_CLIENT_ASSERT(x, ...) { if(!(x)) { PRISM_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define PRISM_ENGINE_ASSERT(x, ...) { if(!(x)) { PRISM_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PRISM_ASSERT(x, ...)
	#define PRISM_ENGINE_ASSERT(x, ...)
#endif

#define PRISM_BIND_EVENT_FUNCTION(function) std::bind(&function, this, std::placeholders::_1)

namespace Prism
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Reference = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Reference<T> CreateReference(Args&& ...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	//Reference<Shader>
}