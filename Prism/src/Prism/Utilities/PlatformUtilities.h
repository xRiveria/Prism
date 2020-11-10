#pragma once
#include <string>

namespace Prism
{
	class FileDialogs
	{
	public:
		//These return empty strings if cancelled.
		static std::string OpenFile(const char* filter); //Filter is essentially the extension filtering we see when we try to open a file.
		static std::string SaveFile(const char* filter); 
	};
}
