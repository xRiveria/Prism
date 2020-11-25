#pragma once

namespace Prism
{
	class ApplicationVersion
	{
	public:

		ApplicationVersion(const int majorVersion, const int minorVersion) : m_majorVersion(majorVersion), m_minorVersion(minorVersion)
		{

		}

		inline const std::string RetrieveApplicationVersion() const { return std::string("Prism Version: ") + std::to_string(m_majorVersion) + "." + std::to_string(m_minorVersion); }
		void SetApplicationVersion(const int majorVersion, const int minorVersion);
		
	private:
		int m_majorVersion;
		int m_minorVersion;
	};
}