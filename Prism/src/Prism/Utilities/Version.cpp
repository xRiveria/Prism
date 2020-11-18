#include "PrismPrecompiledHeader.h"
#include "Version.h"

namespace Prism
{
	const std::string ApplicationVersion::RetrieveApplicationVersion() const
	{
		return std::string("Prism Version: ") + std::to_string(m_majorVersion) + "." + std::to_string(m_minorVersion);
	}

	void ApplicationVersion::SetApplicationVersion(const int majorVersion, const int minorVersion)
	{
		m_majorVersion = majorVersion;
		m_minorVersion = minorVersion;
	}
}