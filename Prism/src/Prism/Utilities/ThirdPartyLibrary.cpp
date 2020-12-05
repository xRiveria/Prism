#include "PrismPrecompiledHeader.h"
#include "ThirdPartyLibrary.h"

namespace Prism
{
	void ThirdPartyLibrary::RegisterThirdPartyLibrary(const std::string& libraryName, const std::string& libraryVersion, const std::string& libraryURL)
	{
		m_ThirdPartyLibraries.emplace_back(libraryName, libraryVersion, libraryURL);
	}
}