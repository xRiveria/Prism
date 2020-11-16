#pragma once
#include <string>
#include <optional>

namespace Prism
{
	class FileDialogs
	{
	public:
		//These return empty strings if cancelled.
		static std::optional<std::string> OpenFile(const char* filter); //Filter is essentially the extension filtering we see when we try to open a file.
		static std::optional<std::string> SaveFile(const char* filter); 
	};
}
