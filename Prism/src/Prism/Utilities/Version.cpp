#include "PrismPrecompiledHeader.h"
#include "Version.h"

namespace Prism
{
	void ApplicationVersion::SetApplicationVersion(const int majorVersion, const int minorVersion)
	{
		m_majorVersion = majorVersion;
		m_minorVersion = minorVersion;
	}
}