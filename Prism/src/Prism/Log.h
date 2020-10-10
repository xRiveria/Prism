#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace Prism
{
	class PRISM_API Log
	{
	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }         //Engine Logger
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }    //Application Logger

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


//Core Logging Macros
#define PRISM_ENGINE_ERROR(...)   ::Prism::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PRISM_ENGINE_INFO(...)    ::Prism::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PRISM_ENGINE_WARN(...)    ::Prism::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PRISM_ENGINE_TRACE(...)   ::Prism::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PRISM_ENGINE_FATAL(...)   ::Prism::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Logging Macros
#define PRISM_CLIENT_ERROR(...)   ::Prism::Log::GetClientLogger()->error(__VA_ARGS__)
#define PRISM_CLIENT_INFO(...)    ::Prism::Log::GetClientLogger()->info(__VA_ARGS__)
#define PRISM_CLIENT_WARN(...)    ::Prism::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PRISM_CLIENT_TRACE(...)   ::Prism::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PRISM_CLIENT_FATAL(...)   ::Prism::Log::GetClientLogger()->fatal(__VA_ARGS__)