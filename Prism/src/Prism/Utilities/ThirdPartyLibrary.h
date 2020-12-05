#pragma once
#include <string>
#include <vector>

namespace Prism
{
	struct Library
	{
		Library(const std::string& libraryName, const std::string& libraryVersion, const std::string& libraryURL)
		{
			this->libraryName = libraryName;
			this->libraryVersion = libraryVersion;
			this->libraryURL = libraryURL;
		}

		std::string libraryName;
		std::string libraryVersion;
		std::string libraryURL;
	};

	class ThirdPartyLibrary
	{
	public:
		void RegisterThirdPartyLibrary(const std::string& libraryName, const std::string& libraryVersion, const std::string& libraryURL);
		const std::vector<Library>& GetThirdPartyLibraries() const { return m_ThirdPartyLibraries; }

	private:
		std::vector<Library> m_ThirdPartyLibraries;
	};
}