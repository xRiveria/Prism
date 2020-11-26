#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "ILogInterface.h"

namespace Prism
{
	class PRISM_API Log
	{
	public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }         //Engine Logger
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }     //Application Logger
		inline static void SetEditorLogger(const std::weak_ptr<ILogInterface>& editorLogger) { s_EditorLogger = editorLogger; }
		inline static std::weak_ptr<ILogInterface>& GetEditorLogger() { return s_EditorLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::weak_ptr<ILogInterface> s_EditorLogger;
	};
}


//Core Logging Macros
#define PRISM_ENGINE_ERROR(...)			::Prism::Log::GetCoreLogger()->error(__VA_ARGS__);		  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_ENGINE_INFO(...)			::Prism::Log::GetCoreLogger()->info(__VA_ARGS__);		  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_ENGINE_WARN(...)			::Prism::Log::GetCoreLogger()->warn(__VA_ARGS__);		  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_ENGINE_TRACE(...)			::Prism::Log::GetCoreLogger()->trace(__VA_ARGS__);		  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_ENGINE_CRITICAL(...)      ::Prism::Log::GetCoreLogger()->critical(__VA_ARGS__);	  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
																								 																								
#define PRISM_CLIENT_ERROR(...)			::Prism::Log::GetClientLogger()->error(__VA_ARGS__);      if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_CLIENT_INFO(...)			::Prism::Log::GetClientLogger()->info(__VA_ARGS__);		  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_CLIENT_WARN(...)			::Prism::Log::GetClientLogger()->warn(__VA_ARGS__);		  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_CLIENT_TRACE(...)			::Prism::Log::GetClientLogger()->trace(__VA_ARGS__);	  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }
#define PRISM_CLIENT_CRITICAL(...)      ::Prism::Log::GetClientLogger()->critical(__VA_ARGS__);	  if (!::Prism::Log::GetEditorLogger().expired()) { ::Prism::Log::GetEditorLogger().lock()->Log("Log", 0); }